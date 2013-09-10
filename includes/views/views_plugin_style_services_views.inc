<?php

class views_plugin_style_services_views extends views_plugin_style {

  function option_definition() {
    $options = parent::option_definition();
    $options['keys'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#markup' => '<div class="error messages">' . t('You need at least one field before you can configure your response object keys.') . '</div>',
      );
      return;
    }

    foreach ($this->display->handler->get_field_labels() as $id => $label) {
      $form['keys'][$id] = array(
        '#type' => 'textfield',
        '#title' => $label,
        '#default_value' => isset($this->options['keys'][$id]) ? $this->options['keys'][$id] : '',
      );
    }

    return $form;
  }

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
        $label = empty($this->options['keys'][$id]) ? $id : $this->options['keys'][$id];
        $new_item[$label] = $value;
      }
    }

    $item = $new_item;
  }
}
