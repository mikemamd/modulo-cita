/**
 * Remove the posibility to perform cross domaincalls in the site
 */
(function($) {
  Drupal.behaviors.cross = {
    attach : function(context) {

      $.ajaxPrefilter( function(s){
        if (s.crossDomain) {
          s.contents.javascript = false;
        }
      });

    }
  };
})(jQuery);
