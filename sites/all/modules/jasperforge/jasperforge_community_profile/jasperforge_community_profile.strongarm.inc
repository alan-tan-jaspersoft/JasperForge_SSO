<?php
/**
 * @file
 * jasperforge_community_profile.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function jasperforge_community_profile_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'gravatar_size';
  $strongarm->value = '128';
  $export['gravatar_size'] = $strongarm;

  return $export;
}
