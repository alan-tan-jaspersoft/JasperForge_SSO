<?php

/**
 * @file
 * Fusion theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all, or
 *   print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $display_submitted: whether submission information should be displayed.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined, e.g. $node->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $node->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 */
 //dpm($content);
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $user_picture; ?>

  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if ($display_submitted || !empty($content['links']['terms'])): ?>
    <div class="meta">
      

      <?php if (!empty($content['links']['terms'])): ?>
        <div class="terms terms-inline">
          <?php print render($content['links']['terms']); ?>
        </div>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <?php if (!$teaser): ?>
    <div id="node-top" class="node-top region nested">
      <?php print render($node_top); ?>
    </div>
  <?php endif; ?>
  
  <div class="content clearfix"<?php print $content_attributes; ?>>
	
	<div class="jf-bug-table-wrapper grid12-9">
		<div class="gutter">
			<table class="jf-bug-table">
				<colgroup span="2"></colgroup>
				<colgroup span="2"></colgroup>
				<colgroup span="2"></colgroup>
				<tbody>
					<tr>
						<td class="jf-bug-table-title">Category:</td>
						<td><?php echo render($content['field_bug_category']); ?></td>
						
						<td class="jf-bug-table-title">Priority:</td>
						<td><?php echo render($content['field_bug_priority']); ?></td>
						
						<td class="jf-bug-table-title">Status:</td>
						<td><?php echo render($content['field_bug_status']); ?></td>
					</tr>
					<tr>
						<td class="jf-bug-table-title">Project:</td>
						<td><?php echo render($content['field_bug_project']); ?></td>
						
						<td class="jf-bug-table-title">Severity:</td>
						<td><?php echo render($content['field_bug_severity']); ?></td>
						
						<td class="jf-bug-table-title">Resolution:</td>
						<td><?php echo render($content['field_bug_resolution']); ?></td>
					</tr>
					<tr>
						<td class="jf-bug-table-title">Component:</td>
						<td><?php //echo render($content['field_bug_component']); ?></td>
						
						<td class="jf-bug-table-title">Reproducibility:</td>
						<td><?php echo render($content['field_bug_reproducibility']); ?></td>
						
						<td class="jf-bug-table-title">Assigned to:</td>
						<td><?php echo render($content['field_bug_assigned_user']); ?></td>
					</tr>
				</tbody>
			</table>
		</div>
	</div>
	
	<div class="jf-bug-action-wrapper grid12-3">
		<div class="gutter">
			<?php 
				//if(empty($content['links']['comment']['#links'])) {
					if ($node->comment == COMMENT_NODE_OPEN) {
						if (user_is_logged_in()) {
							if(user_access('post comments')) {
							  $content['links']['comment']['#links']['comment-add'] = array(
								'title' => t('Add new comment'), 
								'href' => "comment/reply/$node->nid", 
								'attributes' => array('title' => t('Add a new comment to this page.')), 
								'fragment' => 'comment-form',
							  );
							}
							else {
							  $content['links']['comment']['#links']['comment_forbidden'] = array(
								'title' => theme('comment_post_forbidden', array('node' => $node)), 
								'html' => TRUE,
							  );
							}
						}
						else {
							$content['links']['comment']['#links']['comment-add'] = array(
								'title' => t('Add new comment'), 
								'href' => "user/login", 
								'attributes' => array('title' => t('Add a new comment to this page.')), 
								'query' => array(drupal_get_destination()),
							  );
						}
						
					  }
				//}
				
				if(isset($content['links']) && isset($content['links']['comment']) && isset($content['links']['comment']['#links']) && isset($content['links']['comment']['#links']['comment_forbidden'])) {
					unset($content['links']['comment']['#links']['comment_forbidden']);
				}
				
				$content['links']['comment']['#links']['comment-add']['attributes']['class'][] = 'button green';
				print render($content['links']['comment']);
				if(user_is_logged_in()) {
					if(!subscriptions_get_subscription($user->uid, 'node', 'nid', $node->nid)) {
						print l('Subscribe','node/'.$node->nid.'/subscribe/'.drupal_get_token(),array('attributes'=>array('class'=>array('button blue jf-bug-subscribe'))));
					}
					else {
						print l('Unsubscribe','node/'.$node->nid.'/unsubscribe/'.drupal_get_token(),array('attributes'=>array('class'=>array('button blue jf-bug-subscribe'))));
					}
				}
				else {
					print l('Subscribe','user/login',array('attributes'=>array('class'=>'button blue jf-bug-subscribe'), 'query'=> array(drupal_get_destination())));
				}
			?>
		</div>
	</div>
  
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
	  hide($content['field_bug_old_id']);
	  
	  $content['field_bug_attachments']['#classes'] = 'grid12-9';
	  
	  
	  print render($content['rate_jasper']);
      print render($content);
    ?>
	
		<div class="bug-author">
			<?php //print $submitted; ?>
			<?php print theme(
				'author_pane', 
				array(
					'account' 			=> user_load($uid),
					'caller' 			=> 'node_question',
					'picture_preset' 	=> 'small_avatar',
					'context'			=> NULL,
					'disable_css' 		=> FALSE,
				));
			?>
		</div>
	
  </div>

  <?php //print render($content['links']); ?>

  <?php print render($content['comments']); ?>

  <?php if (!$teaser): ?>
    <div id="node-bottom" class="node-bottom region nested">
      <?php print render($node_bottom); ?>
    </div>
  <?php endif; ?>
  
</div>
