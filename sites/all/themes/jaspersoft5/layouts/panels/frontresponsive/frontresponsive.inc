<?php

/**
 * @file
 * Implementation of hook_panels_layouts
 */
function jaspersoft5_frontresponsive_panels_layouts() {
  $items['frontresponsive'] = array(
    'title'    => t('JS Responsive Front'),
    'category' => t('Jaspersoft Panels'),
    'icon'     => 'frontresponsive.png',
    'theme'    => 'frontresponsive',
    'admin css' => 'frontresponsive.admin.css',
    'theme arguments' => array('id', 'content'),
    'regions' => array(
      'frontresponsive_banner'        	=> t('Banner'),
      'frontresponsive_left_above'  	=> t('Left above'),
      'frontresponsive_middle_above'	=> t('Middle above'),
      'frontresponsive_right_above' 	=> t('Right above'),
      'frontresponsive_middle'      	=> t('Middle'),
      'frontresponsive_left_below'  	=> t('Left below'),
      'frontresponsive_right_below' 	=> t('Right below'),
      'frontresponsive_left_bottom'     => t('Left Bottom'),
      'frontresponsive_middle_bottom'   => t('Middle Bottom'),
      'frontresponsive_right_bottom'    => t('Right Bottom'),
    ),
  );
  return $items;
}

/**
 * Preprocess variables for two-brick.tpl.php
 */
function template_preprocess_frontresponsive(&$vars) {
  $vars['panel_prefix'] = '';
  $vars['panel_suffix'] = '';
}
