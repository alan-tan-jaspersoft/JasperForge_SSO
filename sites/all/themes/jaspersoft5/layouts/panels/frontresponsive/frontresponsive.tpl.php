<?php
/**
 * @file
 * Jaspersoft implementation to present a Panels layout.
 *
 * Available variables:
 * - $content: An array of content, each item in the array is keyed to one
 *   panel of the layout.
 * - $css_id: unique id if present.
 * - $panel_prefix: prints a wrapper when this template is used in certain context,
 *   such as when rendered by Display Suite or other module - the wrapper is
 *   added by jaspersoft in the appropriate process function.
 * - $panel_suffix: closing element for the $prefix.
 *
 * @see jaspersoft5_preprocess_frontresponsive()
 * @see jaspersoft5_preprocess_node()
 * @see jaspersoft5_process_node()
 */

// Ensure variables are always set. In the last hours before cutting a stable
// release I found these are not set when inside a Field Collection using Display
// Suite, even though they are initialized in the templates preprocess function.
// This is a workaround, that may or may not go away.
$panel_prefix = isset($panel_prefix) ? $panel_prefix : '';
$panel_suffix = isset($panel_suffix) ? $panel_suffix : '';
?>
<?php print $panel_prefix; ?>
<div class="at-panel panel-display frontresponsive clearfix" <?php if (!empty($css_id)): print "id=\"$css_id\""; endif; ?>>
  <?php if ($content['frontresponsive_banner']): ?>
    <div class="region region-frontresponsive-top region-conditional-stack">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_banner']; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="panel-row row-1 clearfix">
    <div class="region region-frontresponsive-left-above">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_left_above']; ?>
      </div>
    </div>
	<div class="region region-frontresponsive-middle-above">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_middle_above']; ?>
      </div>
    </div>
    <div class="region region-frontresponsive-right-above">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_right_above']; ?>
      </div>
    </div>
  </div>
  <?php if ($content['frontresponsive_middle']): ?>
    <div class="region region-frontresponsive-middle region-conditional-stack">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_middle']; ?>
      </div>
    </div>
  <?php endif; ?>
  <div class="panel-row row-2 clearfix">
    <div class="region region-frontresponsive-left-below">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_left_below']; ?>
      </div>
    </div>
    <div class="region region-frontresponsive-right-below">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_right_below']; ?>
      </div>
    </div>
  </div>
  <div class="panel-row row-3 clearfix">
    <div class="region region-frontresponsive-left-bottom">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_left_bottom']; ?>
      </div>
    </div>
	<div class="region region-frontresponsive-middle-bottom">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_middle_bottom']; ?>
      </div>
    </div>
    <div class="region region-frontresponsive-right-bottom">
      <div class="region-inner clearfix">
        <?php print $content['frontresponsive_right_bottom']; ?>
      </div>
    </div>
  </div>
</div>
<?php print $panel_suffix; ?>
