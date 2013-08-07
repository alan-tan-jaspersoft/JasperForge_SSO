<?php


$plugin = array(
  'title' => t('Oneall social login widget'),
  'description' => 'Provided by jasperforge_community_profile',
  'content_types' => 'community_profile_oneall',
  'single' => FALSE,
  'category' => array(t('Widgets'), 0),
  'render callback' => 'community_profile_oneall_content_type_render',
  'edit form' => 'community_profile_oneall_content_type_edit_form',
  'all contexts' => TRUE,
);

function community_profile_oneall_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass;

  if(empty($args)){
    return array();
  }

  $block->title = check_plain("Social Login");

  $settings = oneall_social_login_widget_get_settings();
  $containerid = 'oneall_social_login_providers_' . rand(99999, 9999999);
  $token = oneall_social_login_core_get_user_token_for_uid($args[0]);
  oneall_social_login_widget_add_js_plugin('social_link',
                                          $settings['api_subdomain'],
                                          $settings['enabled_providers'],
                                          $containerid,
                                          $token
                                          );
  $block->content = theme('oneall_social_login_widget_provider_container',
                          array('containerid' => $containerid));
  return $block;
}



/**
 * 'Edit form' callback for the content type.
 * This example just returns a form; validation and submission are standard drupal
 * Note that if we had not provided an entry for this in hook_content_types,
 * this could have had the default name
 * ctools_plugin_example_no_context_content_type_edit_form.
 *
 */
function community_profile_oneall_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['uid'] = array(
    '#type' => 'textfield',
    '#title' => t('User ID'),
    '#size' => 50,
    '#description' => t('The user to render social logins for.'),
    '#default_value' => !empty($conf['uid']) ? $conf['uid'] : '',
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );
  return $form;
}

function community_profile_oneall_content_type_edit_form_submit($form, &$form_state) {
//  foreach (array('item1', 'item2') as $key) {
//    $form_state['conf'][$key] = $form_state['values'][$key];
//  }
  $form_state['conf']['uid'] = $form_state['values']['uid'];
}