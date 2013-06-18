<?php

class views_plugin_display_services_views extends views_plugin_display {
  function get_style_type() { return 'services'; }
  function uses_breadcrumb() { return FALSE; }
  function has_path() {return TRUE; }

  function option_definition() {
    $options = parent::option_definition();
    // @fixme ensure that this style plugin is the default.
    $options['style_plugin']['default'] = 'services_views';
    $options['path'] = array('default' => '');

    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $categories['page'] = array(
      'title' => t('Services settings'),
      'column' => 'second',
      'build' => array(
        '#weight' => -10,
      ),
    );

    $path = strip_tags('/' . $this->get_option('path'));
    if (empty($path)) {
      $path = t('None');
    }

    $options['path'] = array(
      'category' => 'page',
      'title' => t('Path'),
      'value' => views_ui_truncate($path, 24),
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'path':
        $form['#title'] .= t('Path of the Services resource');
        $form['#help_topic'] = 'path';
        $form['path'] = array(
          '#type' => 'textfield',
          '#description' => t('This is path element that comes after endpoint path.'),
          '#default_value' => $this->get_option('path'),
        );
    }
  }

  function options_validate(&$form, &$form_state) {
    // It is very important to call the parent function here.
    parent::options_validate($form, $form_state);
    if ($form_state['section'] == 'path') {

      if (preg_match('/[^a-zA-Z0-9-_]+/', $form_state['values']['path'])) {
        form_error($form['path'], t('Use only alphanumeric characters, "-" and "_" for resource name'));
      }

      // Get current path and see if it was changed.
      $current_path = $this->get_option('path');
      if ($current_path != $form_state['values']['path']) {
        // We build resources from database to bypass cache.
        module_load_include('inc', 'services', 'includes/services.resource_build');
        $resources = _services_build_resources();

        if (array_key_exists($form_state['values']['path'], $resources)) {
          form_error($form['path'], t('Resource with this path already exists.'));
        }
      }
    }
  }

  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here.
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'path':
        $this->set_option('path', $form_state['values']['path']);
        break;
    }
  }

  function preview() {
    if (!empty($this->view->live_preview)) {
      return '<pre>' . var_export($this->view->render(), TRUE) . '</pre>';
    }
    return $this->view->render();
  }

  /**
   * Instead of going through the standard views_view.tpl.php, delegate this
   * to the style handler.
   */
  function render() {
    return $this->view->style_plugin->render($this->view->result);
  }
}
