<?php

/*
 * Copyright (c) 2006, salesforce.com, inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice, this list of conditions and the
 *    following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 *    the following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *    Neither the name of salesforce.com, inc. nor the names of its contributors may be used to endorse or
 *    promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

include ('header.inc');
$path = dirname(__FILE__);
require_once ("$path/../selfserviceuser/SelfServiceUserUtility.php");
require_once ('XMLSecurityHandler.php');

$errors = null;

function saveUserToSalesforce($firstName, $lastName, $email) {
  // first we open the configuration file to get the SSP information
  $cfg = parse_ini_file('config.ini');
  $goldenUsername      = $cfg['goldenUsername'];
  $goldenPassword      = $cfg['goldenPassword'];
  $sspPassword         = $cfg['sspPassword'];
  $wsdl                = $cfg['sf_wsdl'];
  $accountName         = $cfg['accountName'];

  try {
    $ssuUility = new SelfServiceUserUtility($wsdl, $goldenUsername, $goldenPassword);
  } catch (Exception $e) {
  	global $errors;
    $errors = "There was a problem logging in as the golden user.  The SOAP error message was:  ".$e->faultstring;
    throw $e;
  }
  try {
    $ssuUility->createSSPUser($firstName, $lastName, $email, $sspPassword, $accountName);
  } catch (Exception $e) {
    global $errors;
    $errors = "There was a problem creating the Self-Service User.  The SOAP error message was:  ".$e->faultstring;
    throw $e;
  }
}

function saveUserToXML($username, $password, $email) {
	XMLSecurityHandler::persistUser($username, $password, $email);
}

/**
 * Checks to see if the user has submitted his
 * username and password through the login form,
 */
if (isset ($_POST['submitClick'])) {
  /* Check that all fields were typed in */
  if (!$_POST['username'] || !$_POST['firstname'] || !$_POST['lastname'] || !$_POST['password'] || !$_POST['password2'] || !$_POST['email']) {
    global $errors;
    $errors = ('Please fill in all fields.');
  } else if ($_POST['password'] != $_POST['password2']) {
    $errors = ('Passwords do not match.  Please try again');
  } else {
    /* Spruce up username, email */
    $_POST['username'] = trim($_POST['username']);
    $_POST['email'] = trim($_POST['email']);

    try {
      saveUserToSalesforce($_POST['firstname'], $_POST['lastname'], $_POST['email']);
      saveUserToXML($_POST['username'], $_POST['password'], $_POST['email']);
      header('Location: success.php');
    } catch (Exception $e) {
    	// Error message already created.  Fall through.
    }
  }
}
?>

<p><b>New User Sign-Up</b></p>
<?php
global $errors;
if (isset ($errors)) {
  echo '<p><b><span style=color:#FF0000>'.$errors.'</span><b></p>';
}
?>
<form id="signupform" name="signupform" method="post" action="">
<table width="200" border="1" bordercolor=#FF9966">
  <tr>
    <td><table width="551" height="94" border="0">
      <tr>
        <td width="172">Username:</td>
        <td width="367"><input type="text" name="username" /></td>
      </tr>
      <tr>
      	<td>&nbsp;</td>
      </tr>
      <tr>
        <td width="172">First Name:</td>
        <td width="367"><input type="text" name="firstname" /></td>
      </tr>
      <tr>
        <td width="172">Last Name:</td>
        <td width="367"><input type="text" name="lastname" /></td>
      </tr>
      <tr>
        <td>E-mail Address:</td>
        <td><input name="email" type="text" size="40" maxlength="80" /></td>
      </tr>
      <tr>
      	<td>&nbsp;</td>
      </tr>
      <tr>
        <td>Password:</td>
        <td><input type="password" name="password" /></td>
      </tr>
      <tr>
        <td>Confirm Password: </td>
        <td><input type="password" name="password2" /></td>
      </tr>
      <tr>
        <td>&nbsp;</td>
      </tr>
      <tr>
        <td>&nbsp;</td>
        <td><input type="submit" name="submitClick" value="Submit" /></td>
      </tr>
    </table></td>
  </tr>
</table>
</form>


