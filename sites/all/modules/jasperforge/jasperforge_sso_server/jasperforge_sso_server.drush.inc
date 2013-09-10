<?php

/**
 * @file
 * Drush support
 */

/**
 * Implements hook_drush_command().
 */
function jasperforge_sso_server_drush_command() {
  $items = array();
  $items['sso-sync-name'] = array(
    'description' => 'Synchronize SSO account name with local user name. Mainly internal use only.',
    'arguments' => array(
      'uid' => 'The UID of the SSO account being updated.',
      'name' => 'The new user name.',
      ),
    'required-arguments' => TRUE,
  );
  $items['sso-cleanup-remote'] = array(
    'description' => 'Cleans up linked remote accounts when SSO account is deleted. Mainly internal use only.',
    'arguments' => array(
      'uid' => 'The uid of the SSO account being deleted.',
      'keep_content' => '1 or 0, whether to delete the contents along with the user account.',
      ),
    'required-arguments' => TRUE,
  );
  $items['sso-update-remote'] = array(
    'description' => 'Update linked remote accounts when SSO account changed. Internal use only.',
    'arguments' => array(
      'uid' => 'The uid of the SSO account being updated.',
      'data' => 'The data to update remotes with.',
      'skip' => 'This client site will not receive the user op request.',
      ),
    'required-arguments' => FALSE,
  );
  $items['sso-user-op'] = array(
    'description' => 'Handles sending out user op requests to client sites. Internal use only.',
    'arguments' => array(
      'uid' => 'The uid of the SSO account being updated.',
      'method' => 'The data to update remotes with.',
      'skip' => 'This client site will not receive the user op request.',
      ),
    'required-arguments' => FALSE,
  );
  $items['sso-user-merge'] = array(
    'description' => 'Merges user account on client site.',
    'arguments' => array(
      'site' => 'The site to perform the merge on.',
      'cas_name' => 'The user name of the SSO account.',
      'cas_uid' => 'The uid of the SSO account.',
      'mail' => 'The email address to ID the account to be merged.',
      ),
    'required-arguments' => TRUE,
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function jasperforge_sso_server_drush_help($section) {
  $items = jasperforge_sso_server_drush_command();
  $name = str_replace('domain:', '', $section);
  if (isset($items[$name]))
    return dt($items[$name]['description']);
}

function drush_jasperforge_sso_server_sso_sync_name($uid, $newname, $oldname)
{
  include_once 'includes/JasperForgeSSO.inc';
  $link = new JasperForgeSSO($uid);
  foreach ($link->creds as $site => $value)
    if($link->sync_name || $value['type'] == 'forced' || $value['type'] == 'auto')
      JasperForgeSSO::set_name($site, $newname, $oldname, $value['uid']);
}

function drush_jasperforge_sso_server_sso_cleanup_remote($uid, $keep_content = 1)
{
  include_once 'includes/JasperForgeSSO.inc';
  $link = new JasperForgeSSO($uid);
  $link->delete($keep_content);
}

function drush_jasperforge_sso_server_sso_update_remote($uid, $data, $skip ='')
{
  $data = unserialize($data);
  $link = new JasperForgeSSO($uid);
  if($skip)
    $link->skip = $skip; // if this op is relayed from a client site, don't loopback to that client site
  watchdog('drush_jasperforge_sso_server', dprint_r($data, TRUE));
  $link->update_remote($data);
}


function drush_jasperforge_sso_server_sso_user_op($uid, $method, $skip ='')
{
  $link = new JasperForgeSSO($uid);
  if($skip)
    $link->skip = $skip; // if this op is relayed from a client site, don't loopback to that client site
  $link->cancel($method);
}

function drush_jasperforge_sso_server_sso_user_merge($site, $cas_name, $cas_uid, $mail)
{
  try{
    JasperForgeSSO::merge_account($site, $cas_name, $cas_uid, $mail);
  }
  catch(Exception $e)
  {
    if($e->getMessage() != 'SSO client Timed Out' || $e->getMessage() != $result->data)
      watchdog_exception('jasperforge_sso_server', $e);
  }
}
