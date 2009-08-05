<?php
// $Id$
/**
 * @file
 * This file add helper functions for taxonomy_access_user module
 */

/**
 * Function that will delete custom functions in the sql
 */
function taxonomy_access_user_sql_uninstall() {
  global $db_type;

  $ops = array('view', 'update', 'delete', 'admin');
  switch ($db_type) {
  case 'mysql':
  case 'mysqli':
  case 'pgsql':
    $sql = 'DROP FUNCTION IF EXISTS {grant_%op};';
    break;
  }

  foreach ($ops as $op) {
    db_query(str_replace('%op', $op, $sql));
  }
}

/**
 * Function that will add custom functions in the sql
 */
function taxonomy_access_user_sql_install() {
  global $db_type, $active_db;

  $ops = array('view', 'update', 'delete', 'admin');
  $grant = array();

  switch ($db_type) {
  case 'mysql':
  case 'mysqli':
    $grant[] = <<<EOF
CREATE FUNCTION {grant_%op} (atid INT, auid INT)
RETURNS INT
DETERMINISTIC
BEGIN
  DECLARE my%op INT DEFAULT 0;
  DECLARE curtid INT;
  SET curtid = atid;

  main_loop: LOOP
    SELECT grant_%op INTO my%op FROM {term_access_user} tau WHERE tau.tid = curtid AND tau.uid = auid LIMIT 1;
    IF my%op = 1 THEN
      LEAVE main_loop;
    END IF;
    IF my%op = -1 THEN
      LEAVE main_loop;
    END IF;
    SELECT parent INTO curtid FROM {term_hierarchy} WHERE tid = curtid LIMIT 1;
    IF curtid = 0 THEN
      LEAVE main_loop;
    END IF;
  END LOOP main_loop;
  RETURN my%op;
END
EOF;
    break;
  case 'pgsql':
    $grant[] = <<<EOF
CREATE LANGUAGE plpgsql;
CREATE FUNCTION {grant_%op}(atid INT, auid INT) RETURNS INT AS $$
DECLARE
  my%op integer := 0;
  curtid integer;
BEGIN
  curtid := atid;
  LOOP
    SELECT INTO my%op grant_%op FROM {term_access_user} tau WHERE tau.tid = curtid AND tau.uid = auid;
    IF my%op = 1 THEN
      EXIT;
    END IF;
    IF my%op = -1 THEN
      EXIT;
    END IF;
    SELECT INTO curtid parent FROM {term_hierarchy} WHERE tid = curtid;
    IF curtid = 0 THEN
      EXIT;
    END IF;
  END LOOP;
  RETURN my%op;
END;
$$ LANGUAGE plpgsql;
EOF;
    break;
  default:
    return NULL;
  }
  $grants = array();

  foreach ($ops as $op) {
    foreach ($grant as $query) {
      $grants[$op][] = str_replace('%op', $op, $query);
    }
  }

  unset($grant);

  foreach ($grants as $grant) {
    foreach ($grant as $query) {
      db_query($query);
    }
  }
}


function _taxonomy_access_user_access_tid_one_access($tid, $access, $account = NULL) {
  global $user;

  if ($account === NULL) {
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

  if ($account === NULL) {
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

