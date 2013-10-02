<?php

require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'user') . '/user.module';




/**
 * implements hook_form_FORM_ID_alter
 */
function jasperforge_sso_server_form_user_login_alter(&$form, &$form_state, $form_id)
{
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'jasperforge_sso_server') . '/theme/css/form.css',
  );
}

/**
 * implements hook_form_FORM_ID_alter
 */
function jasperforge_sso_server_form_user_pass_alter(&$form, &$form_state, $form_id)
{
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'jasperforge_sso_server') . '/theme/css/form.css',
  );
}





/**
 * implements hook_form_FORM_ID_alter
 * Added a section for linking existing accounts
 */
function jasperforge_sso_server_form_user_profile_form_alter(&$form, &$form_state, $form_id)
{
  $form['#validate'][] = 'jasperforge_sso_server_user_profile_validate';
}

function jasperforge_sso_server_user_profile_validate(&$form, &$form_state)
{
  if(form_get_errors())
    return;
  $links = new JasperForgeSSO($form['#user']->uid);

  // if name changed, check availability
  $available = TRUE;
  if($form['#user']->name != $form_state['values']['name']
      && jasperforge_sso_server_check_name($form_state['values']['name'], $form['#user']->mail) !== TRUE)
  {
    form_set_error('name', t('The name <em>@name</em> is already taken.',
                   array('@name' => $form_state['values']['name']))
                   );
  }

  // if email changed, check availability
  if($form['#user']->mail != $form_state['values']['mail'])
    _jasperforge_sso_server_check_email($form['#user']->uid, $form_state['values']['mail']);
}

/**
 * Util function
 * Check whether email is available when user changes email address
 *
 * @param String  $uid
 * @param String  $newmail
 */
function _jasperforge_sso_server_check_email($uid, $newmail)
{
  $links = new JasperForgeSSO($uid);
  foreach ($links->creds['active'] as $key => $value) {
    if(!isset($value['link_mail']))
      continue;
    if($newmail == $value['link_mail'])
    {
      form_set_error('mail', t('The e-mail address <em>@mail</em> is registered with one of your linked accounts.
If you would like to use this email address, the linked account need to be merged into your JasperSoft ID first.',
                        array('@mail' => $newmail)));
      return;
    }
  }

  foreach (jasperforge_sso_server_get_supported_sites() as $site)
  {
    $result = JasperForgeSSO::find_cred($site, $newmail);
    if(!$result)
      continue;

    form_set_error('mail', t('The e-mail address <em>@mail</em> is already taken.
          If you own this account, you can link or merge it with this Jaspersoft ID.',
                        array('@mail' => $newmail)));
    return;
  }//foreach
}




function jasperforge_sso_server_manage_form_access($user)
{
  if($user->uid == 1 || !user_edit_access($user))
    return FALSE;
  return TRUE;
}

function jasperforge_sso_server_manage_form($form, &$form_state, $user)
{
  if(isset($form_state['values']['tab__active_tab']))
    $default_tab = $form_state['values']['tab__active_tab'];
  elseif(isset($form_state['input']['tab__active_tab']))
    $default_tab = $form_state['input']['tab__active_tab'];
  else
    $default_tab = 'edit-account-merge';

  $form['tab'] = array(
    '#type' => 'vertical_tabs',
    '#default_tab' => $default_tab,
    );

  $form_state['user'] = $user;
  $form['tab']+= jasperforge_sso_server_merge_form($user);
  $form['tab']+= jasperforge_sso_server_link_form($user);

  return $form;
}


function jasperforge_sso_server_link_form($user)
{
  if($user->uid == 1)
    return;
  $form = array();
  $links = new JasperForgeSSO($user->uid);

  $form['account_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link Accounts'),
    '#tree' => TRUE,
//    '#prefix' => '<div id="account-link-table">',
//    '#suffix' => '</div>',
    '#description' => 'If you already have existing accounts with any of the JasperSoft websites,
    you can link them with this JasperSoft ID. By linking your existing accounts, you can use this new Jaspersoft ID with
    your exisitng accounts. Note that only ONE account can be linked on a given website.'
    );

  $sites = jasperforge_sso_server_get_supported_sites();
  $last = abs(count($sites) - 1);
  foreach ($sites as $i => $site)
  {
    $tmp = l($site, "http://${site}", array('attributes' => array('target'=> '_blank')));
    $tmp .= (isset($links->creds['active'][$site]) && $links->creds['active'][$site]['type'] == 'user')?
            '&nbsp;<span class="marker">Validated</span>' : '';
    $forced = FALSE;
    $form['account_link'][$site]['checkbox'] = array(
      '#type' => 'checkbox',
      '#title' => $tmp,
      '#prefix' => $i == 0 ? '<table><tr><th></th><th>Account Email</th><th>Password</th></tr><tr><td>' : '<tr><td>',
      '#suffix' => '</td>',
      '#default_value' => isset($links->creds['active'][$site]) && $links->creds['active'][$site]['type'] != 'auto'?
                          TRUE: FALSE,
      '#disabled' => $forced,
      );
    $form['account_link'][$site]['mail'] = array(
      '#type' => 'textfield',
      '#default_value' => '',
      '#size' => 50,
      '#maxlength' => 100,
      '#required' => FALSE,
      '#default_value' => isset($links->creds['active'][$site]) && $links->creds['active'][$site]['type'] == 'user'?
                          $links->creds['active'][$site]['link_mail'] : '',
      '#disabled' => $forced,
      '#states' => array(
        'disabled' => array(
          ':input[name="account_link[' . $site. '][checkbox]"]' => array('unchecked' => TRUE),)
          ),
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      );
    $form['account_link'][$site]['password'] = array(
      '#type' => 'password',
      '#maxlength' => 64,
      '#size' => 15,
      '#disabled' => $forced,
      '#states' => array(
        'disabled' => array(':input[name="account_link[' . $site. '][checkbox]"]' => array('unchecked' => TRUE),),
        ),

      '#prefix' => '<td>',
      '#suffix' => $i == $last ? '</td></tr></table>' : '</td></tr>',
      );

    if($forced)
    {
      unset($form['account_link'][$site]['mail']['#states']);
      unset($form['account_link'][$site]['password']['#states']);
    }
  }//foreach

  $check = FALSE;
  foreach ($links->creds['active'] as $key => $value)
    if($value['type'] == 'user')
    {
        $check = TRUE;
        break;
    }

  if($check)
  {
/*    $form['account_link']['sync_name'] = array(
      '#type' => 'checkbox',
      '#title' => 'Synchronize User Names',
      '#default_value' => $links->sync_name,
      '#tree' => FALSE,
      '#description' => 'Check this box to synchronize user names of your linked accounts.'
      );*/
    $form['account_link']['remove_all'] = array(
      '#type' => 'submit',
      '#name' => "remove_all_links",
      '#value' => t('Remove All Links'),
      '#tree' => FALSE,
      '#limit_validation_errors' => array(),
//      '#validate' => array('jasperforge_sso_server_ajax_submit'),
      '#submit' => array('jasperforge_sso_server_link_form_remove_submit')
/*      '#ajax' => array(
        'callback' => 'jasperforge_sso_server_ajax',
        'wrapper' => 'account-link-table',
        'effect' => 'fade',
        'method' => 'replace',
        ),*/
      );
  }//if


  $form['account_link']['submit'] = array(
      '#type' => 'submit',
      '#name' => 'submit_link',
      '#value' => t('Save'),
      '#limit_validation_errors' => array(array('account_link')),
      '#validate' => array('jasperforge_sso_server_link_form_validate'),
      '#submit' => array('jasperforge_sso_server_link_form_submit')
      );
//  $form['#validate'][] = 'jasperforge_sso_server_validate';
//  $form['#submit'][] = 'jasperforge_sso_server_submit';

  return $form;
}


function jasperforge_sso_server_link_form_validate(&$form, &$form_state)
{
  if($form_state['clicked_button']['#value'] != 'Save')
    return;
  form_state_values_clean($form_state);

  // check for errors in user input
  foreach ($form_state['values']['account_link'] as $site => $value)
  {
    if($value['checkbox'] == 0)
      continue;
    $msg = ' required.';
    if(!isset($value['password']) || trim($value['password']) == '')
      $msg = "& Password $msg";
    if(!isset($value['mail']) || trim($value['mail']) == '')
      $msg = "Account email address $msg";

    if($msg != ' required.')
      form_set_error("account_link][${site}", trim($msg,'& '));
  }

  if(form_get_errors())
    return;

  $links = new JasperForgeSSO($form_state['user']->uid);

  // validate new creds
  foreach ($form_state['values']['account_link'] as $site => &$value)
  {
    if($value['checkbox'] == 0)
      continue; //skip
    $value['mail'] = trim($value['mail']);
    if(isset($links->creds['active'][$site]['link_mail'])
        && $links->creds['active'][$site]['link_mail'] == $value['mail'])
      continue; //skip

    $result = JasperForgeSSO::validate_cred($site, $value['mail'], $value['password']);
    if($result === TRUE)
      continue;

    if(!stristr($result, 'Unrecognized') && !stristr($result, 'already linked'))
      $result = "We are unable to verify account <em>{$value['mail']}</em> at this moment, please try again later.";
    form_set_error("account_link][${site}", $result);
  }//foreach
}


function jasperforge_sso_server_link_form_remove_submit($form, &$form_state)
{
  $links = new JasperForgeSSO($form_state['user']->uid);
  foreach ($links->creds['active'] as $site => $mail)
    $links->sso_unlink($site);

  $links->save();
  unset($form_state['input']['account_link'] );
//  drupal_set_message('Account Link(s) Removed.');
  $form_state['rebuild'] = TRUE;
}


function jasperforge_sso_server_link_form_submit(&$form, &$form_state)
{
  if($form_state['clicked_button']['#name'] == 'remove_all_links')
    return;

  $need_save = FALSE;
  $links = new JasperForgeSSO($form_state['user']->uid);
/*  if(isset($form_state['values']['sync_name']) && $links->sync_name != $form_state['values']['sync_name'])
  {
    $links->sync_name = $form_state['values']['sync_name'];
    $need_save = TRUE;
  }*/

  // remove individual links
  foreach ($form_state['values']['account_link'] as $site => $value)
  {
    if($value['checkbox'] == 1)
      continue;
    if(!isset($links->creds['active'][$site]) || $links->creds['active'][$site]['type'] != 'user')
      continue;

    if($links->sso_unlink($site))
    {
      $need_save = TRUE;
      continue;
    }
    drupal_set_message("We could not unlink your account on <em>${site}</em> at the moment, please try again later.",
                       'warning');
  }

  // add or update links
  foreach ($form_state['values']['account_link'] as $site => &$value)
  {
    $skip = isset($value['checkbox']) && $value['checkbox'] == 0
           || isset($links->creds['active'][$site]['link_mail'])
              && $links->creds['active'][$site]['link_mail'] == $value['mail'];
    if($skip)
      continue; //skip unchanged items
    $value['mail'] = trim($value['mail']);
    $data = array(
      'site' => $site,
      'cas_name' => $form_state['user']->name,
      'mail' => $value['mail'],
      'type' => ($value['mail'] == $form_state['user']->mail ? 'forced' : 'user'),
      );
    $result = $links->sso_link($data);
    if($result === TRUE)
    {
      $need_save = TRUE;
      continue; // added successfully, next one
    }

    // error
    // remove the validated marker
    $last = strpos($form['account_link'][$site]['checkbox']['#title'], '&nbsp;');
    if($last !== FALSE)
    {
      $tmp = substr($form['account_link'][$site]['checkbox']['#title'], 0, $last);
      $form['account_link'][$site]['checkbox']['#title'] = $tmp;
    }

    $msg = "We are unable to link your account on <em>${site}</em> at the moment, please try again later.";
    drupal_set_message($msg, 'warning');
  }//foreach

  if($need_save)
    $links->save();

  unset($form_state['input']['account_link']);
  $form_state['rebuild'] = TRUE;
}

function jasperforge_sso_server_ajax_submit($form, &$form_state)
{
  $links = new JasperForgeSSO($form_state['user']->uid);
  foreach ($links->creds['active'] as $site => $mail)
    $links->sso_unlink($site);

  $links->save();
  unset($form_state['input']['account_link'] );
  drupal_set_message('Account Link(s) Removed.');
//  $form_state['rebuild'] = TRUE;
}


function jasperforge_sso_server_ajax($form, $form_state)
{
  return $form['account_link'];
}

/**
 * form callback for Account Merge
 */
function jasperforge_sso_server_merge_form($user)
{
  $form = array();
  $form['account_merge'] = array(
    '#type' => 'fieldset',
    '#title' => t('Merge Accounts'),
    '#tree' => TRUE,
//    '#prefix' => '<div id="account-link-table">',
//    '#suffix' => '</div>',
//    '#description' => 'If you already have existing accounts with any of the JasperSoft websites,
//    you can link them with this JasperSoft ID.'
    );
  $form['account_merge']['warning'] = array(
    '#prefix' => '<div class="messages warning">',
    '#markup' => 'This is an experimental feature. Use with caution!',
    '#suffix' => '</div>',
    );
  $form['account_merge']['description'] = array(
    '#prefix' => '<div class="description">',
    '#markup' => 'This feature allows users to merge one or more existing accounts into the new single sign-on account.
    Note that not all contents and information can be merged and changes may not apply immediately.
    Once merged, the merged existing account will no long be accessible.',
    '#suffix' => '</div>',
    );

 /* $merged = array(
    '172.17.8.175' => array(array('account' => 'atan@jaspersoft.com', 'timestamp' => time())),
    'community.jaspersoft.com' => array(array('account' =>'neko@gmail.com', 'timestamp' => time())),
    );
*/
  $merged = JasperForgeSSO::get_merged($user->uid);
  if(!empty($merged))
  {
    $form['account_merge']['item1'] = array(
      '#markup' => '<table><tr><th>Site</th><th>Account</th><th>Merged On</th></tr>'
      );
    foreach ($merged as $site => $value)
      foreach ($value as $item)
        $form['account_merge'][$site . $item['account'] ] = array(
          '#markup' => "<tr><td>${site}</td><td>{$item['account']}</td><td>"
                        . format_date($item['timestamp'], 'long')
                        . '</td></tr>'
          );
  }//if

  $form['account_merge']['item2'] = array(
    '#markup' => '</table>'
    );

  $form['account_merge']['new'] = array(
    '#type' => 'fieldset',
    '#title' => t('Merge Existing Account'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    );

  $supported_sites = jasperforge_sso_server_get_supported_sites();
  $form['account_merge']['new']['site'] = array(
    '#type' => 'select',
    '#title' => t('Website'),
    '#options' => array_combine($supported_sites, $supported_sites),
    '#required' => TRUE,
    );
  $form['account_merge']['new']['mail'] = array(
      '#type' => 'textfield',
      '#title' => 'Account Email',
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 100,
      '#required' => TRUE,
      '#default_value' => '',
      );
  $form['account_merge']['new']['password'] = array(
      '#type' => 'password',
      '#title' => 'Password',
      '#maxlength' => 64,
      '#required' => TRUE,
      '#size' => 20,
      );

  $form['account_merge']['new']['submit'] = array(
      '#type' => 'submit',
      '#name' => 'submit_merge',
      '#value' => t('Merge Account'),
      '#limit_validation_errors' => array(array('account_merge')),
      '#validate' => array('jasperforge_sso_server_merge_form_validate'),
      '#submit' => array('jasperforge_sso_server_merge_form_submit')
      );

//  $form['#validate'][] = 'jasperforge_sso_server_merge_form_validate';
//  $form['#submit'][] = 'jasperforge_sso_server_merge_form_submit';
  return $form;
}


function jasperforge_sso_server_merge_form_validate(&$form, &$form_state)
{
  if(form_get_errors())
    return;
/*  $merged = array(
    '172.17.8.175' => array(array('account' => 'atan@jaspersoft.com', 'timestamp' => time())),
    'community.jaspersoft.com' => array(array('account' =>'neko@gmail.com', 'timestamp' => time())),
    );*/

  // check if qualified for merging
  $result = db_query('SELECT uid FROM {users} WHERE mail=:mail',
                    array(':mail' => $form_state['values']['account_merge']['new']['mail']))
              ->fetchField();
  if($result) // it's an SSO account
  {
    form_set_error(
          'mail',
          t('The account <em>@mail</em> is not qualified for merging.',
            array('@mail' => $form_state['values']['account_merge']['new']['mail']))
          );
    return;
  }

  // check if already merged
  if(isset($merged[$form_state['values']['account_merge']['new']['site'] ]))
    foreach ($merged[$form_state['values']['account_merge']['new']['site'] ] as $value)
    {
      if($value['account'] != $form_state['values']['account_merge']['new']['mail'])
        continue;
      form_set_error(
          'mail',
          t('The account <em>@mail</em> on <em>@site</em> has already been merged or is being merged.',
            array('@mail' => $form_state['values']['account_merge']['new']['mail'],
                  '@site' => $form_state['values']['account_merge']['new']['site']))
          );
      return;
    }

  // check if qualified for merging on sso client
  $result = JasperForgeSSO::merge_qualification(
              $form_state['values']['account_merge']['new']['site'],
              $form_state['values']['account_merge']['new']['mail'],
              $form_state['values']['account_merge']['new']['password'],
              $form_state['user']->uid
              );
  if($result !== TRUE)
  {
    if(stristr($result, 'Unrecognized'))
      $msg = $result;
    else
    {
      $msg = 'The account <em>@mail</em> is not qualified for merging.';
      if($result != 'FALSE')
        $msg .= " Reason: $result";
    }
    form_set_error('mail', t($msg, array('@mail' => $form_state['values']['account_merge']['new']['mail'])));
    return;
  }
}


function jasperforge_sso_server_merge_form_submit($form, &$form_state)
{
// TODO batch process
  try{
    JasperForgeSSO::merge_account(
      $form_state['values']['account_merge']['new']['site'],
      $form_state['user']->name,
      $form_state['user']->uid,
      $form_state['values']['account_merge']['new']['mail']
      );

    drupal_set_message('The merge has been scheduled. While you can continue using your single sing-on account,
    changes from the merge may not appear immediately.');
  }
  catch(Exception $e)
  {
    if($e->getMessage() != 'SSO client Timed Out' || $e->getMessage() != $result->data)
      watchdog_exception('jasperforge_sso_server', $e);
    drupal_set_message('We are unable to service your request at the moment, please try again later.', 'error');
  }
}
