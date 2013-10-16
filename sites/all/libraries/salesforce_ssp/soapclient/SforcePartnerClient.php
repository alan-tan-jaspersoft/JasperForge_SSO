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
//require_once ('SforceBaseClient.php');

/**
 * This file contains two classes.
 * @package SalesforceSoapClient
 */
/**
 * SforcePartnerClient class.
 *
 * @package SalesforceSoapClient
 */
class SforcePartnerClient extends SforceBaseClient {
  const PARTNER_NAMESPACE = 'urn:partner.soap.sforce.com';

  function SforcePartnerClient() {
    $this->namespace = self::PARTNER_NAMESPACE;
  }
}

/**
 * Salesforce Object
 *
 * @package SalesforceSoapClient
 */
class SObject {
  public $Id;
  public $type;
  public $fields;

  public function __construct($response=NULL) {
    if (isset($response)) {
	    $this->Id = $response->Id[0];
	    $this->type = $response->type;
	    if (isset($response->any)) {
	      try {
	        $this->fields = $this->convertFields($response->any);
	      } catch (Exception $e) {
	        var_dump($e);
	      }
	    }
    }
  }

  /**
   * Parse the "any" string from an sObject.  First strip out the sf: and then
   * enclose string with <Object></Object>.  Load the string using
   * simplexml_load_string and return an array that can be traversed.
   */
  function convertFields($any) {
    $new_string = ereg_replace('sf:', '', $any);
    $new_string = '<Object xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">'.$new_string.'</Object>';
    $xml = simplexml_load_string($new_string);
    return $xml;
  }
}
?>