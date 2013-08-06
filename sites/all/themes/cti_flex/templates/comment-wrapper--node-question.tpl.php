<?php
/**
 * @file
 * Zen theme's implementation to provide an HTML container for comments.
 *
 * Available variables:
 * - $content: The array of content-related elements for the node. Use
 *   render($content) to print them all, or print a subset such as
 *   render($content['comment_form']).
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default value has the following:
 *   - comment-wrapper: The current template type, i.e., "theming hook".
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * The following variables are provided for contextual information.
 * - $node: Node object the comments are attached to.
 * The constants below the variables show the possible values and should be
 * used for comparison.
 * - $display_mode
 *   - COMMENT_MODE_FLAT
 *   - COMMENT_MODE_THREADED
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess_comment_wrapper()
 * @see theme_comment_wrapper()
 */

 $comment_index = array_filter(element_children($content['comments']),'is_numeric');
 $num_comments = count($comment_index);
 if($num_comments > 3) {
	$comments = '';
	$visible_index = array_slice($comment_index,0,3,TRUE);
	$hidden_index = array_slice($comment_index,3,NULL,TRUE);
	foreach($visible_index as $index) {
		$comments .= render($content['comments'][$index]);
	}
	$comments .= '<span class="question-hidden-comments-label text-link">show '.($num_comments-3).' more...</span><div class="question-hidden-comments-wrapper">';	
	foreach($hidden_index as $index) {
		$comments .= render($content['comments'][$index]);
	}
	$comments .= '</div>';
 }
 else {
	$comments = render($content['comments']);
 }
$comment_form = render($content['comment_form']);
?>
<div id="comments" class="<?php print $classes; ?>"<?php print $attributes; ?>>


  <?php print $comments; ?>

  <?php if ($comment_form): ?>
  <div class="comment-form-wrapper">
	<span class="question-comment-add-trigger text-link">add comment</span>
    <?php print $comment_form; ?>
  </div>
  <?php endif; ?>
</div>
