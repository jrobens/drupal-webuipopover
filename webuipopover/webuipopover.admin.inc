<?php

/**
 * @file
 * Beautytips settings form and submit action
 */

/**
 * Menu callback - webuipopover admin settings form
 */
function webuipopover_admin() {
  
  $form['webuipopover_always_add'] = array(
    '#title' => t('Add webuipopover js to every page'),
    '#description' => t('This allows you to give the class \'webuipopover\' to any element on a page and the title attribute will popup as a beautytip.<br /> i.e. <i> &lt;p class="webuipopover" title="type the text you want webuipopover to display here"&gt .....&lt/p&gt</i>'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('webuipopover_always_add', 0),
    '#weight' => -1,
  );
  $selectors = variable_get('webuipopover_added_selectors_array', '');
  $form['webuipopover_added_selectors_string'] = array(
    '#title' => t('Add webuipopover to the following selectors'),
    '#description' => t("Separate selectors with a comma.  webuipopover will be added to each of these on every page.  The element's title attribute will be the text used. (OPTIONAL)"),
    '#type' => 'textfield',
    '#default_value' => is_array($selectors) ? implode(", ", $selectors) : '',
    '#weight' => -1,
  );
  $form['webuipopover_ltr'] = array(
    '#title' => t('Support Left to Right display'),
    '#description' => t('Only check this if this is an ltr site.  This adds css to support it.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('webuipopover_ltr', 0),
  );
  $form['webuipopover_default_styles'] = array(
    '#type' => 'fieldset',
    '#title' => 'Styling Options',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $styles = webuipopover_get_styles(TRUE);
  if (count($styles)) {
    unset($styles['default']);
    foreach ($styles as $name => $style) {
      $bt_style_options[$name]['cssSelect'] = '#edit-webuipopover-default-style-' . str_replace('_', '-', $name);
      $bt_style_options[$name]['text'] = 'Aenean risus purus, pharetra in, blandit quis, gravida a, turpis.  Aenean risus purus, pharetra in, blandit quis, gravida a, turpis.  Aenean risus purus, pharetra in, blandit quis, gravida a, turpis.';
      $bt_style_options[$name]['width'] = isset($style['width']) ? $style['width'] : '300px';
      $bt_style_options[$name]['style'] = $name;
      $style_options[$name] = $name;
    }
  }
  $bt_style_options['default_hover'] = array(
    'cssSelect' => '#webuipopover-site-wide-popup',
    'text' => 'Sed justo nibh, ultrices ut gravida et, laoreet et elit. Nullam consequat lacus et dui dignissim venenatis. Curabitur quis urna eget mi interdum viverra quis eu enim. Ut sit amet nunc augue. Morbi fermentum ultricies velit sed aliquam. Etiam dui tortor, auctor sed tempus ac, auctor sed sapien.',
    'positions' => array('right'),
  );
  // TODO: Determine what to do if default style has been removed.
  $form['webuipopover_default_styles']['webuipopover_default_style'] = array(
    '#type' => 'radios',
    '#title' => t('Choose a default style'),
    '#description' => t('Mouse over the radio buttons to see a preview.'),
    '#prefix' => '<div id="beauty-default-styles">',
    '#suffix' => '</div>',
    '#options' => $style_options,
    '#default_value' => variable_get('webuipopover_default_style', 'plain'),
  );
  $style_options = array(
    'fill' => t('background color (string - html color)'), 
    'strokeWidth' => t('width of border (integer)'),
    'strokeStyle' => t('color of border (string - html color)'),
    'width' => t('width of popup (number in px)'), 
    'padding' => t('space between content and border (number in px)'), 
    'cornerRadius' => t('Controls roundness of corners (integer)'),
    'spikeGirth' => t('thickness of spike (integer)'),
    'spikeLength' => t('length of spike (integer)'),
    'shadowBlur' => t('Size of popup shadow (integer)'),
    'shadowColor' => t('Color of popup shadow (string - html color)'),
  );
  $form['webuipopover_default_styles']['custom_styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom Style Options'),
    '#description' => t('<b>Set a custom style.</b><br /> Note: These will use the default style that is selected as a base <br /> but will overide elements such as background color, border color etc.   <br /><i>Leave these empty unless you know what you are doing.</i><div id="webuipopover-popup-changes"><div id="beauty-click-text"><p>Double Click here to view popup with custom changes</p></div></div>'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('class' => array('bt-custom-styles')),
    '#prefix' => '<div id="webuipopover-site-wide-popup"><div id="beauty-text"><p>Hover here to see the current site-wide webuipopover</p></div></div>',
  );
  $custom_style = variable_get('webuipopover_custom_style', NULL);
  foreach ($style_options as $option => $description) {
    $form['webuipopover_default_styles']['custom_styles']['bt-options-box-' . $option] = array(
      '#title' => $option,
      '#description' => $description,
      '#type' => 'textfield',
      '#default_value' => isset($custom_style[$option]) ? $custom_style[$option] : '',
    );
  }
  $form['webuipopover_default_styles']['custom_styles']['bt-options-box-shadow'] = array(
    '#title' => 'shadow',
    '#description' => t('Whether or not the popup has a shadow'),
    '#type' => 'radios',
    '#options' => array('default' => t('Default'), 'shadow' => t('Shadow On'), 'no_shadow' => t('Shadow Off')),
    '#attributes' => array('class' => array('webuipopover-options-shadow')),
    '#default_value' => isset($custom_style['shadow']) ? $custom_style['shadow'] : 'default',
  );
  $form['webuipopover_default_styles']['custom_styles']['bt-options-cssClass'] = array(
    '#title' => 'cssClass',
    '#description' => t('The class that will be applied to the box wrapper div (of the TIP)'),
    '#type' => 'textfield',
    '#default_value' => isset($custom_style['cssClass']) ? $custom_style['cssClass'] : '',
  );
  $css_style_options = array('color', 'fontFamily', 'fontWeight', 'fontSize');
  $form['webuipopover_default_styles']['custom_styles']['css-styles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Font Styling'),
    '#description' => t('Enter css options for changing the font style'),
    '#attributes' => array('class' => array('webuipopover-css-styling')),
    '#collapsible' => FALSE,
  );
  foreach ($css_style_options as $option) {
    $form['webuipopover_default_styles']['custom_styles']['css-styles']['bt-options-css-' . $option] = array(
      '#title' => $option,
      '#type' => 'textfield',
      '#default_value' => isset($custom_style['cssStyles'][$option]) ? $custom_style['cssStyles'][$option] : '',
    );
  }

  $form['#submit'][] = 'webuipopover_admin_submit';
  
  $path = drupal_get_path('module', 'webuipopover');
  drupal_add_js($path . '/other_libs/colorpicker/js/colorpicker.js');
  drupal_add_css($path . '/other_libs/colorpicker/css/colorpicker.css');
  drupal_add_css($path .'/css/webuipopover.css');
  webuipopover_add_webuipopover($bt_style_options);
  drupal_add_js($path . '/js/bt_admin_page.js');

  if (module_exists('webuipopover_ui')) {
    webuipopover_ui_admin_settings($form);
  }

  return system_settings_form($form);
}

/**
 * Validation for webuipopover settings form
 */
function webuipopover_admin_validate($form, &$form_state) {
  $values = $form_state['values'];
  $integer_fields = array('bt-options-box-strokeWidth', 'bt-options-box-cornerRadius', 'bt-options-box-spikeGirth', 'bt-options-box-spikeLength', 'bt-options-box-shadowBlur'); 
  $pixel_fields = array('bt-options-box-width', 'bt-options-box-padding', 'bt-options-css-fontSize');
  $color_fields = array('bt-options-box-fill', 'bt-options-css-color', 'bt-options-box-shadowColor'); 
  $sanitize_strings = array('bt-options-cssClass', 'bt-options-css-fontFamily', 'bt-options-css-fontWeight');

  foreach ($integer_fields as $name) {
    if ($values[$name]) {
      if (!is_numeric($values[$name])) {
        form_set_error($name, t('You need to enter a numeric value for <em>@name</em>', array('@name' => str_replace(array('bt-options-box-', 'bt-options-css-'), '', $name))));
      }
      else {
        $form_state['values'][$name] = round($values[$name]);  
      }
    }
  }

  foreach ($pixel_fields as $name) {
    if ($values[$name]) {
      $unit = substr($values[$name], -2, 2);
      $value = str_replace(array('px', ' ', 'em'), '', $values[$name]);
      if (!is_numeric($value) || (!$value && $value != 0) || !in_array($unit, array('px', 'em'))) {
        form_set_error($name, t('You need to enter a numeric value for <em>@name</em>, followed by <em>px</em>', array('@name' => str_replace(array('bt-options-box-', 'bt-options-css-'), '', $name))));
      }
    }
  }
}

/**
 * Submit function for webuipopover settings form
 */
function webuipopover_admin_submit($form, &$form_state) {
  $values = $form_state['values'];
  if (count($values)) {
    $custom_style = array();
    $css_style = array();
    foreach ($values as $option => $value) {
      if (strpos($option, 'bt-options-box-') === 0) {
        $option = str_replace('bt-options-box-', '', $option);
        $custom_style[$option] = $value;
      }
      else if ($option == 'bt-options-cssClass') {
        $option = str_replace('bt-options-', '', $option);
        $custom_style[$option] = $value;
      }
      else if (strpos($option, 'bt-options-css-') === 0) {
        $option = str_replace('bt-options-css-', '', $option);
        if ($value) {
          $css_style[$option] = $value;
        }
      }
    }

    // Store the defaults - they will be passed to javascript
    $style = webuipopover_get_style($values['webuipopover_default_style']);
    if (count($custom_style)) {
      foreach ($custom_style as $option => $value) {
        if ($option == 'shadow') {
          if ($value != 'default') {
            $style['shadow'] = $value == 'shadow' ? TRUE : FALSE;
          }
        }
        else if (!empty($value) || $value == '0') {
          $style[$option] = is_numeric($value) ? (int) $value : (string) $value;
        }
      }
    }
    if (count($css_style)) {
      foreach ($css_style as $option => $value) {
        if (!empty($value)) {
          $style['cssStyles'][$option] = (string) $value;
        }
      }
      if (!empty($css_style)) {
        $custom_style['cssStyles'] = $css_style;
      }
    }
    variable_set('webuipopover_defaults', $style);
    variable_set('webuipopover_custom_style', $custom_style);
    variable_set('webuipopover_default_style', $values['webuipopover_default_style']);
    cache_clear_all('webuipopover:webuipopover-styles', 'cache');

    // Store array of selectors that bt will be added to on every page
    $selectors = explode(",", $values['webuipopover_added_selectors_string']);
    $test = serialize($selectors);
    if (count($selectors)) {
      foreach ($selectors as $key => $selector) {
        $selectors[$key] = trim($selector);
      }
    }
    variable_set('webuipopover_added_selectors_array', $selectors);
  }
  if (module_exists('webuipopover_ui')) {
    webuipopover_ui_admin_submit($form, $form_state);
  }
}

