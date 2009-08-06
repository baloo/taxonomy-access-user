<?php
// $Id$
/**
 * @file
 * This file add helper functions for taxonomy_access_user module
 */




function _taxonomy_access_user_access_tid_one_access($tid, $access, $account = NULL) {
  global $user;

  if (!isset($account)) {
    $account = $user;
  }

  $ops = array('grant_view', 'grant_update', 'grant_delete', 'grant_admin');

  if (array_search($access, $ops, TRUE) === FALSE) {
    return FALSE;
  }

  $result = db_result(db_query('SELECT {%s}(%d, %d)', $access, $tid, $account->uid));

  return ($result == '1');
}

function _taxonomy_access_user_access_tid($tid, $access, $account = NULL) {
  foreach ($access as $key => $value) {
    if ($value == 1 && !_taxonomy_access_user_access_tid_one_access($tid, $key, $account)) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Function that permit to check grant access over a term for a user
 */
function _taxonomy_access_user_access($terms, $access, $account = NULL) {
  if (user_access('administer taxonomy', $account)) {
    return TRUE;
  }

  if (is_array($terms)) {
    $tids = array_keys($terms);
  }
  else{
    $tids = array($terms->tid);
  }

  foreach ($tids as $tid) {
    if (!_taxonomy_access_user_access_tid($tid, $access, $account)) {
      return FALSE;
    }
  }

  return TRUE;
}


/**
 * Add acccess informations to a array of terms
 */
function _taxonomy_access_user_get_access($tids, $account = NULL) {
  global $user;

  if (!isset($account)) {
    $account = $user;
  }


  if (!is_array($tids)) {
    $tids = array($tids);
  }

  $result = db_query("SELECT
    tid,
    {grant_view}(tid, %d) as grant_view,
    {grant_update}(tid, %d) as grant_update,
    {grant_delete}(tid, %d) as grant_delete,
    {grant_admin}(tid, %d) as grant_admin
    FROM {term_data}
    WHERE
    tid IN (". db_placeholders($tids, 'text') .")",
      array_merge(array(
        $account->uid,
        $account->uid,
        $account->uid,
        $account->uid,
      ),
      $tids));
  $tids = array();
  while ($row = db_fetch_array($result)) {
    $tids[$row['tid']] = $row;
  }
  return $tids;
}


/**
 * Add informations directly over the parameter
 */
function _taxonomy_access_user_add_taxonomy_access(&$taxo) {
  if (is_array($taxo)) {
    $tids = array_keys($taxo);
  }
  if (is_object($taxo)) {
    $tids = array($taxo->tid);
  }

  $tids = _taxonomy_access_user_get_access($tids);
  foreach ($tids as $tid => $access) {
    foreach ($access as $name => $value) {
      if (is_array($taxo)) {
        $taxo[$tid]->$name = $value;
      }
      if (is_object($taxo)) {
        $taxo->$name = $value;
      }

    }
  }
}

/**
 * Get nodes from terms
 */
function _taxonomy_access_user_get_nodes_from_terms($terms) {
  $tids = array();
  foreach ($terms as $term) {
    $tids[] = $term->tid;
  }

  $result = db_query("SELECT nid FROM {term_node} WHERE tid IN('%s')", implode($tids, "','"));
  $nids = array();
  while ($node = db_fetch_object($result)) {
    $nids[] = $node->nid;
  }

  return $nids;
}

/**
 * Directly inspired from the taxonomy_access_module
 */
function _taxonomy_access_user_node_access_update($nids) {
  foreach ($nids as $node) {
    $loaded_node = node_load($node, NULL, TRUE);
    if (!empty($loaded_node)) {
      node_access_acquire_grants($loaded_node);
    }
  }
  return TRUE;
}

