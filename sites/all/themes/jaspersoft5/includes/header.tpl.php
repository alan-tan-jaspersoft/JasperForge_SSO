<?php

//dpm($page);
//print render($page['menu_bar']);
?>

	<div id="header-wrapper" class="p-rel w-full">

		<div id="header-bg" class="p-abs w-full">
			<div id="bg-1"></div>
			<div id="bg-2"></div>
			<div id="bg-3"></div>
			<div id="bg-4" class="grad-bl-med">
				<div id="bg-wave" class="mca"></div>
			</div>
			<div id="bg-5"></div>
			<div id="bg-6" class="grad-bl-med"></div>
			<div id="bg-7"></div>
		</div>

		<div id="leaderboard-wrapper">
			<div class="container clearfix">
				<ul id="top-menu" class="f-r d-ib ta-r">
				  <li><a href="http://www.jaspersoft.com"><?php echo t('JASPERSOFT.COM'); ?></a></li>
					<li><a href="http://community.jaspersoft.com"><?php echo t('COMMUNITY'); ?></a></li>
					<li><a href="http://support.jaspersoft.com"><?php echo t('SUPPORT'); ?></a></li>
					<li><a href="http://www.jaspersoft.com/contact-us"><?php echo t('CONTACT US'); ?></a></li>
				</ul>
			</div>
		</div>

		<div class="container clearfix">
			<header<?php print $header_attributes; ?>>

				<?php if ($site_logo || $site_name || $site_slogan): ?>
					<!-- start: Branding -->
					<div<?php print $branding_attributes; ?>>

						<?php if ($site_logo): ?>
							<div id="logo">
								<?php print $site_logo; ?>
							</div>
						<?php endif; ?>

					</div><!-- /end #branding -->
				<?php endif; ?>

				<?php print render($page['header']); ?>
			</header>

			<div id="nav-wrapper" class="p-rel">
				<div id="mobile-nav-trigger">&equiv;</div>
				<div id="menu-bar" class="nav clearfix">
				  <?php print $page['menu_bar']; ?>
				<?php //print render($page['menu_bar']); ?>
        <?php if(user_is_logged_in()) :?>
          <div id="logout-link"><a href="/user/logout" style="outline: 0px none; text-decoration: none;"><?php echo t('LOG OUT'); ?></a></div>
        <?php endif; ?>
				</div>

			</div>

		</div>
	</div>