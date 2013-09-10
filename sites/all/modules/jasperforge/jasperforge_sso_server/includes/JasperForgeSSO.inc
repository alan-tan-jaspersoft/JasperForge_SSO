<?php


class JasperForgeSSO
{
    public $uid;
    public $name;
    public $creds = array('active' => array(), 'inactive' => array());
    public $sync_name = 0;
    public $skip = NULL;  // IF this is set to a site, that site will not receive any remote ops

    public function __construct($uid = NULL)
    {
      if(!db_table_exists('jasperforge_sso_links'))
        throw new Exception('Database table "jasperforge_sso_links" does not exist.');

      if($uid == NULL){
        return;
      }
      $this->uid = $uid;
      $this->name = db_query('SELECT name FROM {users} WHERE uid=:uid', array(':uid' => $uid))
                  ->fetchField();
      $result = db_query('SELECT * FROM {jasperforge_sso_links} WHERE uid=:uid', array(':uid' => $uid))
                  ->fetchAllAssoc('lid');
      if(!$result){
        return;
      }

      foreach ($result as &$value)  // contruct the $creds array
      {
        $status = $value->status == 1 ? 'active' : 'inactive';
        $site = $value->site;
        $value = (array) $value;
        $this->creds[$status][$site] = $value;
        unset($this->creds[$status][$site]['status']);
        unset($this->creds[$status][$site]['site']);
        unset($this->creds[$status][$site]['uid']);
        if(!$this->creds[$status][$site]['link_mail'])
          unset($this->creds[$status][$site]['link_mail']);
      }
    }

    /**
     * Saves changes to database
     */
    public function save(){
      if(!db_table_exists('jasperforge_sso_links'))
        throw new Exception('Database table "jasperforge_sso_links" does not exist.');

      if(!is_array($this->creds))
        $this->creds = array('active' => array(), 'inactive' => array());

      try{
        foreach ($this->creds['active'] as $key => $value)
        {
          $fields = array(
                  'uid' => $this->uid,
                  'status' => 1,
                  'site' => $key,
                  'link_uid' => $value['link_uid'],
                  'link_mail' => isset($value['link_mail'])? $value['link_mail'] : NULL,
                  'type' => $value['type']
                  );
          if(isset($value['lid']))
            db_update('jasperforge_sso_links')
              ->condition('lid', $value['lid'])
              ->fields($fields)
              ->execute();
          else
            db_insert('jasperforge_sso_links')
              ->fields($fields)
              ->execute();
        }

        foreach ($this->creds['inactive'] as $key => $value)
        {
          $fields = array(
                  'uid' => $this->uid,
                  'status' => 0,
                  'site' => $key,
                  'link_uid' => $value['link_uid'],
                  'link_mail' => isset($value['link_mail'])? $value['link_mail'] : NULL,
                  'type' => auto
                  );
          if(isset($value['lid']))
            db_update('jasperforge_sso_links')
              ->condition('lid', $value['lid'])
              ->fields($fields)
              ->execute();
          else
            db_insert('jasperforge_sso_links')
              ->fields($fields)
              ->execute();
        }
      }
      catch(Exception $e){
        watchdog_exception('JasperForgeSSO', $e);
      }
    }

    /**
     * Links an existing account
     * @param   array $site
     *    An array that contains the site, SSO user name, SSO uid, existing account's email, and type of the link
     * @return  bool  TRUE on success, FALSE otherwise
     */
    public function sso_link(Array $site)
    {
      if(isset($site['uid']))
      {
        $this->creds['active'][$site['site'] ] = array('link_uid' => $site['uid'], 'type' => $site['type']);
        if(isset($this->creds['inactive'][$site['site'] ])
            && $this->creds['inactive'][$site['site'] ]['link_uid'] == $site['uid'])
          unset($this->creds['inactive'][$site['site'] ]);

        return TRUE;
      }
      $result = JasperForgeSSO::add_cred(
                      $site['site'],
                      $this->name,
                      $this->uid,
                      $site['mail'],
                      $site['type']
                      );
      if(!is_numeric($result))
        return FALSE;

      if(!isset($site['type']))
          $site['type'] = 'user';

      // save the auto created user's record
      if(isset($this->creds['active'][$site['site'] ]) && $this->creds['active'][$site['site'] ]['type'] == 'auto')
        $this->creds['inactive'][$site['site'] ] = $this->creds['active'][$site['site'] ];
      // now udpate
      $this->creds['active'][$site['site'] ] = array('link_uid' => $result, 'type' => $site['type']);
      if($site['type'] == 'user')
        $this->creds['active'][$site['site'] ]['link_mail'] = $site['mail'];
      return TRUE;
    }

    /**
     * Unlinks an existing account
     * @param   string $site
     *    The site to unlink
     * @return  bool  TRUE on success, FALSE otherwise
     */
    public function sso_unlink($site)
    {
      if(JasperForgeSSO::remove_cred($site, $this->uid, $this->creds['active'][$site]['link_uid']))
      {
        if(isset($this->creds['inactive'][$site]))
        {
          $this->creds['active'][$site] = $this->creds['inactive'][$site]; //move auto created user back into link
          unset($this->creds['inactive'][$site]);
        }
        else
          unset($this->creds['active'][$site]);
        return TRUE;
      }

      return FALSE;
    }

    /**
     * Cancels all linked accounts on client sites
     * @param   String  $method
     *    The user cancellation method
     */
    public function cancel($method)
    {
      foreach ($this->creds['active'] as $site => $value)
        if(!isset($this->skip) || $this->skip != $site)
          JasperForgeSSO::cancel_user($site, $this->uid, $value['uid'], $method);

      if(!strstr($method, 'user_cancel_block')) // if deleting user
      {
        db_delete('jasperforge_sso_links')
          ->condition('uid', $this->uid)
          ->execute();
        $this->creds = NULL;
      }
    }

    public function delete($keep_content = 1)
    {
      foreach ($this->creds['active'] as $site => $value)
        JasperForgeSSO::delete_user($site, $this->uid, $value['uid'], $keep_content);

      db_delete('jasperforge_sso_links')
        ->condition('uid', $this->uid)
        ->execute();
      db_delete('jasperforge_sso_merge')
        ->condition('uid', $this->uid)
        ->execute();
    }

    /**
     * Sends updated account info to client site for updating linked accounts
     * @param   Array $edit
     *    An array of updated values, should be the same as the edit array used for user_save()
     */
    public function update_remote($edit)
    {
      if (!is_array($edit))
      {
        watchdog('jasperforge_sso_server', 'Input not valid.<br />' . dprint_r($edit, TRUE), NULL, WATCHDOG_ERROR);
      	return FALSE;
      }

      $cleanup = array();
      $edit['op'] = 'sso_update';
      $edit['secret'] = variable_get('jasperforge_sso_secret', '');
      $msg = 'Updating remote account(s) on<br />';
      foreach ($this->creds['active'] as $site => $value)
      {
        if($this->skip && $this->skip == $site)
          continue;

        $data = $edit;
        $data['uid'] = $value['link_uid'];
        $data['cas_uid'] = $this->uid;
        $data['cas_name'] = $this->name;

        if($value['type'] == 'user')
        {
          unset($data['name']);
          unset($data['status']);
          unset($data['mail']);
        }

        $options = array(
          'method' => 'POST',
          'data' => drupal_http_build_query($data),
          'timeout' => 20,
          'headers' => array(
            'Content-Type' => 'application/x-www-form-urlencoded',
            'Cache-Control' => 'no-cache',
            ),
          );

        $result = drupal_http_request("http://{$site}/jf-sso", $options);
        if(strstr($result->data, 'Local account does not exist'))
          $cleanup[] = $site;

        $msg .= "${site} => " . ($result->data == 'TRUE'? 'Success<br />' : "{$result->data}<br />");
      }//foreach

      // cleanup orphaned links
      foreach ($cleanup as $site)
        if(isset($this->creds['inactive'][$site]))
          $this->creds['active'][$site] = $this->creds['inactive'][$site];
        else
          unset($this->creds['active'][$site]);

      if(!empty($cleanup))
        $this->save();

      watchdog('jasperforge_sso_server', $msg);
    }

    /**
     * Validates credentials of existing account on client site
     * @static
     *
     * @param   String  $site
     *    The client site to check the cred on
     * @param   String  $mail\
     *    The email to ID the existing account
     * @param   String  $pass
     *    Password of the existing account
     * @param   String  $cas_name
     *    The SSO account user name.
     * @return  Mixed   Boolean TRUE on success, else the error message
     */
    public static function validate_cred($site, $mail, $pass, $cas_name = NULL)
    {
      $array = array(
        'op'=>'validate',
        'mail' => $mail,
        'pass' => $pass,
        'secret' => variable_get('jasperforge_sso_secret', '')
        );
      if($cas_name)
        $array['cas'] = $cas_name;

      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query($array),
        'timeout' => 20,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Cache-Control' => 'no-cache',
          ),
        );

      $result = drupal_http_request("http://{$site}/jf-sso", $options);
      if($result->code == HTTP_REQUEST_TIMEOUT){
        watchdog(
          'jasperforge_sso_server',
          "@site timed out while SSO server attempting to validate user account.",
          array('@site' => $site),
          WATCHDOG_ERROR
          );
        return 'Request Timed Out';
      }//if

      if(!stristr($result->data, 'Unrecognized')
          && !stristr($result->data, 'already linked')
          && !stristr($result->data, 'blocked')
          && $result->data != 'TRUE')
        watchdog('jasperforge_sso_server', dprint_r($result, TRUE), NULL, WATCHDOG_ERROR);

      return $result->data == 'TRUE'? TRUE : $result->data;
    }

    /**
     * Check whether the specified account can be merged
     * @static
     *
     * @param   String  $site
     *    The site to perform the check on
     * @param   String  $mail
     *    The email to ID the account
     * @param   String  $pass
     *    The password for validating the account
     * @param   String  $cas_uid
     *    The uid of the SSO user
     * @return  Mixed   Boolean TRUE on success, otherwise the error message
     */
    public static function merge_qualification($site, $mail, $pass, $cas_uid)
    {
      $array = array(
        'op'=>'merge_qualify',
        'mail' => $mail,
        'pass' => $pass,
        'cas_uid' => $cas_uid,
        'secret' => variable_get('jasperforge_sso_secret', '')
        );

      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query($array),
        'timeout' => 20,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Cache-Control' => 'no-cache',
          ),
        );

      $result = drupal_http_request("http://{$site}/jf-sso", $options);
      if($result->code == HTTP_REQUEST_TIMEOUT){
        watchdog(
          'jasperforge_sso_server',
          "@site timed out while SSO server attempting to validate user account.",
          array('@site' => $site),
          WATCHDOG_ERROR
          );
        return 'Request Timed Out';
      }//if

      if(!stristr($result->data, 'Unrecognized')
          && !stristr($result->data, 'linked')
          && $result->data != 'TRUE'
          && $result->data != 'FALSE')
        watchdog('jasperforge_sso_server', dprint_r($result, TRUE), NULL, WATCHDOG_ERROR);

      return $result->data == 'TRUE'? TRUE : $result->data;
    }

    /**
     * Returns an array of merged accounts
     * @static
     *
     * @param   String  $uid
     *    The uid of the SSO account
     */
    public static function get_merged($uid)
    {
      $result = db_query('SELECT * FROM {jasperforge_sso_merge} WHERE uid=:uid', array(':uid' => $uid))
                  ->fetchAllAssoc('mid');
      if(!$result)
        return array();

      $array = array();
      foreach ($result as $mid => $value)
        $array[$value->site][] = array('account' => $value->account, 'timestamp' =>$value->timestamp);
      return $array;
    }

    /**
     * Sends request to client site to merge account
     * @static
     *
     * @param   String  $site
     *    The site to perform the check on
     * @param   String  $mail
     *    The email to ID the account
     * @param   String  $pass
     *    The password for validating the account
     * @param   String  $cas_uid
     *    The uid of the SSO user
     * @throws
     */
    public static function merge_account($site, $cas_name, $cas_uid, $mail)
    {
      $array = array(
        'op' => 'merge',
        'cas' => $cas_name,
        'cas_uid' => $cas_uid,
        'mail' => $mail,
        'secret' => variable_get('jasperforge_sso_secret', '')
        );
      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query($array),
        'timeout' => 60,
        'headers' => array('Content-Type' => 'application/x-www-form-urlencoded', 'Cache-Control' => 'no-cache', ),
        );
      $result = drupal_http_request("http://{$site}/jf-sso", $options);
      if($result->code == HTTP_REQUEST_TIMEOUT)
      {
        watchdog('jasperforge_sso_server',
                "@site timed out while SSO server attempting to validate user account.",
                array('@site' => $form_state['values']['site']),
                WATCHDOG_ERROR);
        throw new Exception('SSO client Timed Out');
      }//if

      if($result->data != 'TRUE')
        throw new Exception($result->data);

      db_insert('jasperforge_sso_merge')
        ->fields(array(
          'uid' => $cas_uid,
          'site' => $site,
          'account' => $mail,
          'timestamp' => REQUEST_TIME
          ))
        ->execute();
    }

    /**
     * Add link
     * @static
     *
     * @param   String  $site
     *    The site to perform the check on
     * @param   String  $cas_name
     *    The user name of the SSO user
     * @param   String  $cas_uid
     *    The uid of the SSO user
     * @param   String  $mail
     *    The email to ID the account
     * @param   String  $type
     *    The password for validating the account
     * @return  Mixed The linked account's uid if success, boolean FALSE otherwise
     */
    public static function add_cred($site, $cas_name, $cas_uid, $mail, $type)
    {
      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query(array(
            'op'=>'link',
            'cas' => $cas_name,
            'mail' => $mail,
            'cas_uid' => $cas_uid,
            'type' => $type,
            'secret' => variable_get('jasperforge_sso_secret', ''))),
        'timeout' => 20,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Cache-Control' => 'no-cache',
          ),
        );

      $result = drupal_http_request("http://{$site}/jf-sso", $options);
      if($result->code == HTTP_REQUEST_TIMEOUT)
      {
        watchdog(
          'jasperforge_sso_server',
          "@site timed out while SSO server attempting to link user account.",
          array('@site' => $site),
          WATCHDOG_ERROR
          );
        return 'FALSE';
      }//if

      if($result->data == 'FALSE')
      {
        watchdog('jasperforge_sso_server', dprint_r($result, TRUE), NULL,WATCHDOG_ERROR);
        return FALSE;
      }

      return $result->data;
    }

    /**
     * Unlink account
     * @static
     *
     * @param   String  $site
     *    The site to perform the check on
     * @param   String  $cas_uid
     *    The uid of the SSO user
     * @param   String  $uid
     *    The uid of the account to remove link
     * @return  bool
     */
    public static function remove_cred($site, $cas_uid, $uid){
      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query(array(
            'op'=>'unlink',
            'uid'=> $uid,
            'cas_uid' => $cas_uid,
            'secret' => variable_get('jasperforge_sso_secret', ''))),
        'timeout' => 15,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Cache-Control' => 'no-cache',
          ),
        );

      $result = drupal_http_request("http://{$site}/jf-sso", $options);

      if($result->code == HTTP_REQUEST_TIMEOUT)
      {
        watchdog(
          'jasperforge_sso_server',
          "@site timed out while SSO server attempting to unlink user account.",
          array('@site' => $site),
          WATCHDOG_ERROR
          );
        return FALSE;
      }
      elseif($result == 'FALSE')
      {
        watchdog(
          'jasperforge_sso_server',
          "@site experienced problem while SSO server attempting to unlink user account.",
          array('@site' => $site),
          WATCHDOG_ERROR
          );
        return FALSE;
      }

      return TRUE;
    }

    /**
     * Find whether an account already existing with the provided email
     * @static
     *
     * @param   String  $site
     *    The site to perform the check on
     * @param   String  $mail
     *    The email to check for
     * @return  bool  TRUE if found, else FALSE
     */
    public static function find_cred($site, $mail)
    {
      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query(array(
            'op'=>'find',
            'mail' => $mail,
            'secret' => variable_get('jasperforge_sso_secret', ''))),
        'timeout' => 15,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Cache-Control' => 'no-cache',
          ),
        );

      $result = drupal_http_request("http://{$site}/jf-sso", $options);
      return $result->data == 'TRUE'? TRUE : FALSE;
    }

    /**
     * Check user name availability
     * @static
     *
     * @param   String  $site
     *    The site to perform the check on
     * @param   String  $name
     *    The user name to check for
     * @param   String  $ignore
     *    An email address. If set, user name belonging to this email will not be reported if found
     * @return  bool
     */
    public static function check_name($site, $name, $ignore)
    {
      $data = drupal_http_build_query(array(
            'op'=>'check_name',
            'name' => $name,
            'mail' => $ignore,
            'secret' => variable_get('jasperforge_sso_secret', '')
            ));
      $options = array(
        'method' => 'POST',
        'data' => $data,
        'timeout' => 15,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Cache-Control' => 'no-cache',
          ),
        );

      $result = drupal_http_request("http://{$site}/jf-sso", $options);
      return $result->data == 'TRUE'? TRUE : FALSE;
    }


    public static function delete_user($site, $cas_uid, $uid, $keep_content = 1)
    {
      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query(array(
            'op'=>'delete',
            'cas_uid' => $cas_uid,
            'uid' => $uid,
            'keep_content' => $keep_content,
            'secret' => variable_get('jasperforge_sso_secret', ''))),
        'timeout' => 20,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Cache-Control' => 'no-cache',
          ),
        );
      $result = drupal_http_request("http://{$site}/jf-sso", $options);

      if($result->code == HTTP_REQUEST_TIMEOUT){
        watchdog(
          'jasperforge_sso_server',
          "@site timed out while SSO server attempting to delete user account.",
          array('@site' => $site),
          WATCHDOG_ERROR
          );
        return 'Request Timed Out';
      }//if
      return $result->data == 'TRUE'? TRUE : $result->data;
    }

    /**
     * Cancels user account on client site
     * @static
     *
     * @param   String  $site
     *    The site to perform the op on
     * @param   String  $cas_uid
     *    The uid of the SSO user
     * @param   String  $uid
     *    The uid of the account to cancel
     * @param   String  $method
     *    The cancellation method
     * @return  Mixed   Boolean TRUE if success, otherwise the error msg
     */
    public static function cancel_user($site, $cas_uid, $uid, $method)
    {
      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query(array(
            'op'=>'cancel_user',
            'cas_uid' => $cas_uid,
            'uid' => $uid,
            'method' => $method,
            'secret' => variable_get('jasperforge_sso_secret', ''))),
        'timeout' => 60,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Cache-Control' => 'no-cache',
          ),
        );
      $result = drupal_http_request("http://{$site}/jf-sso", $options);

      if($result->code == HTTP_REQUEST_TIMEOUT){
        watchdog(
          'jasperforge_sso_server',
          "@site timed out while SSO server attempting to cancel user account.",
          array('@site' => $site),
          WATCHDOG_ERROR
          );
        return 'Request Timed Out';
      }//if
      return $result->data == 'TRUE'? TRUE : $result->data;
    }
}//class
