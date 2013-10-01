(function($) {
	$(function() {
		
		var sections = $('.edition-container[data-menu-item]');
		var menu = $('<ul class="section-menu" />');
		var menu_wrapper = $('<div class="container-wrapper"><div class="container section-menu-wrapper" /></div>');
		var menu_container = $('#section-menu-wrapper-container');
		// get the sections / container wrapper and build and insert the new menu
		$('.edition-container[data-menu-item]').each(function() {
			var menu_item = $(this).data('menu-item');
			$(this).addClass(menu_item.toLowerCase().replace(/ /g,'-').replace(/\?/g,''));
			menu.append($('<li><a id="'+menu_item.toLowerCase().replace(/ /g,'-').replace(/\?/g,'')+'">'+menu_item+'</li>'));
		});
		
		menu_wrapper.find('.container').append(menu);
		menu_container.append(menu_wrapper);

		menu_wrapper.smint();
		
		menu_container.css('position','relative');
	});
})(jQuery);