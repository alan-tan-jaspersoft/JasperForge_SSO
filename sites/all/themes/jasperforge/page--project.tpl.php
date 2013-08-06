<?php
	include "includes/header.tpl.php";
?>
			<?php 
				$resources_external = $support_external = 0;
				if(isset($node->field_resources_type[LANGUAGE_NONE]) && $node->field_resources_type[LANGUAGE_NONE][0]['value'] == 'External Link') {
					$resources_external = 1;
				}
				if(isset($node->field_support_type[LANGUAGE_NONE]) && $node->field_support_type[LANGUAGE_NONE][0]['value'] == 'External Link') {
					$support_external = 1;
				}
			?>
			<span id="resources-external" style="display:none;" data-value="<?php echo $resources_external; ?>"></span>
			<span id="support-external" style="display:none;" data-value="<?php echo $support_external; ?>"></span>
			
			
				<?php if(isset($project_icon) && $project_icon):?>
					<img id="project-icon" src="<?php print $project_icon; ?>" />
				<?php endif; ?>
				
				<div id="project-header-reviews">
					<?php
						drupal_add_css('sites/all/modules/fivestar/css/fivestar.css');
						
						print theme(
							'fivestar_static', 
							array(
								'rating' 	=> votingapi_select_single_result_value(
									array(
										'entity_id' 	=> $node->nid,
										'entity_type' 	=> 'node',
										'tag' 			=> 'rating',
										'function' 		=> 'average',
									)
								),
								'stars' 	=> 5, 
								'tag' 		=> 'rating',
								'widget'		=> array(
									'name'	=> 'basic project-header',
									'css'	=> 'sites/all/modules/fivestar/widgets/basic/basic.css',
								)
							)
						);
					?>
					<div id="project-header-reviews-count">
						<?php 
							if((int)$project_rating_count == 0) {
								$link = user_is_logged_in() ? l('Be the first!','comment/reply/'.$node->nid,array('query'=>array(drupal_get_destination()))) : l('Be the first!','user/login',array('query'=>array('destination'=>'comment/reply/'.$node->nid.'?destination=node/'.$node->nid.'/reviews')));
								print 'No reviews. '.$link;
							} else {
								print l(format_plural($project_rating_count,'(1 review)','(@count reviews)'),drupal_lookup_path('alias','node/'.$node->nid).'/reviews'); 
							}
							
						?>
					</div>
				</div>
				
			
			
			
              <?php print render($title_prefix); ?>
              <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
              <?php endif; ?>
              <?php print render($title_suffix); ?>
			  
			  <?php if(isset($project_tagline) && $project_tagline):?>
					<div id="project-tagline"><?php print $project_tagline; ?></div>
			  <?php endif; ?>
			  
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