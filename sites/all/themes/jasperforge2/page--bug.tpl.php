<?php
	include "includes/header.tpl.php";
?>
              <?php print render($title_prefix); ?>
              <?php if ($title): ?>
			  <?php
				$id = '';
				if(is_array($node->field_bug_id) && isset($node->field_bug_id['und'])) {
					$id = '[#'.$node->field_bug_id['und'][0]['value'].'] - ';
				}
			  
			  ?>
              <h1 class="title"><?php print $id.$title; ?></h1>
              <?php endif; ?>
              <?php print render($title_suffix); ?>
			  
			  <?php
				global $user;
				if(node_access("update", node_load(arg(1)), $user) === TRUE && !arg(2)) {
					echo l('[edit]','node/'.arg(1).'/edit',array('attributes'=> array('class' => 'jf-bug-edit-link')));
				}
			  ?>
			  
			  <?php $name = theme('username', array('account' => $node)); ?>
			  <?php print '<span class="jf-bug-node-byline">Posted by '.$name.' on <em>'.format_date($node->created).'</em></span>'; ?>
			  
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