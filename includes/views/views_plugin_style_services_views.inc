<?php

class views_plugin_style_services_views extends views_plugin_style {
  /**
   * Render the grouping sets.
   *
   * Plugins may override this method if they wish some other way of handling
   * grouping.
   *
   * @param $sets
   *   Array containing the grouping sets to render.
   * @param $level
   *   Integer indicating the hierarchical level of the grouping.
   *
   * @return string
   *   Rendered output of given grouping sets.
   */
  function render_grouping_sets($sets, $level = 0) {
    $output = '';
    foreach ($sets as $set) {
      $row = reset($set['rows']);
      // Render as a grouping set.
      // @todo do this, too.
      if (is_array($row) && isset($row['group'])) {
        // $output .= theme(views_theme_functions('views_view_grouping', $this->view, $this->display),
        //   array(
        //     'view' => $this->view,
        //     'grouping' => $this->options['grouping'][$level],
        //     'grouping_level' => $level,
        //     'rows' => $set['rows'],
        //     'title' => $set['group'])
        // );
      }
      // Render as a record set.
      else {
        if ($this->uses_row_plugin()) {
          $output = $this->render_fields($set['rows']);
        }
      }
    }
    unset($this->view->row_index);

    array_walk($output, array($this, '_curate_row_array_walk_callback'));

    return $output;
  }

  /**
   * This method is an array_walk callback that does two things:
   *   - Re-keys each field to use the configured label instead of the internal
   *     field id.
   *   - Removes fields that want to be excluded from the final results.
   */
  function _curate_row_array_walk_callback(&$item, $key) {
    $new_item = array();

    foreach ($item as $id => $value) {
      // Skip excluded fields.
      if (empty($this->view->field[$id]->options['exclude'])) {
        $label = $this->view->field[$id]->label();
        $label = !empty($label) ? $label : $id;
        $new_item[$label] = $value;
      }
    }

    $item = $new_item;
  }
}
