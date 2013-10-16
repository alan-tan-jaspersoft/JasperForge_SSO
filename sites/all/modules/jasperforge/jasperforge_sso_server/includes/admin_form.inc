<?php

/**
 * implements form callback for module settings page
 */
function jasperforge_sso_server_admin_form($form, &$form_state)
{
  $form['sites'] = array(
    '#title' => t('SSO Client Sites'),
    '#type' => 'textarea',
    '#rows' => 4,
    '#description' => t ('Enter the list of SSO client site URLs, separated by newline or comma.'),
    '#default_value' => variable_get('jf_sso_sites', "www.jaspersoft.com\ncommunity.jaspersoft.com"),
    '#weight' => -15,
    );

  $form['secret_container'] = array(
    '#type' => 'container',
    '#attributes' => array(),
    );
/*  $form['secret_container']['secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#default_value' => variable_get('jasperforge_sso_secret', ''),
    '#size' => 60,
    '#maxlength' => 256,
    '#required' => FALSE,
    '#prefix' => '<div id="field-secret">',
    '#suffix' => '</div>',
    );*/
  $form['secret_container']['secret'] = array(
    '#type' => 'item',
    '#title' => t('Secret Key'),
    '#description' => t('All supported sites using JasperForge SSO must have this secret key.'),
    '#markup' => '<div id="field-secret">' . variable_get('jasperforge_sso_secret', '') . '</div>',
    );

  $form['secret_container']['generate'] = array(
    '#type' => 'button',
    '#value' => t('Generate New Secret Key'),
    '#name' => 'generate',
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'jasperforge_sso_server_secret_key_ajax',
      'wrapper' => 'field-secret',
      'effect' => 'fade',
      'method' => 'replace',
      'event' => 'click',
      ),
    );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#name' => 'save_settings',
    '#tree' => FALSE,
    );

  $form['#validate'][] = 'jasperforge_sso_server_admin_validate';
  $form['#submit'][] = 'jasperforge_sso_server_admin_submit';
  return $form;
}

function jasperforge_sso_server_secret_key_ajax($form, $form_state)
{
//  $key = md5(time());
//  variable_set('jasperforge_sso_secret', $key);
//  $form['secret_container']['secret']['#default_value'] = variable_get('jasperforge_sso_secret', '');
  return $form['secret_container']['secret']['#markup'];
}

function jasperforge_sso_server_admin_validate(&$form, &$form_state)
{
//  dpm('Running Validate');
  if($form_state['clicked_button']['#name'] == 'generate')
  {
    $key = md5(time());
    variable_set('jasperforge_sso_secret', $key);
//    $form_state['values']['secret'] = $key;
//    $form_state['input']['secret'] = $key;
    $form_state['rebuild'] = TRUE;
  }
//  dpm($form);
//  dpm($form_state);

}

function jasperforge_sso_server_admin_submit(&$form, &$form_state)
{
  if($form_state['values']['sites'] != $form['sites']['#default_value'])
    variable_set('jf_sso_sites', $form_state['values']['sites']);

  drupal_set_message('The configuration options have been saved.');
}