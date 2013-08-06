<?php
	include "includes/header.tpl.php";
	drupal_add_library('jquery_plugin', 'cycle');
	drupal_add_js(drupal_get_path('theme','jasperforge2').'/js/jasperforge2-front.js');
?>
	<h1 style="display:none">Jaspersoft Community</h1>
	<div id="front-pre-wrapper" class="clearfix">
		<?php print render($page['front_pre_row']); ?>
	</div>
	
	<!--<div id="front-content-left-wrapper" class="clearfix grid12_8">
		<?php //print render($page['front_content_left']); ?>
	</div> -->
	
	<?php print render($page['front_right']); ?>
	<?php print render($page['front_top_left']); ?>
	
	<!-- <div id="front-content-right-wrapper" class="clearfix grid12_3">
		<?php //print render($page['front_content_right']); ?>
	</div> -->
	
	<?php print render($page['front_top_right']); ?>
	<?php print render($page['front_bottom']); ?>
	
	<div id="content" style="display:none">
	<?php print render($page['content']); ?>
	</div>
	
<?php
	include "includes/footer.tpl.php";
?>