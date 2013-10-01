(function($) {
	$(function() {
	
		$('[id^="block-menu-menu-main-menu-"]').mmenu({
			'slidingSubmenus' : false,
			'configuration' : {
				'clone' : true
			}
		});
		
		$('#mobile-nav-trigger').click(function() {
			$('.mm-menu').trigger('toggle');
		});
		
	});
})(jQuery);