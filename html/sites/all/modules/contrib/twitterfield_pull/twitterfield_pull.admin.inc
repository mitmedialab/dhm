<?php

/**
 * @file
 * The Twitter Field Pull Admin.
 */

/**
 * Admin Form Twitter Field Pull.
 */
function twitterfield_pull_admin_settings() {

  // When displaying the page, make sure the list of fields is up-to-date.
  field_info_cache_clear();

  $form = array();

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['twitterfield_pull'] = array(
    '#type' => 'fieldset',
    '#title'       => t('Content Type'),
    '#group'       => 'settings',
  );

  // Save twitter pull frequency.
  $form['twitterfield_pull']['twitterfield_pull_update_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Minimum update period'),
    '#options' => array(
      60 => t('1 min'),
      300 => t('5 min'),
      900 => t('15 min'),
      1800 => t('30 min'),
      3600 => t('1 hour'),
      10800 => t('3 hours'),
      21600 => t('6 hours'),
      43200 => t('12 hours'),
      86400 => t('1 day'),
      259200 => t('3 day'),
      604800 => t('1 week'),
      2419200 => t('4 weeks'),
    ),
    '#default_value' => variable_get('twitterfield_pull_update_frequency', 3600),
    '#description' => t('This is the minimum time that must elapse before new tweets may be downloaded.'),
    '#weight' => 0,
  );

  // Gather node type information and prefix keys.
  $node_types = node_type_get_names();
  foreach ($node_types as $key => $value) {
    $node_types['twitterfield_pull_' . $key] = $value;
    unset($node_types[$key]);
  }

  $form['twitterfield_pull']['twitterfield_pull_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable field audit on content types.'),
    '#description' => t('Enable field audit on content types.'),
    '#default_value' => variable_get('twitterfield_pull_node_types', array()),
    '#options' => $node_types,
  );

  $types = variable_get('twitterfield_pull_node_types', array());

  foreach ($types as $key => $type) {

    if ($type) {

      $type = str_replace("twitterfield_pull_", "", $type);

      $form[$key] = array(
        '#type' => 'fieldset',
        '#title' => $node_types[$key],
        '#group'       => 'settings',
      );

      $fields = field_info_instances('node', $type);

      $selected_fields = twitterfield_pull_get_selected_field($type);

      $options = NULL;

      // All twitter widgets supported.
      $supported_widget_type = array('twitter_username', 'twitterfield');

      foreach ($fields as $field) {
        if (in_array($field['widget']['type'], $supported_widget_type)) {
          $field_info_field = field_info_field($field['field_name']);
          $options[$field['field_name']] = $field['label'];
        }
      }

      $form[$key]['twitterfield_pull_fields'][$key] = array(
        '#type' => 'checkboxes',
        '#title' => t('Enable twitter pull'),
        '#description' => t('Select which fields will be pull from the twitter.'),
        '#default_value' => $selected_fields,
        '#options' => $options,
      );

    };
  }

  $form['#submit'][] = 'twitterfield_pull_admin_settings_submit';

  return system_settings_form($form);

}

/**
 * Save track field settings.
 */
function twitterfield_pull_admin_settings_submit($form, &$form_state) {

  $job_enable = FALSE;

  // Delete old values.
  db_delete('twitterfield_pull')->execute();

  $node_types = $form_state['values']['twitterfield_pull_node_types'];
  $frequency = $form_state['values']['twitterfield_pull_update_frequency'];

  foreach ($node_types as $nt_key => $nt_value) {
    if ($nt_value) {

      if (isset($form_state['values'][$nt_key])) {
        $fields = $form_state['values'][$nt_key];
      }
      else {
        $fields = array();
      }

      foreach ($fields as $f_key => $f_value) {
        if ($f_value) {
          $job_enable = TRUE;
          db_insert('twitterfield_pull')
            ->fields(array(
            'field_name' => $f_key,
            'entity_type' => 'node',
            'bundle' => str_replace("twitterfield_pull_", "", $nt_key),
          ))
          ->execute();
        }
      }
    }
  }

  if ($job_enable) {
    // Launch the JOB.
    // _twitterfield_pull_set_job($frequency);
  }
  else {
    // Delete the JOB.
    // _twitterfield_pull_delete_job();
  }

  drupal_theme_rebuild();
}
