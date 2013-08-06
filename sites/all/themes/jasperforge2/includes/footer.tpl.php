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
	<?php if( extension_loaded('newrelic') ) { echo newrelic_get_browser_timing_footer(); } ?>
	