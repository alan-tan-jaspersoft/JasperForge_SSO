(function ($) {


Drupal.behaviors.eventSlide = {
	attach: function (context, settings) {
		$('.view-id-events.view-display-id-block_2 .view-content').cycle({
			fx: 'scrollUp' 
		});
	}
};

})(jQuery);