<?php

/**
 * implements hook_form_FORM_ID_alter
 */
function jasperforge_ssp_compat_form_jasperforge_sso_server_admin_form_alter(&$form, &$form_state, $form_id)
{
  $form['ssp_site'] = array(
    '#type' => 'textarea',
    '#rows' => 2,
    '#title' => t('Salesforce Self-service Compatible Site'),
    '#default_value' => variable_get('jasperforge_ssp_compat_ssp_site', ''),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
    '#weight' => -10,
    '#description' => t ('Enter the list of Salesforce Self-service enabled site URLs, separated by newline or comma.'),
  );

  $form['#submit'][] = 'jasperforge_ssp_compat_admin_form_submit';
}


function jasperforge_ssp_compat_admin_form_submit(&$form, &$form_state)
{
  if($form_state['values']['ssp_site'] != $form['ssp_site']['#default_value'])
    variable_set('jasperforge_ssp_compat_ssp_site', $form_state['values']['ssp_site']);
}
