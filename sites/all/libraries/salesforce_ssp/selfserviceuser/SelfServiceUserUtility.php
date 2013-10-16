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

/**
 * This file contains one class for Self-Service.
 * @package SelfServiceUser
 */

$path = dirname(__FILE__);
require_once ("$path/../soapclient/SforcePartnerClient.php");

/**
 * Used to authenticate and create Self-Service users.
 * @package SelfServiceUser
 */
class SelfServiceUserUtility {
  private $mySforceConnection;
  private $myLoginResult;
  private $wsdl;
  private $goldenUsername;
  private $goldenPassword;

  public function __construct($pWsdl, $pGoldenUsername, $pGoldenPassword) {
    $this->wsdl = $pWsdl;
    $this->goldenUsername = $pGoldenUsername;
    $this->goldenPassword = $pGoldenPassword;

    try {
      $this->mySforceConnection = new SforcePartnerClient();
      $this->mySforceConnection->createConnection($this->wsdl);
      $this->myLoginResult = $this->mySforceConnection->login($this->goldenUsername, $this->goldenPassword);
    } catch (Exception $e) {
      throw $e;
    }
  }

  private function _getAccountId($accountName) {
    $queryString = 'SELECT Id FROM account WHERE Name=\'' . $accountName . '\'';
    $queryResult = $this->mySforceConnection->query($queryString);
    return $queryResult->records[0]->Id[0];
  }

  private function _createContact($firstName, $lastName, $email, $accountName) {
    // Contact must be associated with an Account to enable Self-Service User.
    $createContactFields = array (
      'FirstName' => $firstName,
      'LastName' => $lastName,
      'Email' => $email,
    'AccountId' => $this->_getAccountId($accountName));
    $contactSObject = new SObject();
    $contactSObject->fields = $createContactFields;
    $contactSObject->type = 'Contact';
    $createContactResult = $this->mySforceConnection->create(array (
      $contactSObject
    ));
    return $createContactResult;
  }

  /**
   * Authenticate the self-service user via the /sserv/login.jsp to
   * retrieve the self-service portal URL (cssURL) and user's session id (csssid)
   *
   * @param String $username        username to authenticate
   * @param String $sspPassword     password
   */
  public function getSSP_URL($username, $sspPassword) {
    try {
      $salesforceURL = $this->myLoginResult->serverUrl;
      $this->mySforceConnection->getLocation();
      $this->mySforceConnection->getSessionId();
      $describeSObjectResult = $this->mySforceConnection->describeSObject('Task');
      $orgId = $this->myLoginResult->userInfo->organizationId;
      $hostURL = $describeSObjectResult->urlDetail;
      $hostURLArray = parse_url($hostURL);
      $scheme = $hostURLArray['scheme'];
      $hostname = $hostURLArray['host'];

      $sspURL = $scheme . '://' . $hostname . '/sserv/login.jsp?orgId=' .
      $orgId . '&un=' . $username . '&pw=' . $sspPassword;
      $handle = fopen($sspURL, 'r');

      $buffer = "";
      while (!feof($handle)) {
        $buffer = $buffer . fgets($handle);
      }
      fclose($handle);
      preg_match('/(URL=)([^\?]+)(\?csssid=)([^\"]+)/i', $buffer, $matches);
	  //echo '<pre>'.htmlentities(print_r($buffer,1)).'</pre>';
      $cssURL = $matches[2];
      $cssID = $matches[4];
      $destURL = $scheme . '://' . $hostname . $cssURL . '?csssid=' . $cssID;
      return $destURL;
    } catch (Exception $e) {
      throw $e;
    }
  }

  /**
   * Creating a SSP User requires these 5 parameters.  A Contact object is created
   * and assigned to the specified accountName.  A SelfServiceUser object is created
   * and assigned to the newly created Contact.  The SelfServiceUser's password is then
   * set to the sspPassword passed in.
   *
   * @param String $firstName     First Name
   * @param String $lastName      Last Name
   * @param String $email         Email
   * @param String $sspPassword   Self-Service Portal password
   * @param String $accountName   Account Name
   */
  public function createSSPUser($firstName, $lastName, $email, $sspPassword, $accountName) {
    try {
      $response = $this->_createContact($firstName, $lastName, $email, $accountName);
      if ($response->success == true) {
        $createSSUFields = array (
          'FirstName' => $firstName,
          'LastName' => $lastName,
          'Username' => $email,
          'Email' => 'admin_email_preferably@test.com',
          'IsActive' => true,
          'TimeZoneSidKey' => $this->myLoginResult->userInfo->userTimeZone,
          'LocaleSidKey' => $this->myLoginResult->userInfo->userLocale,
          'LanguageLocaleKey' => $this->myLoginResult->userInfo->userLanguage,
          'ContactId' => $response->id
        );
        $sObject1 = new SObject();
        $sObject1->fields = $createSSUFields;
        $sObject1->type = 'SelfServiceUser';
        $createSSUResult = $this->mySforceConnection->create(array (
          $sObject1
        ));
        $this->mySforceConnection->setPassword($createSSUResult->id, $sspPassword);
      } else {
        echo 'Could not create SSP user since there was a problem creating the Contact';
      }
    } catch (Exception $e) {
      global $errors;
      $errors = $e->faultstring;
      echo $errors;
    }
  }
}
?>