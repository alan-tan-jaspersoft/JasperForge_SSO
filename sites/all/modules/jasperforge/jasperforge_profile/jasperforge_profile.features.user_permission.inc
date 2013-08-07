<?php
/**
 * @file
 * jasperforge_profile.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function jasperforge_profile_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer dependencies.
  $permissions['administer dependencies'] = array(
    'name' => 'administer dependencies',
    'roles' => array(
      'Administrator' => 'Administrator',
    ),
    'module' => 'conditional_fields',
  );

  return $permissions;
}
