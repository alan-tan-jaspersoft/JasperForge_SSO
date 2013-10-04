<?php


class JasperForgeSSO
{
    public $uid;
    public $name;
    public $creds = array('active' => array(), 'inactive' => array());
    public $sync_name = 0;
    public $skip = NULL;  // IF this is set to a site, that site will not receive any remote ops

    /**
     * @param int|string  $uid  The user's ID to load links for.
     */
    public function __construct($uid = NULL, $reset = FALSE)
    {
      if(!db_table_exists('jasperforge_sso_links'))
        throw new Exception('Database table "jasperforge_sso_links" does not exist.');

      if($uid == NULL)
        return;

      $this->uid = (int)$uid;

      // get data from cache
      if($reset && $result = cache_get("JasperForgeSSO_$uid", 'cache_field'))
      {
        $result = $result->data;
        $this->name = $result['name'];
        $this->creds = $result['creds'];
        return;
      }

      // get fresh data
      $this->name = db_query('SELECT name FROM {users} WHERE uid=:uid', array(':uid' => $uid))
                  ->fetchField();
      $result = db_query('SELECT * FROM {jasperforge_sso_links} WHERE uid=:uid', array(':uid' => $uid))
                  ->fetchAllAssoc('lid');
      if(!$result)
        return;

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

      cache_set("JasperForgeSSO_$uid",
                array('name' => $this->name, 'creds' => $this->creds),
                'cache_field',
                CACHE_TEMPORARY
                );
    }

    /**
     * Saves changes to database
     */
    public function save()
    {
      if(!db_table_exists('jasperforge_sso_links'))
        throw new Exception('Database table "jasperforge_sso_links" does not exist.');

      if(!is_array($this->creds))
        $this->creds = array('active' => array(), 'inactive' => array());

      $fields = array(
        'uid' => $this->uid,
        'status' => 1,
        );
      try{
        foreach ($this->creds['active'] as $key => $value)
        {
          $fields['site'] = $key;
          $fields['link_uid'] = $value['link_uid'];
          $fields['link_mail'] = isset($value['link_mail'])? $value['link_mail'] : NULL;
          $fields['type'] = $value['type'];

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

        $fields['status'] = 0;
        $fields['type'] = 'auto';
        foreach ($this->creds['inactive'] as $key => $value)
        {
          $fields['site'] = $key;
          $fields['link_uid'] = $value['link_uid'];
          $fields['link_mail'] = isset($value['link_mail'])? $value['link_mail'] : NULL;
          $fields['type'] = $value['type'];

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

      cache_set("JasperForgeSSO_{$this->uid}",
                array('name' => $this->name, 'creds' => $this->creds),
                'cache_field',
                CACHE_TEMPORARY
                );
    }

    /**
     * Links an existing account
     * @param   array $site
     *    An array that contains the site, existing account's email, and type of the link
     * @return  bool  TRUE on success, FALSE otherwise
     */
    //TODO change $site to separate params
    public function sso_link(Array $site)
    {
      if(isset($site['link_uid']))  // link request sent in by client site
      {
        if(!isset($this->creds['active'][$site['site'] ]))
          $this->creds['active'][$site['site'] ] = array('link_uid' => $site['link_uid'], 'type' => $site['type']);
        elseif($this->creds['active'][$site['site'] ]['type'] == 'user')
        { // new auto-type user will be put into inactive group
          $this->creds['inactive'][$site['site'] ] = array('link_uid' => $site['link_uid'], 'type' => $site['type']);
          return TRUE;
        }

        if(isset($this->creds['inactive'][$site['site'] ])
            && $this->creds['inactive'][$site['site'] ]['link_uid'] == $site['link_uid'])
          unset($this->creds['inactive'][$site['site'] ]);

        return TRUE;
      }

      $result = self::add_cred($site['site'], $this->name, $this->uid, $site['mail'], $site['type']);
      if(!is_numeric($result))
        return FALSE;

      if(!isset($site['type']))
          $site['type'] = 'user';

      // move the auto created user's record into inactive group
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
      // remove link in DB
      db_delete('jasperforge_sso_links')
          ->where('uid=:uid AND site=:site AND type=:type',
                  array(':uid' => $this->uid, ':site' => $site, 'type' => 'user'))
          ->execute();

      if($this->skip && $this->skip == $site
        || self::remove_cred($site, $this->uid, $this->creds['active'][$site]['link_uid']))
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
      cache_clear_all("JasperForgeSSO_{$this->uid}", 'cache_field');

      foreach ($this->creds['active'] as $site => $value)
        if(!isset($this->skip) || $this->skip != $site)
          self::cancel_user($site, $this->uid, $value['uid'], $method);

      if(!strstr($method, 'user_cancel_block')) // if deleting user
      {
        db_delete('jasperforge_sso_links')
          ->condition('uid', $this->uid)
          ->execute();
        db_delete('jasperforge_sso_merge')
          ->condition('uid', $this->uid)
          ->execute();
        $this->creds = NULL;
      }
    }

    /**
     * Sends user delete request to client sites
     */
    public function delete($keep_content = 1)
    {
      cache_clear_all("JasperForgeSSO_{$this->uid}", 'cache_field');

      foreach ($this->creds['active'] as $site => $value)
        self::delete_user($site, $this->uid, $value['uid'], $keep_content);

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

      if(isset($edit['name']))
        cache_set("JasperForgeSSO_{$this->uid}",
                  array('name' => $this->name, 'creds' => $this->creds),
                  'cache_field',
                  CACHE_TEMPORARY
                  );

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
          unset($data['roles']);
        }

        $result = self::http_request($site, $data);
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
     * Helper function for sending http requests
     * @static
     * @param String  $site
     *    The client site to send request to
     * @param array   $data
     *    An associative array conataining the query
     * @param int     $timeout
     *    Timeout in seconds
     * @return  Obj   See drupal_http_request()
     */
    protected static function http_request($site, $data = array(), $timeout = 15)
    {
      $data['secret'] = variable_get('jasperforge_sso_secret', '');
      $options = array(
        'method' => 'POST',
        'data' => drupal_http_build_query($data),
        'timeout' => $timeout,
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
          'Cache-Control' => 'no-cache',
          ),
        );
      return drupal_http_request("http://{$site}/jf-sso", $options);
    }

    /**
     * Validates credentials of existing account on client site
     * @static
     *
     * @param   String  $site
     *    The client site to check the cred on
     * @param   String  $mail
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
        );
      if($cas_name)
        $array['cas'] = $cas_name;

      $result = self::http_request($site, $array, 20);
      if($result->code == HTTP_REQUEST_TIMEOUT){
        watchdog(
          'jasperforge_sso_server',
          "@site timed out while SSO server attempting to validate user account.",
          array('@site' => $site),
          WATCHDOG_ERROR
          );
        return 'Request Timed Out';
      }//if

      if($result->data == 'Bad Request')
      {
        watchdog('jasperforge_sso_server', dprint_r($result, TRUE), NULL, WATCHDOG_ERROR);
        return FALSE;
      }

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
        );

      $result = self::http_request($site, $array, 20);
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
          && !stristr($result->data, 'not allowed')
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
     * @return  Array   An associtative array containing the accounts merged on each client site
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
     * @param   String  $cas_name
     *    The uid of the SSO user
     * @param   String  $cas_uid
     *    The uid of the SSO user
     * @param   String  $mail
     *    The email to ID the account
     */
    public static function merge_account($site, $cas_name, $cas_uid, $mail)
    {
      // Update the cred list is done by client site


      $array = array(
        'op' => 'merge',
        'cas' => $cas_name,
        'cas_uid' => $cas_uid,
        'mail' => $mail,
        );

      $result = self::http_request($site, $array, 20);
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
     * @return  Mixed   The linked account's uid if success, boolean FALSE otherwise
     */
    public static function add_cred($site, $cas_name, $cas_uid, $mail, $type)
    {
      $array = array(
            'op'=>'link',
            'cas' => $cas_name,
            'mail' => $mail,
            'cas_uid' => $cas_uid,
            'type' => $type,
        );

      $result = self::http_request($site, $array, 20);
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
     * @return  bool  TRUE if op is success
     */
    public static function remove_cred($site, $cas_uid, $uid)
    {
      // send unlink request to client site
      $array = array(
        'op'=>'unlink',
        'uid'=> $uid,
        'cas_uid' => $cas_uid,
        );
      $result = self::http_request($site, $array);
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
      $array = array('op'=>'find','mail' => $mail);
      $result = self::http_request($site, $array, 20);
      if($result->data == '1')
        throw new Exception("This email belongs to user 1 on $site");
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
     * @return  bool  TRUE if name is available
     */
    public static function check_name($site, $name, $ignore)
    {
      $data = array('op'=>'check_name', 'name' => $name, 'mail' => $ignore);
      $result = self::http_request($site, $data);
      return $result->data == 'TRUE'? TRUE : FALSE;
    }

    /**
     * Returns user roles from remote client site
     * @static
     * @param   String   $site
     *    The client site to send to request to
     * @param   String    $mail
     *    The mail to ID the user with
     * @return  Array   An array containing roles that the ID has
     */
    public static function get_roles($site, $mail)
    {
      $data = array('op'=>'get_roles','mail' => $mail);
      $result = self::http_request($site, $data);
      if($result->data)
        $result = json_decode($result->data);
      return $result? $result : array();
    }

    /**
     * Sends user delete request to remote client site
     */
    public static function delete_user($site, $cas_uid, $uid, $keep_content = 1)
    {
      $data = array(
            'op'=>'delete',
            'cas_uid' => $cas_uid,
            'uid' => $uid,
            'keep_content' => $keep_content,
        );
      $result = self::http_request($site, $data);
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
      $data = array(
            'op'=>'cancel_user',
            'cas_uid' => $cas_uid,
            'uid' => $uid,
            'method' => $method
        );
      $result = self::http_request($site, $data, 60);
      if($result->code == HTTP_REQUEST_TIMEOUT)
      {
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
