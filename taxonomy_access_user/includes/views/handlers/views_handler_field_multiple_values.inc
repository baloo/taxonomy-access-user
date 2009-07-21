<?php
// $Id$

/**
 * @file
 * A handler to provide proper displays for text instead of values.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_multiple_values extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['types'] = array('default' => '0|Default');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['types'] = array(
      '#type' => 'textarea',
      '#title' => t('Types'),
      '#default_value' => $this->options['types'],
      '#required' => TRUE,
      '#description' => t('Add a type per line with the following syntax : "%syntax"', array('%syntax' => '<id>|<name>')),
    );
  }

  function render($values) {
    $value = $values->{$this->field_alias};

    $lines = explode(chr(10), $this->options['types']);

    $types = array();
    foreach ($lines as $line) {
      $type = explode('|', $line);
      $types[$type[0]] = $type[1];
    }

    if (isset($types[$value]))
      return $types[$value];
    else
      return reset($types);

  }
}
