<?php

//echo '<pre>'.print_r($form,1).'</pre>';

//$form['create'];
$form['create'] = array('#markup'=>ctools_modal_text_button(t('or register with Email'), 'modal_forms/nojs/jf-user-login-register-step2', t('Register with Email'), 'create-account-link ctools-modal-modal-popup-login-register-step2'));
unset($form['divider']);

?>

<script language="javascript" type="text/javascript">
	jQuery(function(){

		jQuery.fn.msnLabels = function (options) {
			var defaults = {
				'containerClass' : 'msnContainer',
				'dataAttr' : 'data-label',
				'labelClass' : 'inlineText',
				'containerObj' : '<span \>',
				'fadeOutspeed' : 100,
				'fadeInspeed' : 100
			};
			var option = jQuery.extend(defaults, options);
			var obj = jQuery(this);
			obj.each(function () {

				var element = jQuery(this);
				var inlineText = element.attr(option.dataAttr);

				var container = jQuery(option.containerObj, {
						'class' : option.containerClass
					});

				jQuery(element).wrap(container);

				var newLabel = jQuery('<span \>', {
						'text' : inlineText,
						'class' : option.labelClass
					});

				jQuery(newLabel).insertBefore(element);

			});
			jQuery('.' + option.labelClass).live('click', function () {
				jQuery(this).next('input').focus();
			});
			obj.focus(function () {
				showAndHide(jQuery(this));
			});
			function showAndHide(x) {

				x.prev('span').fadeOut(option.fadeOutspeed);

				obj.focusout(function () {

					var value = x.val();

					if (!value) {
						x.prev('span').fadeIn(option.fadeInspeed);
					} else {
						x.prev('span').fadeOut(option.fadeOutspeed);
					}
				});
			};
			obj.each(function () {

				var loadingVal = jQuery(this).val();

				if (loadingVal) {
					jQuery(this).prev('span').hide();
				}

			});
			jQuery('.' + option.containerClass).css({
				'position' : 'relative'
			});
			jQuery('.' + option.labelClass).css({
				'position' : 'absolute'
			});
		}
	});

	jQuery(function() {
		jQuery('.inlinelabel').msnLabels();
		jQuery('#lastname').prev('.inlineText').css('margin-left','20px');
	});

	jQuery(function() {
		var $ = jQuery;
		$('#user-register-submit').click(function() {
			var $this = $(this);
			var valid = true;
			var fields = $('#firstname,#lastname,#email');
			jssl = {};
			fields.each(function() {
				if($(this).val().length == 0) {
					valid = false;
					$(this).addClass('modal-invalid');
				}
				else {
					$(this).removeClass('modal-invalid');
					jssl[$(this).attr('id')] = $(this).val();
				}
			});
			if(valid) {
				$('.create-account-link').click();
			}
			else {
				return false;
			}
		});
	});

	jQuery(function() {
		var $ = jQuery;
		$('#firstname,#lastname,#email').live("keypress", function(e) {
			if(e.keyCode == 13) {
				$('#user-register-submit').click();
				return false;
			}
		});
	});

</script>

    <div class="modal2-rule"></div>

    <div id="modal2-content-wrapper">
        <div id="modal2-content-left">
            <h3 id="modal2-subtitle">Join the Jaspersoft Community, it's Free!</h3>
            <h4 id="modal2-list-title">Membership allows you to:</h4>
            <ul id="modal2-list">
                <li>Access official Jaspersoft Documentation</li>
                <li>Interact with members through our Answers</li>
                <li>Contribute to our Community Wiki</li>
                <li>Download Products and Extensions</li>
                <li>Submit enhancement requests and defects</li>
                <li>Provide feedback on community content</li>
                <li>Build credibility with the community</li>
            </ul>
        </div>
        <div id="modal2-content-right">
            <h3 class="modal2-label">Log In:</h3>
			<!--
            <input type="text" id="username" class="inlinelabel" data-label="Username" />
            <input type="password" id="password" class="inlinelabel" data-label="Password" />
            <input type="submit" value="Log In" id="user-login-submit" />
			-->

			<?php
				echo drupal_render($form['name']);
				echo drupal_render($form['pass']);
				echo drupal_render($form['actions']);
				echo drupal_render($form['reset_pass']);
			?>

            <div id="modal2-vdivider">
                <div class="modal2-rule"></div>
                <h3 class="modal2-label light">OR</h3>
                <div class="modal2-rule"></div>
            </div>

            <h3 class="modal2-label">Register with Email:</h3>
            <input type="text" id="firstname" class="inlinelabel" autocomplete="off" data-label="First Name" />
            <input type="text" id="lastname" class="inlinelabel" autocomplete="off" data-label="Last Name" />
            <input type="text" id="email" class="inlinelabel" autocomplete="off" data-label="Email Address" />
            <a class="button green" id="user-register-submit">Register</a>

        </div>
    </div>

    <div class="modal2-rule"></div>
    <h3 class="modal2-label light">Log In or Register with:</h3>

	<?php
        $form_keys = array_keys($form);
        foreach($form_keys as $key) {
          if(strpos($key,'oneall_social_login_oneall') !== FALSE) {
            echo drupal_render($form[$key]);
          }
        }
	?>

<?php echo drupal_render_children($form); ?>