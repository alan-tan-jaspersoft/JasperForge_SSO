<?php

/*
 * Created on Apr 7, 2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
session_start();
session_unset();
session_destroy();
header('Location: index.php');
?>
