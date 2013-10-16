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

session_start();

$SSU_URL = $_SESSION['SSU_URL'];
$login_page = 'index.php';
if (!$SSU_URL) {
	header('Location: '.$login_page);
} else {
?>

<br />
<br />
<table align="center">
  <tr>
    <td>
	    <h1 align="center">Pizza of the Day</h1>
	  </td>
	</tr>
	<tr>
	  <td>&nbsp;</td>
	</tr>
	<tr>
    <td align="center"><h2>The Avant-gardist</h2></td>
	</tr>
	<tr>
    <td align="center">
      <ul>
    		<li>Athenian Olive Oil</li>
    		<li>San Remo Pesto Sauce</li>
    		<li>Barrel Aged Feta Cheese</li>
    		<li>Almond Nut Permesan Cheese</li>
    		<li>Vietnamese Basil</li>
  		</ul>
		</td>
   </tr>
  <tr>
    <td>
      To lock-in freshness, we ship our delicious pizzas on dry-ice.
      All pizzas are made of 100% organic and free-trade ingredients.
       <br /><br />

      If you have questions or are not happy with your pizza or delivery,
      please visit our <a href="selfserviceportal.php">Self-Service Portal</a>.
    </td>
  </tr>
</table>
<?php
}
?>
