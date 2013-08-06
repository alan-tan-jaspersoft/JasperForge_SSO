<?php
/**
 * @file
 *   Author Pane module compatibility.
 */

 dpm('called');
 
/**
 * Implementation of hook_preprocess_author_pane_user_picture().
 */
function gravatar_preprocess_author_pane_user_picture(&$variables) {
  // this method does not work because the Gravatar image is not stored locally (file_exists() test fail)
  //$account = $variables['account'];
  //$account->picture = _gravatar_get_account_user_picture($account);
  //$variables['account'] = $account;
  dpm($variables);
  $variables['picture'] = theme('user_picture', $variables['account']);
}