<?php
// $Id$
/**
 * @file
 * This file defines forms and pages for taxonomy_access_user administration
 */


function taxonomy_access_user_yes_no_render($value) {
  switch ($value) {
  case 1:
    return t('Yes');
  case 0:
    return t('Inherit');
  case -1:
    return t('No');
  default:
    return '';
  }
}

/**
 * Permission list for term
 */
function taxonomy_access_user_term_edit($term) {
  drupal_set_title(t('Permissions on %term', array('%term' => $term->name)));

  $pager_increment = variable_get('taxonomy_access_user_pager_increment', 10);

  $result = pager_query('SELECT
    tau.tauid,
    tau.uid,
    u.name,
    tau.grant_view,
    tau.grant_update,
    tau.grant_delete,
    tau.grant_admin
    FROM {term_access_user} tau
    LEFT JOIN {users} u ON tau.uid = u.uid
    WHERE tid = %d', $pager_increment, 0, NULL, $term->tid);

  $rows = array();
  while ($sqlrow = db_fetch_array($result)) {
    $row = array();
    $row['name'] = l($sqlrow['name'], 'user/'. $sqlrow['uid']);

    // permissions
    $row['grant_view'] = taxonomy_access_user_yes_no_render($sqlrow['grant_view']);
    $row['grant_update'] = taxonomy_access_user_yes_no_render($sqlrow['grant_update']);
    $row['grant_delete'] = taxonomy_access_user_yes_no_render($sqlrow['grant_delete']);
    $row['grant_admin'] = taxonomy_access_user_yes_no_render($sqlrow['grant_admin']);

    // Operations
    $row['ops']['edit'] = l(t('edit'), 'taxonomy/access/edit/'. $sqlrow['tauid'], array(
      'attributes' => array(
        'class' => 'popups-form-reload',
      ),
    ));
    $row['ops']['spacer'] = ' - ';

    $row['ops']['delete'] = l(t('delete'), 'taxonomy/access/delete/'. $sqlrow['tauid'], array(
      'attributes' => array(
        'class' => 'popups-form-reload',
      ),
    ));

    // Rendering operations
    $ops = '';
    foreach ($row['ops'] as $op) {
      $ops .= $op;
    }
    $row['ops'] = $ops;

    $rows[$sqlrow['tauid']] = $row;

  }

  $header = array(t('User'), t('View'), t('Update'), t('Delete'), t('Admin'), t('Operations'));
  $output = theme('table', $header, $rows, array('id' => 'tauid'));
  $output .= theme('pager', NULL, $pager_increment);

  return $output;
}


function taxonomy_access_user_add_access($form_state, $term) {
  global $user;

  if ($user->uid == $tauid->uid) {
    drupal_set_message(t('You are about editing your own access to this term'), 'warning');
  }

  $form = array();

  $ops = array('view', 'update', 'delete', 'admin');

  $account = user_load($tauid->uid);

  $form['user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => check_plain($account->name),
    '#weight' => -1,
  );

  taxonomy_access_user_access_add_edit_form($form, $term);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add access'),
    '#weight' => 10);

  $form['term'] = array(
    '#type' => 'value',
    '#value' => $term->tid,
  );

  return $form;
}

/**
 * This function validate the addition form
 */
function taxonomy_access_user_add_access_validate($form, &$form_state) {
  // Tests the user
  if (!$account = user_load(array('name' => $form_state['values']['user']))) {
    form_set_error('user', t('User does not exist'));
  }

  // Test if the user is already assigned to this term
  $result = db_query_range('SELECT tauid FROM {term_access_user} WHERE tid = %d AND uid = %d', $form_state['values']['term'], $account->uid, 0, 1);
  if ($row = db_fetch_object($result)) {
    form_set_error(
      'user',
      t(
        'User already assigned to this term, click <a href="@access-edit" class="popups-form-reload">here</a> to edit it',
        array(
          '@access-edit' => url(
            'taxonomy/access/edit/'. $row->tauid, 
            array(
              'query' => drupal_get_destination(),
            )
          ),
        )
      )
    );
  }
}

function taxonomy_access_user_add_access_submit($form, &$form_state) {
  module_load_include('inc', 'taxonomy_access_user');
  $account = user_load(array('name' => $form_state['values']['user']));

  taxonomy_access_user_control('add', taxonomy_get_term($form_state['values']['term']), $form_state['values'], $account);

  $terms = array(taxonomy_get_term($form_state['values']['term']));
  $terms = array_merge($terms, taxonomy_get_children($form_state['values']['term']));

  $affected_nids = _taxonomy_access_user_get_nodes_from_terms($terms);

  _taxonomy_access_user_node_access_update($affected_nids);
}

/**
 * This function generate common fields for access addition/deletion forms
 */
function taxonomy_access_user_access_add_edit_form(&$form, $term = NULL, $tauid = NULL) {
  $ops = array('view', 'update', 'delete', 'admin');

  if ($term != NULL) {
    $parents = taxonomy_get_parents($term->tid);
  }
  else {
    $parents = array(0);
  }

  if (!isset($tauid) {
    $tauid = new stdClass;
  }

  $form['permissions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Permissions settings'),
    '#weight' => 5,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  foreach ($ops as $op) {
    $form['permissions']['grant_'. $op] = array(
      '#type' => 'radios',
      '#title' => t('@op term', array('@op' => $op)),
      '#default_value' => intval($tauid->{'grant_'. $op}),
      '#options' => array(-1 => t('No'), 0 => t('Inherit'), 1 => t('Yes')),
    );
    // Unset the default value if it is the root tid
    if (count($parents) === 0) {
      unset($form['permissions']['grant_'. $op]['#options'][0]);
    }
  }
}

function taxonomy_access_user_access_edit($form_state, $tauid) {
  global $user;

  if ($user->uid == $tauid->uid) {
    drupal_set_message(t('You are about editing your own access to this term'), 'warning');
  }

  $form = array();


  $account = user_load($tauid->uid);

  $form['user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => check_plain($account->name),
    '#weight' => -1,
  );

  taxonomy_access_user_access_add_edit_form($form, taxonomy_get_term($tauid->tid), $tauid);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change access'),
    '#weight' => 10);

  $form['term'] = array(
    '#type' => 'value',
    '#value' => $tauid->tid,
    );
  $form['term_access_user'] = array(
    '#type' => 'value',
    '#value' => $tauid->tauid,
  );

  return $form;
}

function taxonomy_access_user_access_edit_validate($form, &$form_state) {
  if (!$account = user_load(array('name' => $form_state['values']['user']))) {
    form_set_error('user', t('User does not exist'));
  }

  $result = db_query_range('SELECT tauid FROM {term_access_user} WHERE tauid <> %d AND tid = %d AND uid = %d', $form_state['values']['term_access_user'], $form_state['values']['term'], $account->uid, 0, 1);
  if ($row = db_fetch_object($result)) {
    form_set_error(
      'user',
      t(
        'User already assigned to this term, click <a href="@access-edit" class="popups-form-reload">here</a> to edit it',
        array(
          '@access-edit' => url(
            'taxonomy/access/edit/'. $row->tauid, 
            array(
              'query' => drupal_get_destination(),
            )
          ),
        )
      )
    );
  }

}

function taxonomy_access_user_access_edit_submit($form, &$form_state) {
  module_load_include('inc', 'taxonomy_access_user');
  $account = user_load(array('name' => $form_state['values']['user']));

  $term_access_user = array(
    'tauid' => $form_state['values']['term_access_user'],
    'uid' => $account->uid,
    'grant_view' => $form_state['values']['grant_view'],
    'grant_update' => $form_state['values']['grant_update'],
    'grant_delete' => $form_state['values']['grant_delete'],
    'grant_admin' => $form_state['values']['grant_admin'],
  );

  if (db_result(db_query('SELECT tauid FROM {term_access_user} WHERE tauid = %d', $form_state['values']['term_access_user'])) === FALSE) {
    unset($term_access_user['tauid']);
    $term_access_user['tid'] = $form_state['values']['term'];
    drupal_write_record('term_access_user', $term_access_user);
  }
  else {
    drupal_write_record('term_access_user', $term_access_user, array('tauid'));
  )

  $terms = array(taxonomy_get_term($form_state['values']['term']));
  $terms = array_merge($terms, taxonomy_get_children($form_state['values']['term']));

  $affected_nids = _taxonomy_access_user_get_nodes_from_terms($terms);

  _taxonomy_access_user_node_access_update($affected_nids);


}



/**
 * Form access deletion building function
 */
function taxonomy_access_user_access_delete($form_state, $tauid) {
  $form = array();

  $account = user_load($tauid->uid);
  $term = taxonomy_get_term($tauid->tid);

  $form['term'] = array(
    '#type' => 'value',
    '#value' => $tauid->tid,
  );
  $form['tauid'] = array(
    '#type' => 'value',
    '#value' => $tauid->tauid,
  );


  return confirm_form($form,
    t('Are you sure you want to delete access of %title to %username ?', array(
      '%title' => $term->name,
      '%username' => $account->name)),
    isset($_GET['destination']) ? $_GET['destination'] : '/',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute access deletion
 */
function taxonomy_access_user_access_delete_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    module_load_include('inc', 'taxonomy_access_user');
    $terms = array(taxonomy_get_term($form_state['values']['term']));
    $terms = array_merge($terms, taxonomy_get_children($form_state['values']['term']));

    $affected_nids = _taxonomy_access_user_get_nodes_from_terms($terms);

    _taxonomy_access_user_node_access_update($affected_nids);

    db_query('DELETE FROM {term_access_user} WHERE tauid = %d', $form_state['values']['tauid']);
  }
}



/**
 * Form for taxonomy_access_user settings
 */
function taxonomy_access_user_settings() {
  $form = array();

  $vocabularies = taxonomy_get_vocabularies();

  $vids = array();
  foreach ($vocabularies as $vocabulary) {
    $vids[$vocabulary->vid] = check_plain($vocabulary->name);
  }

  $form['taxonomy_access_user_vids'] = array(
    '#type' => 'select',
    '#title' => 'Vocabulary',
    '#default_value' => variable_get('taxonomy_access_user_vids', array()),
    '#options' => $vids,
    '#multiple' => TRUE,
    '#description' => t('Vocabularies on which applies control access'),
  );

  $return = system_settings_form($form);

  return $return;
}
