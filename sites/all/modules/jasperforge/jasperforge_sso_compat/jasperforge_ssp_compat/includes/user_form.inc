<?php



/**
 * Implementation of hook_form_alter().
 */
function jasperforge_ssp_compat_form_alter(&$form, $form_state, $form_id) {
  // Alter all user login forms.
  if((variable_get('salesforce_ssp_override', FALSE) || isset($_GET['salesforce']))
       && ($form_id == 'user_login' || $form_id == 'user_login_block' || $form_id == 'user_login_form'))
  {
    $form['name']['#maxlength'] = 128;
    $form['name']['#description'] = t('Enter your Salesforce Self-service username.');
    foreach($form['#validate'] as $key => &$value)
      if($value == 'salesforce_ssp_auth_authenticate')
      {
        $value = 'jasperforge_ssp_compat_authenticate';
        break;
      }
  }

}


function jasperforge_ssp_compat_salesforce_login_form($form, &$form_state) {
  global $user;

  // Display login form:
  $form['name'] = array('#type' => 'textfield',
    '#title' => t('Username'),
    '#size' => 60,
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#required' => TRUE,
  );

  $form['name']['#description'] = t('Enter your Salesforce Self-service username.');
  $form['pass'] = array('#type' => 'password',
    '#title' => t('Password'),
    '#description' => t('Enter the password that accompanies your username.'),
    '#required' => TRUE,
  );
  $form['#validate'] = array('jasperforge_ssp_compat_authenticate', 'user_login_final_validate');
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Log in'));

  return $form;
}

function jasperforge_ssp_compat_form_jasperforge_sso_server_manage_form_alter(&$form, $form_state, $form_id)
{
  $form['tab']['salesforce'] = array(
    '#type' => 'fieldset',
    '#title' => t('Salesforce Self-Service'),
    '#tree' => TRUE,
    );


  $form['tab']['salesforce']['description'] = array(
    '#prefix' => '<div class="description">',
    '#markup' => t('If you are a Salesforce Self-Service user, it\'s recommended that you add your Salesforce logins
    to your SSO account.'),
    '#suffix' => '</div>',
    );


  $result = db_query('SELECT aid, authname FROM {authmap} WHERE uid=:uid', array(':uid' => $form_state['user']->uid))
              ->fetchAllAssoc('authname');
  if(!empty($result))
  {
    $form['tab']['salesforce']['item1'] = array(
      '#markup' => '<table><tr><th>Saved Salesforce Accounts</th><th>Logged In</th></tr>'
      );
    foreach ($result as $key => $value)
    {
      $form['tab']['salesforce'][$key] = array('#markup' => "<tr><td>{$key}</td>");
      if(isset($_COOKIE['salesforce_ssp_auth_authenticated']) && $_COOKIE['salesforce_ssp_auth_authenticated'] == $key)
        $form['tab']['salesforce'][$key]['#markup'] .= '<td>&#x2713;</td></tr>';
      else
        $form['tab']['salesforce'][$key]['#markup'] .= '<td>&#x2717;</td></tr>';
    }
  }//if

  $form['tab']['salesforce']['item2'] = array(
    '#markup' => '</table>'
    );


  $form['tab']['salesforce']['new'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add Account'),
    '#weight' => 5,
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    );

  $form['tab']['salesforce']['new']['mail'] = array(
      '#type' => 'textfield',
      '#title' => 'Account Email',
      '#default_value' => '',
      '#size' => 30,
      '#maxlength' => 100,
      '#required' => TRUE,
      '#default_value' => '',
      '#attributes' => array('autocomplete' => 'off')
      );
  $form['tab']['salesforce']['new']['password'] = array(
      '#type' => 'password',
      '#title' => 'Password',
      '#size' => 20,
      '#maxlength' => 64,
      '#required' => TRUE,
      '#attributes' => array('autocomplete' => 'off')
      );

  $form['tab']['salesforce']['new']['submit'] = array(
      '#type' => 'submit',
      '#name' => 'submit_ssp',
      '#value' => t('Save'),
      '#limit_validation_errors' => array(array('salesforce')),
      '#validate' => array('jasperforge_ssp_compat_ssp_form_validate'),
      '#submit' => array('jasperforge_ssp_compat_ssp_form_submit')
      );

  $form['#attached']['css'][] = drupal_get_path('module', 'jasperforge_ssp_compat') . '/theme/css/form.css';

}




function jasperforge_ssp_compat_ssp_form_validate(&$form, &$form_state)
{
  require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'salesforce_ssp_auth') . '/salesforce_ssp_auth.module';

// TODO enable this
/*  $auth = salesforce_ssp_auth_sfdc_ssu($form_state['values']['salesforce']['new']['mail'],
                                       $form_state['values']['salesforce']['new']['password']
                                       );
  if($auth)*/
    return;

  form_set_error('salesforce][new', t('Sorry, unrecognized username or password.'));
}

function jasperforge_ssp_compat_ssp_form_submit(&$form, &$form_state)
{
  $form_state['rebuild'] = TRUE;
  unset($form_state['input']['salesforce'] );

  try{
    db_insert('authmap')
      ->fields(array(
        'uid' => $form_state['user']->uid,
        'authname' => $form_state['values']['salesforce']['new']['mail'],
        'module' => 'salesforce_ssp_auth'
        ))
      ->execute();
  }
  catch(Exception $e)
  {
    watchdog_exception('jasperforge_ssp_compat', $e);
    drupal_set_message(t('Sorry, we cannot add your Salesforce account at the moment. Please try again later'), 'error');
  }
}


