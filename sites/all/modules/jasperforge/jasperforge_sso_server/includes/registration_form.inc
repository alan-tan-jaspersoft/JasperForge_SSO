<?php

require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'user') . '/user.module';


function jasperforge_sso_server_reg_form($form, &$form_state)
{
  $form = array();
  if (!isset($form_state['step']))
    $form_state['step'] = 0;

  $step = jasperforge_sso_server_reg_steps($form_state['step']);
  $form = $step['form']($form, $form_state);
  $form['#validate'][] = 'jasperforge_sso_server_reg_validate';
//  $form['#submit'][] = 'user_register_submit';

  $form['#attached']['css'] = array(
    drupal_get_path('module', 'jasperforge_sso_server') . '/theme/css/form.css',
  );
  return $form;
}

function jasperforge_sso_server_reg_validate($form, &$form_state)
{
  $steps = jasperforge_sso_server_reg_steps($form_state['step']);
  $steps['validate']($form, $form_state);
}

/**
 * Util Function
 * Returns the registration step callback
 */
function jasperforge_sso_server_reg_steps($step = NULL)
{
  $steps = array(
    array('form' => 'user_register_form', 'validate' => 'jasperforge_sso_server_initial_validate'),
    array('form' => 'jasperforge_sso_server_step1', 'validate' => 'jasperforge_sso_server_step1_validate'),
    array('form' => 'jasperforge_sso_server_step2', 'validate' => 'jasperforge_sso_server_step2_validate'),
    array('form' => 'jasperforge_sso_server_step3', 'validate' => 'jasperforge_sso_server_step3_validate'),
    array('form' => 'jasperforge_sso_server_step4', 'validate' => 'jasperforge_sso_server_step4_validate'),
  );
  return isset($step)? $steps[$step] : $steps;
}

/**
 * implements hook_form_alter
 * Alters user_register_form
 */
function jasperforge_sso_server_form_alter(&$form, &$form_state, $form_id){
  if ($form_id != 'jasperforge_sso_server_reg_form' || $form_state['step'] != 0)
    return;

  unset($form['actions']);
  unset($form['#submit']);
//  $form['account']['name']['#type'] = 'hidden';
//  $form['account']['name']['#required'] = FALSE;
//  $form['#validate'] = array_merge(array('jasperforge_sso_server_set_username'), $form['#validate']);

  $form['account']['#prefix'] = '<div id="user-register-form">';
  $form['account']['#suffix'] = '</div>';

  if(isset($_GET['username']))
    $form['account']['name']['#default_value'] = $_GET['username'];
  if(isset($_GET['mail']))
    $form['account']['mail']['#default_value'] = $_GET['mail'];

  if(isset($form_state['values']['sso']['checkbox']))
    $check = $form_state['values']['sso']['checkbox'];
  elseif(isset($_GET['mail']))
    $check = TRUE;
  else
    $check = FALSE;
  $form['account']['sso'] = array(
    '#type' => 'checkbox',
    '#title' => 'Do you already have existing account(s) with any of the JasperSoft websites?',
    '#weight' => -100,
    '#default_value' => $check,
    );

  $form['account']['instruction'] = array(
    '#type' => 'fieldset',
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => -99,
    '#description' => 'If you would like to reuse or link an exisiting account, please enter the credentials of that account into the following text boxes.<br />
You will be able to link more existing accounts later.',
    '#states' => array(
      'visible' => array(
        ':input[id="edit-sso"]' => array('checked' => TRUE),)),
    );
/*  $form['sso']['instruction'] = array(
    '#type' => 'item',
    '#title' => t('From'),
    '#markup' => $user->name .' &lt;'. $user->mail .'&gt;',
    );*/

  $form['account']['pass'] = array(
    '#type' => 'password_confirm',
    '#required' => TRUE,
    '#size' => 25,
    '#description' => isset($_GET['mail'])?
                        t('Please enter the same password you use to log into the above account.')
                      : ''
  );

  $form['account']['next'] = array(
    '#type' => 'button',
    '#value' => t('Next'),
//    '#limit_validation_errors' => array(array('mail')),
    '#ajax' => array(
      'callback' => 'jasperforge_sso_server_user_register_ajax',
      'wrapper' => 'user-register-form',
      'effect' => 'fade',
      'method' => 'replace',
      'event' => 'click',
      ),
    );


}


function jasperforge_sso_server_initial_validate($form, &$form_state)
{
  if(form_get_errors())
    return;

  $form_state['rebuild'] = TRUE;

  $sites = jasperforge_sso_server_get_supported_sites();
  $mail = $form_state['values']['mail'];
  $pass = $form_state['values']['pass'];

  form_state_values_clean($form_state);
  $form_state['account_info'] = $form_state['values'];
  $_SESSION['form_state'] = $form_state;
  $_SESSION['form'] = $form;
  $form_state['existing'] = array();
  foreach ($sites as $site) // check to see if we already know this email
    if(JasperForgeSSO::find_cred($site, $mail))
      $form_state['existing'][$site] = $site;

///  $form_state['existing'] = array('community.jaspersoft.com' => 'community.jaspersoft.com');

  // we don't know this email
  if(empty($form_state['existing']))
  {
    $result = jasperforge_sso_server_check_name($form_state['values']['name']); // check name availability
    if($result !== TRUE){ // name not available
      form_set_error('name', $result);
      unset($form_state['account_info']);
      return;
    }
    $form_state['step'] = 3;  // so far so good
    return;
  }

  // we recognize this email
  $form_state['verified'] = FALSE;
  foreach ($form_state['existing'] as $site)  // see if we can validate
    if(JasperForgeSSO::validate_cred($site, $mail, $pass) === TRUE)
    {
      $form_state['force_add'] = $form_state['existing'];
      $form_state['verified'] = TRUE;
      break;
    }

  if(empty($form_state['force_add'])) // did not validate, ask for creds again
  {
    $form_state['step'] = 1;
    return;
  }

  // creds validated
  if(jasperforge_sso_server_check_name($form_state['values']['name'], $mail) !== TRUE)
  { // but name not available on all sites
    $form_state['step'] = 2;
    return;
  }

  // everything's good
  $form_state['step'] = 3;
}


/**
 * Validate existing account
 */
function jasperforge_sso_server_step1($form, &$form_state){
  $description = t('<em>@mail</em> has already been registered in the one or more of JasperSoft web sites. ',
                    array('@mail' => $form_state['account_info']['mail']));

//  $description .= '<br />' . t('Please enter a valid pair of Account Email and password to verify your ownership of the account(s), or click the <em>Prev</em> button to go back.');
  $description .= '<br />' . t('Please enter password to verify your ownership of the account(s), or click the <em>Prev</em> button to go back.');


  $form['user_register'] = array(
    '#type' => 'container',
    '#weight' => -1,
    '#title' => $description,
    '#tree' => TRUE,
    '#prefix' => '<div id="user-register-form">',
    '#suffix' => '</div>',
    );
  $form['user_register']['item'] = array(
    '#type' => 'item',
    '#weight' => -1,
    '#title' => $description,
    '#tree' => TRUE,
    );
  $form['user_register']['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#maxlength' => 64,
    '#size' => 20,
    '#required' => TRUE,
    '#tree' => TRUE,
    );
  $form['user_register']['reset_pass'] = array(
    '#type' => 'button',
    '#value' => t('Forgot Password'),
    '#name' => 'reset_pass',
    '#prefix' => '<div id="sso-reset-pass">',
    '#suffix' => '</div>',
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'jasperforge_sso_server_reset_pass_ajax',
      'wrapper' => 'sso-reset-pass',
      'effect' => 'fade',
      'method' => 'replace',
      ),
    );



  $form['user_register']['prev'] = array(
    '#type' => 'button',
    '#value' => t('Prev'),
    '#name' => 'prev',
    '#weight' => 10,
//    '#tree' => FALSE,
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'jasperforge_sso_server_user_register_ajax',
      'wrapper' => 'user-register-form',
      'effect' => 'fade',
      'method' => 'replace',
      ),
    );
  $form['user_register']['next'] = array(
    '#type' => 'button',
    '#value' => t('Next'),
    '#name' => 'next',
    '#weight' => 9,
    '#submit' => array('jasperforge_sso_server_reg_submit'),
//    '#tree' => FALSE,
    '#ajax' => array(
      'callback' => 'jasperforge_sso_server_user_register_ajax',
      'wrapper' => 'user-register-form',
      'effect' => 'fade',
      'method' => 'replace',
      'event' => 'click',
      ),
    );

//dpm($form_state['user'], 'step1');
  return $form;
}

function jasperforge_sso_server_step1_validate($form, &$form_state){
  if(form_get_errors())
    return;

  $form_state['rebuild'] = TRUE;
  if($form_state['clicked_button']['#name'] == 'reset_pass')
    return;

  if($form_state['clicked_button']['#name'] == 'prev')
  { // we are going back
    $form = $_SESSION['form'];
    $form_state = $_SESSION['form_state'];
    $form_state['rebuild'] = TRUE;
    $form_state['step'] = 0;
    return;
  }

  $verified = FALSE;
  foreach ($form_state['existing'] as $site)  // try to validate again
  {
    $verified = JasperForgeSSO::validate_cred(
                  $site,
//                  $form_state['values']['user_register']['mail'],
                  $form_state['account_info']['mail'],
                  $form_state['values']['user_register']['password']
                  );
    if($verified === TRUE)
      break;
    if(stristr($verified, 'blocked'))
    {
      $verified = TRUE;
      $form_state['verified'] = FALSE;  // user needs email verification
      break;
    }
  }
//$verified = TRUE;
  if($verified !== TRUE)
  {
    form_set_error('user_register', 'Unrecognized username or password');
    return;
  }
  $form_state['force_add'] = array_combine($form_state['existing'], $form_state['existing']);
  $form_state['verified'] = TRUE;
  $form_state['step'] = 3;
}


/**
 * validated but name not available, ask for new user name
 */
function jasperforge_sso_server_step2($form, &$form_state){
  $name = isset($_SESSION['form_state']['values']['name'])? $_SESSION['form_state']['values']['name']: '';

  $form['user_register'] = array(
    '#type' => 'container',
    '#weight' => -1,
//    '#title' => $description,
    '#tree' => TRUE,
    '#prefix' => '<div id="user-register-form">',
    '#suffix' => '</div>',
    );
  $form['user_register']['msg'] = array(
    '#type' => 'item',
    '#weight' => -11,
    '#markup' => "Your account has been verified, but <em>${name}</em> is already taken.
<br />Please enter a new user name.",
    );

  $form['user_register']['name'] = $_SESSION['form']['account']['name'];

  $form['user_register']['next'] = array(
    '#type' => 'button',
    '#value' => t('Next'),
    '#name' => 'next',
    '#weight' => 10,
    '#ajax' => array(
      'callback' => 'jasperforge_sso_server_user_register_ajax',
      'wrapper' => 'user-register-form',
      'effect' => 'fade',
      'method' => 'replace',
      'event' => 'click',
      ),
  );
  return $form;
}

function jasperforge_sso_server_step2_validate($form, &$form_state){
  if(form_get_errors())
    return;

  $form_state['rebuild'] = TRUE;

  $result = jasperforge_sso_server_check_name($form_state['values']['name'], $_SESSION['form_state']['values']['mail']);
  if($result !== TRUE)
  {
    form_set_error('name', $result);
    return;
  }
  $_SESSION['form_state']['values']['name'] = $form_state['values']['name'];

  $form_state['step'] = $form_state['step'] + 1;
}

/**
 * Ask if user want to link any more accounts
 */
function jasperforge_sso_server_step3($form, &$form_state)
{
//  $values = isset($form_state['multistep_values']['rate_the_room']) ? $form_state['multistep_values']['rate_the_room'] : array();
//dpm($form_state, 'step3');
  $description = t('If you already have existing accounts with any of the JasperSoft websites,
  you can link them with this JasperSoft ID. Note that you can only link one existing account per site.<br />
  If you have more than one account that you want to use with this JasperSoft ID, you may consider using the Account Merge
  feature. This feature is accessible after registration on your user profile page.');
  $form['user_register'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link Existing Accounts'),
    '#weight' => -1,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#prefix' => '<div id="account-link-table">',
    '#suffix' => '</div>',
    '#description' => $description,
    );

  $sites = jasperforge_sso_server_get_supported_sites();
  $last = abs(count($sites) - 1);
  foreach ($sites as $i => $site)
  {
    $tmp = l($site, "http://${site}", array('attributes' => array('target'=> '_blank')));
//    $tmp .= isset($links->creds[$site])? '&nbsp;<span class="marker">Validated</span>' : '';
    $form['user_register'][$site]['checkbox'] = array(
      '#type' => 'checkbox',
      '#title' => $tmp,
      '#prefix' => $i == 0 ? '<table><tr><th></th><th>Account Email</th><th>Password</th></tr><tr><td>' : '<tr><td>',
      '#suffix' => '</td>',
      '#default_value' => isset($form_state['force_add'][$site])? TRUE : FALSE,
      '#disabled' => isset($form_state['force_add'][$site])? TRUE : FALSE,
      );
    $form['user_register'][$site]['mail'] = array(
      '#type' => 'textfield',
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 100,
      '#required' => FALSE,
      '#default_value' => isset($form_state['force_add'][$site])? $_SESSION['form_state']['values']['mail'] : '',
      '#disabled' => isset($form_state['force_add'][$site])? TRUE : FALSE,
      '#prefix' => '<td>',
      '#suffix' => '</td>',
      );
    $form['user_register'][$site]['password'] = array(
      '#type' => 'password',
      '#maxlength' => 64,
      '#size' => 15,
      '#disabled' => isset($form_state['force_add'][$site])? TRUE : FALSE,
      '#prefix' => '<td>',
      '#suffix' => $i == $last ? '</td></tr></table>' : '</td></tr>',
      );
  }//foreach


  $form['user_register']['description'] = array(
    '#prefix' => '<div class="description">',
    '#markup' => 'Check the box next to the site name to add, uncheck to remove link.',
    '#suffix' => '</div>',
    );
  if(!empty($form_state['force_add']))
    $form['user_register']['force_add_description'] = array(
      '#prefix' => '<div class="description">',
      '#markup' => 'Some existing accounts are automatically dectected and merged. If you would like to link or merge
      other accounts, you can do so after registration.',
      '#suffix' => '</div>',
      );

  $form['user_register']['create_account'] = array(
    '#type' => 'submit',
    '#value' => t('Create new account'),
    '#name' => 'create_account',
    '#tree' => FALSE,
    '#submit' => array('jasperforge_sso_server_reg_submit'),
  );

  return $form;
}


function jasperforge_sso_server_step3_validate($form, &$form_state)
{
  $error = FALSE;
  foreach ($form_state['values']['user_register'] as $site => &$value)
  {
    if(isset($value['checkbox']) && $value['checkbox'] == 0)
    {
      unset($form_state['values']['user_register'][$site]);
      continue; //skip
    }

    if(isset($form_state['force_add'][$site]))
      continue; // already verified

    $result = JasperForgeSSO::validate_cred($site, $value['mail'], $value['password']);
    if($result === TRUE)
      continue;

    // error
    $error = TRUE;
    if(stristr($result, 'Unrecognized username or password'))
    {
      form_set_error("user_register][${site}", $result);
      continue;
    }

    // other error
    $msg = "We could not validate your account on <em>${site}</em> at the moment, Please try again later.";
    drupal_set_message($msg, 'warning');
  }//foreach

  $form_state['account_link'] = $form_state['values']['user_register'];

  if($error)
  {
    $form_state['rebuild'] = TRUE;
    return;
  }

  // one last check for name availability
  $result = jasperforge_sso_server_check_name(
                $_SESSION['form_state']['values']['name'],
                $_SESSION['form_state']['values']['mail']
                );
  if($result !== TRUE)
  {
      $form_state['step'] = $form_state['step'] + 1;
      $form_state['rebuild'] = TRUE;
      return;
  }

  // everything good, submit the form
  $form_state['rebuild'] = FALSE;
}




function jasperforge_sso_server_reg_submit($form, &$form_state)
{
  if($form_state['rebuild'] == TRUE)
    return;

  $verified = isset($form_state['verified']) && $form_state['verified'];

  if(!empty($form_state['account_link']))
    _jasperforge_sso_server_sync_roles($form_state['account_link']);

  jasperforge_sso_server_user_register_submit($_SESSION['form'], $_SESSION['form_state'], $verified);

  $form_state['account_info']['uid'] = $_SESSION['form_state']['values']['uid'];
  unset($_SESSION['form_state']);
  unset($_SESSION['form']);
  jasperforge_sso_server_add_links_submit($form_state, $verified);
}

function _jasperforge_sso_server_sync_roles($links)
{
  if(!isset($_SESSION['form_state']['values']['roles']))
    return;

  $tmp = array();
  foreach ($links as $site => $value) {
    $tmp += JasperForgeSSO::get_roles($site, $value['mail']);
  }
  array_unique($tmp);
  $roles = array();
  foreach ($tmp as $value)
  {
    $rid = db_query('SELECT rid FROM {role} WHERE name=:name', array(':name' => $value))->fetchField();
    if($rid)
    {
      $roles[$rid] = $value;
      continue;
    }
    $new_role = new stdClass();
    $new_role->name = $value;
    if(user_role_save($new_role) === FALSE)
      continue;
    $rid = db_query('SELECT rid FROM {role} WHERE name=:name', array(':name' => $value))->fetchField();
    $roles[$rid] = $value;
  }

  foreach ($roles as $key => $value)
    $_SESSION['form_state']['values']['roles'][$key] = $value;
}


function jasperforge_sso_server_user_register_submit($form, &$form_state, $verified)
{
  $admin = $form_state['values']['administer_users'];
  $pass = $form_state['values']['pass'];
  $notify = !empty($form_state['values']['notify']);

  // Remove unneeded values.
  form_state_values_clean($form_state);

  $form_state['values']['pass'] = $pass;
  $form_state['values']['init'] = $form_state['values']['mail'];
  $form_state['values']['status'] = $verified? 1 : 0;

  $account = $form['#user'];
  entity_form_submit_build_entity('user', $account, $form, $form_state);

  // Populate $edit with the properties of $account, which have been edited on
  // this form by taking over all values, which appear in the form values too.
  $edit = array_intersect_key((array) $account, $form_state['values']);
  $account = user_save($account, $edit);

  // Terminate if an error occurred during user_save().
  if (!$account)
  {
    drupal_set_message(t("Error saving user account."), 'error');
    $form_state['redirect'] = '';
    return;
  }
  $form_state['user'] = $account;
  $form_state['values']['uid'] = $account->uid;

  watchdog('user',
    'New user: %name (%email).',
    array('%name' => $form_state['values']['name'], '%email' => $form_state['values']['mail']),
    WATCHDOG_NOTICE,
    l(t('edit'), 'user/' . $account->uid . '/edit')
    );

  // Add plain text password into user account to generate mail tokens.
  $account->password = $pass;

  $uri = entity_uri('user', $account);
  // No e-mail verification required; log in user immediately.
  if ($verified
      || !variable_get('user_email_verification', TRUE) && !variable_get('user_verify_lock',0) && $account->status)
  {
    _user_mail_notify('register_no_approval_required', $account);
    $form_state['uid'] = $account->uid;
    user_login_submit(array(), $form_state);
    drupal_set_message(t('Registration successful. You are now logged in.'));
    $form_state['redirect'] = '';
  }
  elseif(variable_get('user_verify_lock',0))
    $form_state['redirect'] = '';
  elseif ($account->status || $notify)
  {  // No administrator approval required.
    $op = $notify ? 'register_admin_created' : 'register_no_approval_required';
    _user_mail_notify($op, $account);
    if ($notify)
      drupal_set_message(t('A welcome message with further instructions has been e-mailed to the new user <a href="@url">%name</a>.',
                         array('@url' => url($uri['path'], $uri['options']), '%name' => $account->name))
                         );
    else {
      drupal_set_message(t('A welcome message with further instructions has been sent to your e-mail address.'));
      $form_state['redirect'] = '';
    }
  }
  else
  {  // Administrator approval required.
    _user_mail_notify('register_pending_approval', $account);
    drupal_set_message(t('Thank you for applying for an account. Your account is currently pending approval by the site administrator.<br />In the meantime, a welcome message with further instructions has been sent to your e-mail address.'));
    $form_state['redirect'] = '';
  }
}


function jasperforge_sso_server_add_links_submit(&$form_state)
{
  $user = user_load($form_state['account_info']['uid']);
  $links = new JasperForgeSSO($form_state['account_info']['uid']);
//  $links->sync_name = isset($form_state['sync_name'])? $form_state['sync_name'] : FALSE;

  // add links
  if(isset($form_state['account_link']) && empty($form_state['account_link']) && !empty($form_state['force_add']))
  {
    $form_state['account_link'] = $form_state['force_add'];
    foreach ($form_state['account_link'] as &$value) {
      $value['mail'] = $form_state['account_info']['mail'];
      $value['checkbox'] = 1;
    }
  }
  foreach ($form_state['account_link'] as $site => $value)
  {
    if(isset($value['checkbox']) && $value['checkbox'] == 0)
      continue; //skip

    $data = array(
      'site' => $site,
      'cas_name' => $user->name,
      'mail' => $value['mail'],
      'type' => isset($form_state['force_add'][$site])? 'auto' : 'user',
      );
    $result = $links->sso_link($data);

    if($data['type'] == 'auto')
    {
//      JasperForgeSSO::merge_account($site, $user->name, $user->uid, $user->mail);
      $site = escapeshellarg($site);
      $cas_name = escapeshellarg($user->name);
      $cas_uid = escapeshellarg($user->uid);
      $mail = escapeshellarg($user->mail);
      exec("drush sso-user-merge {$site} {$cas_name} {$cas_uid} {$mail} > /dev/null &");
    }
    if($result === TRUE)
      continue; // added successfully, next one

    // other error
    $msg = "We could not link your account on <em>${site}</em> at the moment, Please try again later.";
    drupal_set_message($msg, 'warning');
  }//foreach

  $links->save();

  // run background task to synchronize user info if necessary
  if(empty($links->creds['active']))
    return;

  $data = array();
  $data['name'] = $user->name;
  $data['mail'] = $user->mail;

  $data = serialize($data);
  $data = escapeshellarg($data);
  exec("drush sso-update-remote {$user->uid} {$data} > /dev/null &");
}



function jasperforge_sso_server_step4($form, &$form_state)
{
  $form = array();
  $name = $_SESSION['form_state']['values']['name'];
  $form['msg'] = array(
    '#type' => 'item',
    '#weight' => -11,
    '#markup' => "Uh-oh, someone just took the user name: <em>${name}</em><br />Please choose a new user name.",
    );

  $form['name'] = $_SESSION['form']['account']['name'];

  $form['create_account'] = array(
    '#type' => 'submit',
    '#value' => t('Create new account'),
    '#name' => 'create_account',
    '#submit' => array('jasperforge_sso_server_reg_submit'),
  );

  return $form;
}

function jasperforge_sso_server_step4_validate($form, &$form_state){
  jasperforge_sso_server_step2_validate($form, $form_state);
  $result = jasperforge_sso_server_check_name(
              $form_state['values']['name'],
              $_SESSION['form_state']['values']['mail']
              );
  if($result !== TRUE)
  {
    form_set_error('name', $result);
    return;
  }
  $_SESSION['form_state']['values']['name'] = $form_state['values']['name'];
  $form_state['rebuild'] = FALSE;
}


function jasperforge_sso_server_user_register_ajax($form, $form_state)
{
  return isset($form['user_register'])? $form['user_register'] : $form;
}



function jasperforge_sso_server_reset_pass_ajax($form, $form_state)
{
  foreach ($form_state['existing'] as $site)
    exec("drush sso-reset-pass {$site} {$form_state['account_info']['mail']} > /dev/null &");

  drupal_set_message(t('Password reset instructions have been sent to <em>@mail</em>.',
                     array('@mail' => $form_state['account_info']['mail'])));
  return isset($form['user_register']['reset_pass'])? $form['user_register']['reset_pass'] : $form;
}


