
/**
 * jQuery to show on beautytips admin settings page
 */
Drupal.behaviors.webuipopoverAdmin = {
  attach: function(context, settings) {
    if (!jQuery("#edit-webuipopover-always-add").attr("checked")) {
      // Disable input and hide its description.
      jQuery("#edit-webuipopover-added-selectors").attr("disabled","disabled");
      jQuery("#edit-webuipopover-added-selectors-wrapper").hide(0);
    }
    jQuery("#edit-webuipopover-always-add").bind("click", function() {
      if (jQuery("#edit-webuipopover-always-add").attr("checked")) {
        // Auto-alias unchecked; enable input.
        jQuery("#edit-webuipopover-added-selectors").removeAttr("disabled");
        jQuery("#edit-webuipopover-added-selectors-wrapper").slideDown('fast');
      }
      else {
        // Auto-alias checked; disable input.
        jQuery("#edit-webuipopover-added-selectors").attr("disabled","disabled");
        jQuery("#edit-webuipopover-added-selectors-wrapper").slideUp('fast');
      }
    });

    // Add the color picker to certain textfields
    jQuery('#edit-bt-options-box-fill, #edit-bt-options-box-strokestyle, #edit-bt-options-box-shadowcolor, #edit-bt-options-css-color').ColorPicker({
      onSubmit: function(hsb, hex, rgb, el) {
        jQuery(el).val('#' + hex);
        jQuery(el).ColorPickerHide();
      },
      onBeforeShow: function () {
        value = this.value.replace("#", "");
        jQuery(this).ColorPickerSetColor(value);
      }
    })
    .bind('keyup', function(){
      jQuery(this).ColorPickerSetColor(this.value);
    });


    var popupText = "Sed justo nibh, ultrices ut gravida et, laoreet et elit. Nullam consequat lacus et dui dignissim venenatis. Curabitur quis urna eget mi interdum viverra quis eu enim. Ut sit amet nunc augue. Morbi ferm entum ultricies velit sed aliquam. Etiam dui tortor, auctor sed tempus ac, auctor sed sapien.";
    themeSettings = webuipopoverGetThemeSettings();
    currentTheme = jQuery("input[name='webuipopover_default_style']:checked").val();
    jQuery("#beauty-default-styles input").click(function() {
      currentTheme = jQuery("input[name='webuipopover_default_style']:checked").val();
    });

    jQuery("#webuipopover-popup-changes").click( function() {
      options = webuipopoverSetupDefaultOptions(themeSettings[currentTheme]);
      // General options
      jQuery("#webuipopover-site-wide-popup").next('fieldset').find('.fieldset-wrapper').children('.form-item:not(.webuipopover-css-styling)').each( function() {
        var name = jQuery(this).find('input').attr('name'); 
        var optionName = name.replace("bt-options-box-", "");
        var newValue = jQuery(this).find('input').val();
        if (optionName == 'shadow') {
          newValue = jQuery(".webuipopover-options-shadow input[@name='bt-options-box-shadow']:checked").val();
          newValue = newValue == 'default' ? null : (newValue == 'shadow' ? true : false);
        }
        if (newValue || newValue === false) {
          if (optionName == 'cornerRadius') {
            newValue = Number(newValue);
          }
          options[optionName] = newValue;
        }
      });
      // css options
      jQuery(".webuipopover-css-styling").children('.form-item').each( function() {
        var newValue = jQuery(this).find('input').val();
        var name = jQuery(this).find('input').attr('name'); 
        var optionName = name.replace("bt-options-css-", "");
        if (!options['cssStyles']) {
          options['cssStyles'] = new Array();
        }
        if (newValue || newValue === false) {
          options['cssStyles'][optionName] = newValue;
        }
      });
      jQuery(this).bt(popupText, options);
    });
  }
}

function webuipopoverSetupDefaultOptions(themeSettings) {
  var options = new Array();

  for (var key in themeSettings) {
    if (key == 'cssStyles') {
      options['cssStyles'] = new Array();
      for (var option in themeSettings['cssStyles']) {
        options['cssStyles'][option] = themeSettings['cssStyles'][option];
      }
    }
    else {
      options[key] = themeSettings[key];
    }
  }
  options['positions'] = 'right';
  options['trigger'] = ['dblclick', 'click'];

  return options;
}

function webuipopoverGetThemeSettings() {
  themeSettings = Drupal.settings.webuipopover;
  return themeSettings;
}
