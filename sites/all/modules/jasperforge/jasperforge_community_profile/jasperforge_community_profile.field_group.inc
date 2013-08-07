<?php
/**
 * @file
 * jasperforge_community_profile.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function jasperforge_community_profile_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_bio|user|user|form';
  $field_group->group_name = 'group_bio';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_profile';
  $field_group->data = array(
    'label' => 'Bio',
    'weight' => '2',
    'children' => array(
      0 => 'field_user_phone',
      1 => 'field_country',
      2 => 'field_first_name',
      3 => 'field_last_name',
      4 => 'field_state_us',
      5 => 'field_state_can',
      6 => 'field_about_me',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_bio|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_company_info|user|user|form';
  $field_group->group_name = 'group_company_info';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = 'group_profile';
  $field_group->data = array(
    'label' => 'Company Info',
    'weight' => '13',
    'children' => array(
      0 => 'field_user_role',
      1 => 'field_company',
      2 => 'field_company_size',
      3 => 'field_user_title',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_company_info|user|user|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_profile|user|user|form';
  $field_group->group_name = 'group_profile';
  $field_group->entity_type = 'user';
  $field_group->bundle = 'user';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Profile Information',
    'weight' => '1',
    'children' => array(
      0 => 'field_social_network',
      1 => 'picture',
      2 => 'group_bio',
      3 => 'group_company_info',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Profile Information',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_profile|user|user|form'] = $field_group;

  return $export;
}
