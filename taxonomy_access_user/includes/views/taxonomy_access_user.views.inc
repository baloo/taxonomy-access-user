<?php
// $Id$
/**
 * @file
 * this file defines the fields for the term_access_user table
 */

function taxonomy_access_user_views_data() {
  $data[TAXONOMY_ACCESS_USER_TABLE]['table']['group']  = t('Term access');

  // Advertise this table as a possible base table

  // For other base tables, explain how we join
  $data[TAXONOMY_ACCESS_USER_TABLE]['table']['join'] = array(
    // Directly links to node table.
    'term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
    // Directly links to user table.
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // uid field
  $data[TAXONOMY_ACCESS_USER_TABLE]['uid'] = array(
    'title' => t('User'),
    'help' => t('Link a term to the user who have access to it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Term access'),
    ),
  );

  // nid field
  $data[TAXONOMY_ACCESS_USER_TABLE]['tid'] = array(
    'title' => t('Term'),
    'help' => t('Link a term access to the term.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'term_data',
      'field' => 'tid',
      'label' => t('Term'),
    ),
  );

  // grant_view field
  $data[TAXONOMY_ACCESS_USER_TABLE]['grant_view'] = array(
    'title' => t('Grant view'),
    'help' => t('View nodes associated to the term'),
    'field' => array(
      'handler' => 'views_handler_field_ternary',
      'click sortable' => TRUE,
    ),
  );
  // grant_update field
  $data[TAXONOMY_ACCESS_USER_TABLE]['grant_update'] = array(
    'title' => t('Grant update'),
    'help' => t('Update nodes associated to the term'),
    'field' => array(
      'handler' => 'views_handler_field_ternary',
      'click sortable' => TRUE,
    ),
  );
  // grant_delete field
  $data[TAXONOMY_ACCESS_USER_TABLE]['grant_delete'] = array(
    'title' => t('Grant delete'),
    'help' => t('Delete nodes associated to the term'),
    'field' => array(
      'handler' => 'views_handler_field_ternary',
      'click sortable' => TRUE,
    ),
  );
  // grant_admin field
  $data[TAXONOMY_ACCESS_USER_TABLE]['grant_admin'] = array(
    'title' => t('Grant admin'),
    'help' => t('Admin the term'),
    'field' => array(
      'handler' => 'views_handler_field_ternary',
      'click sortable' => TRUE,
    ),
  );
  // edit_link field
  $data[TAXONOMY_ACCESS_USER_TABLE]['edit_link'] = array(
    'title' => t('Edit link'),
    'help' => t('Edit term access'),
    'field' => array(
      'handler' => 'views_handler_field_taxonomy_access_user_link_edit',
    ),
  );
  // delete_link field
  $data[TAXONOMY_ACCESS_USER_TABLE]['delete_link'] = array(
    'title' => t('Delete link'),
    'help' => t('Delete term access'),
    'field' => array(
      'handler' => 'views_handler_field_taxonomy_access_user_link_delete',
    ),
  );

  return $data;
}

