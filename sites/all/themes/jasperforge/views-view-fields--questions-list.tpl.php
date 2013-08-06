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


	<div class="title-wrapper">
		<?php print $fields['title']->content; ?>
		
		<div class="meta-wrapper">
			<?php print $fields['name']->wrapper_prefix; ?>
				<?php print $fields['name']->content; ?>
			<?php print $fields['name']->wrapper_suffix; ?>
			
			<?php print $fields['points']->wrapper_prefix; ?>
			&nbsp;(<?php print $fields['points']->content; ?>)
			<?php print $fields['points']->wrapper_suffix; ?>
			
			<?php print $fields['created']->wrapper_prefix; ?>
			<?php print $fields['created']->content; ?>
			<?php print $fields['created']->wrapper_suffix; ?>
			
			<?php print $fields['field_question_project']->wrapper_prefix; ?>
			<?php print $fields['field_question_project']->content; ?>
			<?php print $fields['field_question_project']->wrapper_suffix; ?>
		</div>
		
	</div>
	
	<div class="stats-wrapper">
		<div class="views-field views-field-value<?php echo ((int)$row->votingapi_cache_node_points_sum_value > 0) ? ' stats-blue' : ''; ?>">
			<?php print $fields['value']->content; ?>
			<div class="stats-label"><?php print format_plural($row->votingapi_cache_node_points_sum_value,'vote','votes'); ?></div>
		</div>
		
		
		<?php 
			if(isset($row->_field_data['nid']['entity']->field_best_answer['und'][0]['nid']) && $row->_field_data['nid']['entity']->field_best_answer['und'][0]['nid']) {
				$answer_class = ' stats-green';
			}
			elseif(isset($row->_field_data['nid']['entity']->field_answer_count['und'][0]['count']) && $row->_field_data['nid']['entity']->field_answer_count['und'][0]['count'] > 0) {
				$answer_class = ' stats-blue';
			}
			else {
				$answer_class = ' stats-red';
			}
		?>
		<div class="views-field views-field-field-answer-count<?php echo $answer_class; ?>">
			<?php print $fields['field_answer_count']->content; ?>
			<?php
				$ans_count = (isset($row->_field_data['nid']['entity']->field_answer_count['und'][0]['count']) && $row->_field_data['nid']['entity']->field_answer_count['und'][0]['count']) ? $row->_field_data['nid']['entity']->field_answer_count['und'][0]['count'] : 0;
			
			?>
			<div class="stats-label"><?php print format_plural($ans_count,'answer','answers'); ?></div>
		</div>
		
		<div class="views-field views-field-totalcount<?php echo ((int)$row->node_counter_totalcount > 0) ? ' stats-blue' : ''; ?>">
			<?php 
				$suffix = array("", "K", "M");
				if((int)$row->node_counter_totalcount == 0) { $hits = 0; } else {
				$hits = (round($row->node_counter_totalcount/pow(1000, ($i = floor(log($row->node_counter_totalcount, 1000)))), (int)$row->node_counter_totalcount >= 100000 ? 0 : 1) . $suffix[$i]); }
			?>
			
			<?php print $hits; //print $fields['totalcount']->content; ?>
			<div class="stats-label"><?php print format_plural($row->node_counter_totalcount,'view','views'); ?></div>
		</div>
		
		<?php
			global $vvfql;
			if($vvfql != TRUE) {
				watchdog('VVFQL','hello');
				$vvfql = TRUE;
			}
		
		?>
		
		
	</div>	
	
	
