<?php

/**
 * @file
 * Theme implementation to display information about a given user.
 
 * Since the user is the author of a post or of a profile page, the user is
 * referred to as "author" below.
 
 * Available variables (core modules):
 * - $account: The entire user object for the author.
 * - $picture: Themed user picture for the author. 
 *   See author-pane-user-picture.tpl.php.
 * - $account_name: Themed user name for the author.
 * - $account_id: User ID number for the author.
 *
 * - $joined: Date the post author joined the site. (Uses shortdate format)
 * - $joined_ago: Time since the author registered in the format "TIME ago"
 *
 * - $online_status_class: "author-offline" or "author-online".
 * - $online_status: Translated text "Online" or "Offline"
 * - $last_active: Time since author was last active. eg: "5 days 3 hours"
 *
 * - $contact: Linked translated text "Contact user".
 *
 * - $profile - Profile object from core Profile module.
 *     Usage: $profile['category']['field_name']['#value']
 *     Example: <?php print $profile['Personal info']['profile_name']['#value']; ?>
 
 * Available variables (contributed modules):
 * - $facebook_status: Status, including username, from the Facebook-style 
 *   Statuses module.
 * - $facebook_status_status: Status from the Facebook-style Statuses module.
 *
 * - $privatemsg: Linked translated text "Send private message" provided by
 *   the Privatemsg module.
 *
 * - $user_badges: Badges from User Badges module.
 *
 * - $userpoints_points: Author's total number of points from all categories.
 * - $userpoints_categories: Array holding each category and the points for 
 *   that category. Both provided by the User Points module.
 *
 * - $user_stats_posts: Number of posts from the User Stats module.
 * - $user_stats_ip: IP address from the User Stats module.
 *
 * - $user_title: Title from the User Titles module.
 * - $user_title_image: Image version of title from User Titles module. This is
 *   not shown by default. If you want to show images instead of titles, change
 *   all instances of the variable in the code below.
 *
 * - $og_groups: Linked list of Organic Groups that the author is a member of.
 *
 * - $location_user_location: User location as reported by the Location module.
 *
 * - $fasttoggle_block_author: Link to toggle the author blocked/unblocked.
 *
 * - $troll_ban_author: Link to ban author via the Troll module.

 * Not working as of this writing but kept for future compatability:
 * - $user_relationships_api: Linked text "Add to <relationship>" or 
 *   "Remove from <relationship>".
 * - $flag_friend: Linked text. Actual text depends on module settings.

 */
?>

<?php
  // This bit of debugging info will show the full path to and name of this
  // template file to make it easier to figure out which template is
  // controlling which author pane.
  if (!empty($show_template_location)) {
    print __FILE__;
  }
?>

<div class="author-pane">

	<?php if(in_array('Jaspersoft Staff', array_values($account->roles))): ?>
	<div class="author-pane-staff"></div>
	<?php endif; ?>

 <div class="author-pane-inner">
    <?php /* General section */ ?>
	
    <div class="author-pane-section author-pane-general">
      
	  <?php /* User picture / avatar (has div in variable) */ ?>
      <?php if (!empty($picture)): ?>
        <?php print $picture; ?>
      <?php endif; ?>
	  
      <?php /* Account name and points */ ?>
	  <div class="author-pane-name-wrapper">
      <div class="author-pane-line author-name">
        <?php print $account_name; ?>
      </div>
      <?php if (isset($userpoints_points)): ?>
        <div class="author-pane-line author-points">
          <?php print $userpoints_points; ?>
        </div>
      <?php endif; ?>
	  </div>
      
	  <?php /* Joined */ ?>
      <?php if (!empty($joined)): ?>
        <div class="author-pane-line author-joined">
          <span class="author-pane-label"><?php print t('Joined'); ?>:</span> <?php print $joined; ?>
        </div>
      <?php endif; ?>
	  
      <?php /* Last active */ ?>
      <?php if (!empty($last_active)): ?>
        <div class="author-pane-line author-active">
           <span class="author-pane-label"><?php print t('Last seen'); ?>:</span> <?php print t('!time ago', array('!time' => $last_active)); ?>
        </div>
      <?php endif; ?>


      
    </div>
	
  </div>
</div>
