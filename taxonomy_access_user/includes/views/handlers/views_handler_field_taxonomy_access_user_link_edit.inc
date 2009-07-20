<?php
// $Id$
/**
 * @file
 * This file defines new views field specific to this module : Edit link
 */

/**
 * Field handler to present a link domain admin delegation right delete.
 */
class views_handler_field_taxonomy_access_user_link_edit extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['tauid'] = 'tauid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }


  function render($values) {
    global $user;

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');

    return l(
      $text,
      "taxonomy/access/edit/{$values->{$this->aliases['tauid']}}",
      array(
        'query' => drupal_get_destination(),
        'attributes' => array(
          'class' => 'popups-form-reload',
        ),
      )
    );
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

}

