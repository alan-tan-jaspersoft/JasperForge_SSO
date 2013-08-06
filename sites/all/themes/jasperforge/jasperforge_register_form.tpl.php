<?php

//echo '<pre>'.print_r($form,1).'</pre>';

?>

<script language="javascript" type="text/javascript">

	jQuery(function() {
		var $ = jQuery;
		if(typeof jssl == 'object') {
			$('.form-item-mail input').val(jssl.email);
			$('.field-name-field-first-name input').val(jssl.firstname);
			$('.field-name-field-last-name input').val(jssl.lastname);
		}
	});

</script>

    <div class="modal2-rule"></div>

    <div id="modal2-content-wrapper" class="modal-register">
		<?php

    function &user_form_recursive_render($target_key, &$array){
      foreach ($array as $key => &$value) {
        if($key[0] == '#'
            || strpos($key, 'field_') !== 0 && strpos($key, 'group_') !== 0
            || !is_array($value)){
          continue;
        }
        if($key == $target_key && is_array($value)){
          return $value;  // found the target
        }
        else{ // target not found but current element is array
          $value = &user_form_recursive_render($target_key, $value);
          if($value){ // try to find target inside this array
            return $value;  // found
          }
        }//if-else
        // continue to next
      }//foreach
      return FALSE;
    }



//		dpm($form['group_profile'], '$form[account]');
		//prw('UR-f',$form);
	  print drupal_render($form['account']);
	  $fields = array(
      0 => 'field_first_name',
      1 => 'field_last_name',
      2 => 'field_company',
      3 => 'field_user_phone',
      4 => 'field_company_size',
      5 => 'field_user_role',
      6 => 'field_country',
      7 => 'field_state_us',
      8 => 'field_state_can',
      );
    foreach ($fields as $target) {
      if($result = &user_form_recursive_render($target, $form)){
        print drupal_render($result);
      }
    }
      echo drupal_render($form['captcha']);
      echo drupal_render($form['actions']);
		?>
	</div>


<?php echo drupal_render_children($form); ?>