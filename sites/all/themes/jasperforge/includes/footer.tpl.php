<?php

?>
</div><!-- /content-inner-inner -->
          </div><!-- /content-inner -->
        </div><!-- /content-group -->

        <?php print render($page['sidebar_second']); ?>
      </div><!-- /main -->

    </div><!-- /page-inner -->
  </div><!-- /page -->

	<div id="footer-wrapper">
      <?php print render($page['footer']); ?>
	</div>
	
	<?php
		$dest = $_SERVER['REQUEST_URI'];
		if(!user_is_logged_in()): 
	?>
		<a style="display:none;" id="hidden-login-register" href="/modal_forms/nojs/jf-user-login-register?optoutdest=<?php echo $dest; ?>" class="ctools-use-modal ctools-modal-modal-popup-login-register"></a>
	<?php endif; ?>
	
	<?php if( extension_loaded('newrelic') ) { echo newrelic_get_browser_timing_footer(); } ?>
	