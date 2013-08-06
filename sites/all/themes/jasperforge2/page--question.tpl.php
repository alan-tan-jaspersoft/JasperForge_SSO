<?php
	include "includes/header.tpl.php";
?>
              <?php print render($title_prefix); ?>
              <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
              <?php endif; ?>
              <?php print render($title_suffix); ?>
			  <?php
				global $user;
				if(node_access("update", node_load(arg(1)), $user) === TRUE && !arg(2)) {
					echo l('[edit]','node/'.arg(1).'/edit',array('attributes'=> array('class' => 'jf-question-edit-link')));
				}
			  ?>
			  
			<div class="question-submitted">
				<?php print "Posted on ".date('F j, Y \a\t g:ia',$node->created); ?>
			</div>
          <?php //print theme('grid_block', array('content' => render($tabs), 'id' => 'content-tabs')); ?>
              <?php if ($action_links): ?>
              <ul class="action-links"><?php print render($action_links); ?></ul>
              <?php endif; ?>
              <div id="content-content" class="content-content">
                <?php print render($page['content']); ?>
              </div><!-- /content-content -->
            
<?php
	include "includes/footer.tpl.php";
?>