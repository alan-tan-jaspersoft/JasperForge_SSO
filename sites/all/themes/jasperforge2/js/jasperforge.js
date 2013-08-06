(function ($) {

Drupal.behaviors.jasperforge2EqualHeights = {
  attach: function (context, settings) {
    if (jQuery().equalHeights) {
      $("#front-pre-row div.equal-heights div.content").equalHeights();
      $("#front-content-left div.equal-heights div.content").equalHeights();
      $("#front-content-right div.equal-heights div.content").equalHeights();
    }
  }
};

Drupal.behaviors.tocSizing = {
  attach: function (context, settings) {
      $(".toc").addClass('grid12-4');
    }
};

Drupal.behaviors.searchLabel = {
	attach: function (context, settings) {
		$('#main-menu .block-search-api-page input.form-submit').val('');
	}
};

/*
Drupal.behaviors.jasperforge2Tabs = {
  attach: function (context, settings) {
	$('.quicktabs_main').addClass('clearfix');
	$('.quicktabs-style-jasperforge2 li a').prepend('<div class="quicktabs-active-top"></div>').append('<div class="quicktabs-active-inner"><div class="left"><div class="inner"></div></div><div class="right"><div class="inner"></div></div></div>');
  }
};*/

Drupal.behaviors.jasperforge2_login = {
  attach: function (context, settings) {
	$('#modal-content').once('CToolsAttachBehaviors', function(e) {
		if(typeof Drupal.settings.social_login != 'undefined') {
			eval("window.oneall.api.plugins."+Drupal.settings.social_login.load.plugin_type+".build("+Drupal.settings.social_login.load.arguments+");");
		}
	});
  }
};

})(jQuery);