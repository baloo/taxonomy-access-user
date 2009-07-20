<?php
// $Id$

/**
 * @file
 * A handler to provide proper displays for ternary values.
 *
 * Allows for display of true/default/false, yes/default/no.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_ternary extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => 'yes-default-no');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Output format'),
      '#options' => array(
        'yes-default-no' => t('Yes/Default/No'),
        'true-default-false' => t('True/Default/False'),
      ),
      '#default_value' => $this->options['type'],
    );
  }

  function render($values) {
    $value = $values->{$this->field_alias};

    $return = array(
      'yes-default-no' => array(
        -1 => t('No'),
        0 => t('Default'),
        1 => t('Yes'),
      ),
      'true-default-false' => array(
        -1 => t('No'),
        0 => t('Default'),
        1 => t('Yes'),
      ),
    );

    if (array_search($value, array(-1, 0, 1)) === FALSE)
      $value = 0;

    switch ($this->options['type']) {
      case 'yes-default-no':
      default:
        return $return['yes-default-no'][$value];
      case 'true-default-false':
        return $return['true-default-false'][$value];
    }
  }
}
