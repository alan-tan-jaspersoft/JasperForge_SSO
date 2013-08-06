<?php
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
?>
<?php

	//prs($fields['views']->content);
	//prs($fields['field_answer_count']->raw);
	//prs(array_keys((array)$fields['author_name']->handler));
	//prs($fields['author_name']->handler->entity_type);
	//prs($fields['author_name_1']->raw);
	//prs(array_keys((array)$fields['name']->handler));
	//prs($fields['views']->content);
?>

<?php
	$fields['views']->content = str_replace(',','',$fields['views']->content);
?>

	<div class="title-wrapper">
		<?php print $fields['title']->content; ?>
		
		<?php if(isset($_GET['search_api_views_fulltext']) && $_GET['search_api_views_fulltext']): ?>
		<div class="excerpt-wrapper grid12-6">
			<?php print $fields['search_api_excerpt']->content; ?>
		</div>
		<?php endif; ?>
		
		<div class="meta-wrapper">
			<?php print $fields['author']->wrapper_prefix; ?>
				<?php print $fields['author']->content; ?>
			<?php print $fields['author']->wrapper_suffix; ?>
			
			<?php print $fields['author_points']->wrapper_prefix; ?>
			&nbsp;(<?php print $fields['author_points']->content; ?>)
			<?php print $fields['author_points']->wrapper_suffix; ?>
			
			<?php print $fields['created']->wrapper_prefix; ?>
			<?php print $fields['created']->content; ?>
			<?php print $fields['created']->wrapper_suffix; ?>
			
			<?php if(isset($fields['field_question_project'])): ?>
			<?php print $fields['field_question_project']->wrapper_prefix; ?>
			<?php print $fields['field_question_project']->content; ?>
			<?php print $fields['field_question_project']->wrapper_suffix; ?>
			<?php endif; ?>
			
			
			<?php 
			$value = db_query("SELECT value FROM votingapi_cache WHERE entity_type = 'node' AND tag = 'jasper_rate' AND function = 'sum' AND entity_id = :nid",array(':nid'=>(int)$fields['nid']->content))->fetchField();
			$fields['votes']->content = (isset($value) && $value) ? $value : 0;
			?>
			
		</div>
		
	</div>
	
	<div class="stats-wrapper">
		<div class="views-field views-field-value<?php echo ((int)$fields['votes']->content > 0) ? ' stats-vote-active' : ' stats-vote'; ?>">
			<?php print $fields['votes']->content; ?>
			<div class="stats-label"><?php print format_plural($fields['votes']->content,'vote','votes'); ?></div>
		</div>
		
		
		<?php 
			if($fields['field_best_answer']->content > 0) {
				$answer_class = ' stats-answer-selected';
			}
			elseif((int)$fields['field_answer_count']->content > 0) {
				$answer_class = ' stats-answer-active';
			}
			else {
				$answer_class = ' stats-answer';
			}
		?>
		<div class="views-field views-field-field-answer-count<?php echo $answer_class; ?>">
			<?php $answer_count = $fields['field_answer_count']->content ? $fields['field_answer_count']->content : 0; ?>
			<?php print $answer_count;//print $fields['field_answer_count']->content; ?>
			<div class="stats-label"><?php print format_plural($answer_count,'answer','answers'); ?></div>
		</div>
		
		<div class="views-field views-field-totalcount<?php echo ((int)$fields['views']->content > 0) ? ' stats-view-active' : ' stats-view'; ?>">
			<?php 
				$suffix = array("", "K", "M");
				if((int)$fields['views']->content == 0) { $hits = 0; } else {
				$hits = (round($fields['views']->content/pow(1000, ($i = floor(log($fields['views']->content, 1000)))), (int)$fields['views']->content >= 100000 ? 0 : 1) . $suffix[$i]); }
			?>
			
			<?php print $hits; //print $fields['totalcount']->content; ?>
			<div class="stats-label"><?php print format_plural($fields['views']->content,'view','views'); ?></div>
		</div>
	</div>	
	
	
