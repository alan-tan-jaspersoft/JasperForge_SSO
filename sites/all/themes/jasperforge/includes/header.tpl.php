<?php
?>
<?php if( extension_loaded('newrelic') ) { echo newrelic_get_browser_timing_header(); } ?>
  <div id="page" class="page">
    <div id="page-inner" class="page-inner">

	<div id="header-bg">
		<div id="header-bg-1"></div>
		<div id="header-bg-2"></div>
		<div id="header-bg-3"></div>
		<div id="header-bg-4"></div>
		<div id="header-bg-5">
			<div id="header-bg-wave">
				<div id="header-bg-radial"></div>
			</div>
		</div>
		<div id="header-bg-6"></div>
		<div id="header-bg-7"></div>
		<div id="header-bg-8"></div>
		<div id="header-bg-9"></div>
	</div>
	

      <!-- header-group region: width = grid_width -->
      <div id="header-group" class="header-group region clearfix <?php print $grid_width; ?>">
	  
	  
		<ul id="tertiary-menu">
			<li><a href="http://www.jaspersoft.com/products">Products</a></li>
			<li><a href="http://www.jaspersoft.com/solutions">Solutions</a></li>
			<li><a href="http://www.jaspersoft.com/services">Services</a></li>
			<li><a href="http://www.jaspersoft.com/resources">Resources</a></li>
		</ul>

        <?php if ($logo || $site_name || $site_slogan): ?>
        <div id="header-site-info" class="header-site-info block">
          <div id="header-site-info-inner" class="header-site-info-inner inner">
            <?php if ($logo): ?>
            <div id="logo">
              <!-- <a href="<?php print check_url($front_page); ?>" title="<?php print t('Jaspersoft Community'); ?>"><img src="<?php print $logo; ?>" alt="<?php print t('Jaspersoft Community'); ?>" /></a> -->
              <a href="<?php print check_url($front_page); ?>" title="<?php print t('Jaspersoft Community'); ?>"><img src="/sites/all/themes/jasperforge/images/community_logo.png" alt="<?php print t('Jaspersoft Community'); ?>" /></a>
            </div>
            <?php endif; ?>
            <?php if ($site_name || $site_slogan): ?>
            <div id="site-name-wrapper" class="clearfix">
              <?php if ($site_name): ?>
              <span id="site-name"><a href="<?php print check_url($front_page); ?>" title="<?php print t('Jaspersoft Community'); ?>"><?php print $site_name; ?></a></span>
              <?php endif; ?>
              <?php if ($site_slogan): ?>
              <span id="slogan"><?php print $site_slogan; ?></span>
              <?php endif; ?>
            </div><!-- /site-name-wrapper -->
            <?php endif; ?>
          </div><!-- /header-site-info-inner -->
        </div><!-- /header-site-info -->
        <?php endif; ?>

        <?php print render($page['header']); ?>
        <?php print render($page['main_menu']); ?>
      </div><!-- /header-group -->

      <!-- main region: width = grid_width -->
      <div id="main" class="main region clearfix <?php print $grid_width; ?>">
        <?php print render($page['sidebar_first']); ?>

        <!-- content group: width = grid_width - sidebar_first_width - sidebar_second_width -->
        <div id="content-group" class="content-group region nested <?php print $content_group_width; ?>">
          <a id="main-content-area"></a>
          <?php print theme('grid_block', array('content' => $breadcrumb, 'id' => 'breadcrumbs')); ?>
          <?php print theme('grid_block', array('content' => $messages, 'id' => 'content-messages')); ?>
          <?php print theme('grid_block', array('content' => render($page['help']), 'id' => 'content-help')); ?>
          <div id="content-inner" class="content-inner block">
            <div id="content-inner-inner" class="content-inner-inner inner">