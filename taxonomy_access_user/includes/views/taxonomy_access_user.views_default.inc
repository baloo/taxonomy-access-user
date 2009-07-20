<?php
// $Id$

/**
 * @file
 * This file defines default views for taxonomy_access_user
 */

/**
 * Implementation of hook_views_default_views().
 */
function taxonomy_access_user_views_default_views() {
  $views = array();

  /*
   * View 'taxonomy_access_users'
   */
  $view = new view;
  $view->name = 'taxonomy_access_users';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'users';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'uid' => array(
      'label' => 'Term access',
      'required' => 1,
      'id' => 'uid',
      'table' => 'term_access_user',
      'field' => 'uid',
      'relationship' => 'none',
    ),
    'tid' => array(
      'label' => 'Term',
      'required' => 1,
      'id' => 'tid',
      'table' => 'term_access_user',
      'field' => 'tid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'name' => array(
      'label' => 'Name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'link_to_user' => 1,
      'overwrite_anonymous' => 0,
      'anonymous_text' => '',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'grant_view' => array(
      'label' => 'View',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'type' => 'yes-default-no',
      'not' => 0,
      'exclude' => 0,
      'id' => 'grant_view',
      'table' => 'term_access_user',
      'field' => 'grant_view',
      'relationship' => 'none',
    ),
    'grant_update' => array(
      'label' => 'Update',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'type' => 'yes-default-no',
      'not' => 0,
      'exclude' => 0,
      'id' => 'grant_update',
      'table' => 'term_access_user',
      'field' => 'grant_update',
      'relationship' => 'none',
    ),
    'grant_delete' => array(
      'label' => 'Delete',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'type' => 'yes-default-no',
      'not' => 0,
      'exclude' => 0,
      'id' => 'grant_delete',
      'table' => 'term_access_user',
      'field' => 'grant_delete',
      'relationship' => 'none',
    ),
    'grant_admin' => array(
      'label' => 'Admin',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'type' => 'yes-default-no',
      'not' => 0,
      'exclude' => 0,
      'id' => 'grant_admin',
      'table' => 'term_access_user',
      'field' => 'grant_admin',
      'relationship' => 'none',
    ),
    'edit_link' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'edit_link',
      'table' => 'term_access_user',
      'field' => 'edit_link',
      'relationship' => 'none',
      'text' => '',
    ),
    'delete_link' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'delete_link',
      'table' => 'term_access_user',
      'field' => 'delete_link',
      'relationship' => 'none',
      'text' => '',
    ),
  ));
  $handler->override_option('arguments', array(
    'tid' => array(
      'default_action' => 'empty',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'tid',
      'table' => 'term_data',
      'field' => 'tid',
      'relationship' => 'tid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'node_domain' => 0,
        'node_mail' => 0,
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '25' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 1);
  $handler->override_option('header', '');
  $handler->override_option('title', '');
  $handler->override_option('empty_format', '1');
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'name' => 'name',
    ),
    'info' => array(
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $views[$view->name] = $view;

  return $views;
}
