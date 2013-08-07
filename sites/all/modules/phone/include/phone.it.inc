<?php

/**
 * @file
 * CCK Field for Italian phone numbers.
 */

function phone_it_metadata() {
  // These strings are translated using t() on output.
  return array(
    'error' => '"%value" is not a valid Italian phone number<br>Italian phone numbers should be like 999 999/999999.',
  );
}

/**
 * Verifies that $phonenumber is valid
 *
 * @param string $phonenumber
 * @return boolean Returns boolean FALSE if the phone number is not valid.
 */
function valid_it_phone_number($phonenumber) {
  // define regular expression
  $regex = "/^(\+39)?[\s]?([0-9]{2,3}(\/|-|\s)?[0-9]{6,8})$/i";

  // return true if valid, false otherwise
  return (bool) preg_match($regex, $phonenumber);
}

/**
 * Formatting for Italian Phone Numbers.
 *
 * @param string $phonenumber
 * @return string Returns a string containting the phone number with some formatting.
 */
function format_it_phone_number($phonenumber, $field) {

  //$phonenumber = trim($phonenumber);

   // do some formatting on the phone number
   $phonenumber = str_replace("[\s-]", "", $phonenumber);

   if ($field['phone_country_code']) {
      if ($matches[1] != "+39") {
        $phonenumber = "+39" . " " . $phonenumber;
      }
   }

   return $phonenumber;
}
