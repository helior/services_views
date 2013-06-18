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
    return $output;
  }
}
