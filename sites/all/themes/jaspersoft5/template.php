<?php

/**
 * @file
 * Process theme data.
 *
 * Use this file to run your theme specific implimentations of theme functions,
 * such preprocess, process, alters, and theme function overrides.
 *
 * Preprocess and process functions are used to modify or create variables for
 * templates and theme functions. They are a common theming tool in Drupal, often
 * used as an alternative to directly editing or adding code to templates. Its
 * worth spending some time to learn more about these functions - they are a
 * powerful way to easily modify the output of any template variable.
 *
 * Preprocess and Process Functions SEE: http://drupal.org/node/254940#variables-processor
 * 1. Rename each function and instance of "jaspersoft5" to match
 *    your subthemes name, e.g. if your theme name is "footheme" then the function
 *    name will be "footheme_preprocess_hook". Tip - you can search/replace
 *    on "jaspersoft5".
 * 2. Uncomment the required function to use.
 */

//$path_to_theme = drupal_get_path('theme','jaspersoft5');

/**
 * Preprocess variables for the html template.
 */
function jaspersoft5_preprocess_html(&$vars) {
  //global $theme_key;
	drupal_add_css('//netdna.bootstrapcdn.com/font-awesome/3.2.1/css/font-awesome.css',array('type' => 'external'));
  // Two examples of adding custom classes to the body.

  // Add a body class for the active theme name.
  // $vars['classes_array'][] = drupal_html_class($theme_key);

  // Browser/platform sniff - adds body classes such as ipad, webkit, chrome etc.
  // $vars['classes_array'][] = css_browser_selector();
}
/* -- Delete this line to enable.
// */


/**
 * Process variables for the html template.
 */
/* -- Delete this line if you want to use this function
function jaspersoft5_process_html(&$vars) {
}
// */


/**
 * Override or insert variables for the page templates.
 */
function jaspersoft5_preprocess_page(&$vars)
{
//  global $theme_key;
//  $theme_name = $theme_key;

//  $vars['site_logo'] = url('images/jspersoft3_logo.png');

//dpm($vars);
}


function jaspersoft5_process_page(&$vars)
{
  $menu = menu_local_tasks();
  $menu = '<nav class="block block-menu contextual-links-region menu-wrapper menu-bar-wrapper clearfix"><ul class="menu clearfix">' . render($menu['tabs']['output']) . '</ul></nav>';

  $vars['page']['menu_bar'] = $menu;
//  dpm($menu);
}


/**
 * Override or insert variables into the node templates.
 */
function jaspersoft5_preprocess_node(&$vars) {
	if(isset($vars['type']) && $vars['type']) {
		$vars['theme_hook_suggestions'][] = 'node__'.$vars['type'];
	}
}
/* -- Delete this line if you want to use these functions
function jaspersoft5_process_node(&$vars) {
}
// */


/**
 * Override or insert variables into the comment templates.
 */
/* -- Delete this line if you want to use these functions
function jaspersoft5_preprocess_comment(&$vars) {
}
function jaspersoft5_process_comment(&$vars) {
}
// */


/**
 * Override or insert variables into the block templates.
 */
/* -- Delete this line if you want to use these functions
function jaspersoft5_preprocess_block(&$vars) {
}
function jaspersoft5_process_block(&$vars) {
}
// */


/**
 *  Field Preprocessor.
 */
function jaspersoft5_preprocess_field(&$vars) {
	//kpr($vars);
}

/**
 *  Entity Preprocessor.
 */
function jaspersoft5_preprocess_entity(&$vars) {
	//kpr($vars);
}
