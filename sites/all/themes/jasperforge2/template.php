<?php

function jasperforge2_preprocess_page(&$vars) {
	if(drupal_is_front_page()) {
		$vars['theme_hook_suggestions'][] = 'page__front';
		return;
	}
	if (isset($vars['node']->type)) {
		$vars['theme_hook_suggestions'][] = 'page__' . $vars['node']->type;
		//$vars['submitted'] = $vars['node']->submitted;
		//dpm($vars);
	}
}

function jasperforge2_preprocess_comment(&$vars) {
	if(isset($vars['node'])){
		$vars['theme_hook_suggestions'][] = 'comment__'.$vars['node']->type;
	}
}

function jasperforge2_preprocess_comment_wrapper(&$vars) {
	if(isset($vars['content']['#node']) && $vars['content']['#node'] && $vars['content']['#node']->type == 'answer') {
		$node = $vars['content']['#node'];
		if(user_access('post comments') && (variable_get('comment_form_location_' . $node->type, COMMENT_FORM_BELOW) == COMMENT_FORM_BELOW)) {
			$vars['content']['comment_form'] = drupal_get_form("comment_node_{$node->type}_form", (object) array('nid' => $node->nid));
		}
	}
}

function jasperforge2_preprocess_region(&$vars) {
	
	if($vars['region'] == 'front_top_left') {
		$key = array_search('grid12-12',$vars['classes_array']);
		unset($vars['classes_array'][$key]);
		$vars['classes_array'][] = 'grid12-2';
	}
	elseif($vars['region'] == 'front_top_right') {
		$key = array_search('grid12-12',$vars['classes_array']);
		unset($vars['classes_array'][$key]);
		$vars['classes_array'][] = 'grid12-7';
	}
	elseif($vars['region'] == 'front_right') {
		$key = array_search('grid12-12',$vars['classes_array']);
		unset($vars['classes_array'][$key]);
		$vars['classes_array'][] = 'grid12-3';
	}
	elseif($vars['region'] == 'front_bottom') {
		$key = array_search('grid12-12',$vars['classes_array']);
		unset($vars['classes_array'][$key]);
		$vars['classes_array'][] = 'grid12-9';
	}
}


function jasperforge2_wysiwyg_editor_settings_alter(&$settings, $context) {
   if ($context['profile']->editor == 'ckeditor') {
     $settings['skin'] = 'chris,' . base_path() . 'sites/all/libraries/ckeditor_skins/chris/';
   }
}

function jasperforge2_preprocess_field(&$vars) {
	if(isset($vars['element']['#classes']) && $vars['element']['#classes']) {
		$vars['classes_array'][] = $vars['element']['#classes'];
	}
}

