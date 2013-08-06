<?php
	include "includes/header_new.tpl.php";
?>

<h2>TESTING</h2>
              <?php print render($title_prefix); ?>
              <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
              <?php endif; ?>
              <?php print render($title_suffix); ?>
          <?php print theme('grid_block', array('content' => render($tabs), 'id' => 'content-tabs')); ?>
              <?php if ($action_links): ?>
              <ul class="action-links"><?php print render($action_links); ?></ul>
              <?php endif; ?>
              <div id="content-content" class="content-content">
                <?php print render($page['content']); ?>
              </div><!-- /content-content -->
<?php
	include "includes/footer.tpl.php";
?>
