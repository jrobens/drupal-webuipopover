
/**
 * Defines the default beautytip and adds them to the content on the page
 */ 
(function ($) {
  Drupal.behaviors.webuipopover = {
    attach: function(context, settings) {
      // Fix for drupal attach behaviors in case the plugin is not attached.
      /*
      if (typeof(jQuery.bt) == 'undefined' && jQuery.bt == null){
        return;
      }
      jQuery.bt.options.closeWhenOthersOpen = true;
       */

      var webuipopover = Drupal.settings.webuipopover;

      // On ajax page loads, if the same settings are added
      // to the page, then it can mess up the settings.
      // If this is fixed in Drupal, then this can be removed.
      function fixArray(originalArray, count) {
        for (var key in originalArray) {
          if (key == 'cssStyles') {
            originalArray[key] = fixArray(originalArray[key], count);
          }
          else if (originalArray[key].length == count) {
            originalArray[key] = originalArray[key][0];
          }
          else {
            length = Math.round(originalArray[key].length / count);
            originalArray[key] = originalArray[key].slice(0, length);
          }
        }
        return originalArray;
      }

      // Add the the tooltips to the page
      for (var key in webuipopover) {
        // If there's an ajax page load on a page, drupal can add these
        // settings more than once and it adds the settings instead of replaces.
        // We have to fix these here.
        if (typeof(Drupal.settings.webuipopover[key]['cssSelect']) == 'object') {
          var count = Drupal.settings.webuipopover[key]['cssSelect'].length;
          webuipopover[key] = fixArray(webuipopover[key], count);
          Drupal.settings.webuipopover[key] = webuipopover[key];
        }
        // Build array of options that were passed to beautytips_add_beautyips
        var btOptions = [];
        if (webuipopover[key]['list']) {
          for ( var k = 0; k < webuipopover[key]['list'].length; k++) {
            btOptions[webuipopover[key]['list'][k]] = webuipopover[key][webuipopover[key]['list'][k]];
          }
        }
        if (webuipopover[key]['cssSelect']) {
          if (webuipopover[key]['animate']) {
            btOptions = webuipopoverAddAnimations(webuipopover[key]['animate'], btOptions);
          }
          // Run any java script that needs to be run when the page loads
          if (webuipopover[key]['contentSelector'] && webuipopover[key]['preEval']) {
            $(webuipopover[key]['cssSelect']).each(function() {
              if (!webuipopoverProcessed(this, false)) {
                eval(webuipopover[key]['contentSelector']);
              }
            });
          }
          if (webuipopover[key]['text']) {
            $(webuipopover[key]['cssSelect']).each(function() {
              if (!webuipopoverProcessed(this)) {
               // $(this).bt(beautytips[key]['text'], btOptions);
                $(this).webuiPopover();
              }
            });
          }
          else if (webuipopover[key]['ajaxPath']) {
            $(webuipopover[key]['cssSelect']).each(function() {
              if (!webuipopoverProcessed(this)) {
                if (webuipopover[key]['ajaxDisableLink']) {
                  $(this).click(function(event) {
                    event.preventDefault();
                  });
                }
               // $(this).bt(btOptions);
                $(this).webuiPopover();
              }
            });
          }
          else { 
            $(webuipopover[key]['cssSelect']).each(function() {
              if (!webuipopoverProcessed(this))
              {
                $(this).webuiPopover({trigger:'hover'});
               // $(this).bt(btOptions);
              }
            });
          }
        }
        btOptions.length = 0;
      }
    }
  };

  /**
   * Determine if an element has already been processed.
   */
  function webuipopoverProcessed(element, addClass) {
    // Tooltips module, so reverse dependency.
    var tips =  $('.calendar_tooltips');
    tips.addClass('webui-popover-content');
    tips.css('display', "");

    if ($(element).hasClass('webuipopover-module-processed')) {
      return true;
    }
    if (addClass != false) {
      $(element).addClass('webuipopover-module-processed');
    }
    return false;
  }

  function webuipopoverAddAnimations(animations, btOptions) {
    switch (animations['on']) {
      case 'none':
        break;
      case 'fadeIn':
        btOptions['showTip'] = function(box) {
          $(box).fadeIn(500);
        };
        break;
      case 'slideIn':
        break;
    }
    switch (animations['off']) {
      case 'none':
        break;
      case 'fadeOut':
        btOptions['hideTip'] = function(box, callback) { 
          $(box).animate({opacity: 0}, 500, callback);
        };
        break;
      case 'slideOut':
        btOptions['hideTip'] = function(box, callback) {
          var width = $("body").width();
          $(box).animate({"left": "+=" + width + "px"}, "slow", callback);
        };
        break;
    }
    return btOptions;
  }
})(jQuery);
