jQuery(function() {
	jQuery('#sidebar-first').resizable({
		handles: 'e',
		minWidth:150,
		maxWidth:400,
		resize: function(e,ui) {
			var max = Math.min(1180,jQuery(window).width()-20);
			jQuery('#content-group,#content').css('width',max-ui.size.width);
			jQuery('#sidebar-first').css('height',jQuery('#block-book_explorer-book_explorer').height());
		}
	});
	
	jQuery(window).resize(function() {
		var width = jQuery(this).width();
		if(width < 1200) {
			var width = width - jQuery('#sidebar-first').width() - 20;
			jQuery('#content-group,#content').css('width',width);
		}
	});
});
