<?php

/**
 * @file
 * Contains the flagged content filter handler.
 */

/**
 * Handler to filter the twitterfield.
 *
 * @ingroup views
 */
class twitterfield_pull_handler_filter_field extends views_handler_filter_in_operator {
  function get_value_options() {
    $node_types = variable_get('twitterfield_pull_node_types', array());
    $selected_fields = array();
    foreach ($node_types as $key => $node_type) {
      if ($node_type) {
        $type = str_replace("twitterfield_pull_", "", $node_type);
        $selected_fields = array_merge((array)$selected_fields, (array)twitterfield_pull_get_selected_field($type));
      }
    }
    $this->value_options = $selected_fields;
  }
}
