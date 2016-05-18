<?php
/**
 * @file
 * Theme the more link.
 *
 * - $view: The view object.
 * - $more_url: the url for the more link.
 * - $link_text: the text for the more link.
 *
 * @ingroup views_templates
 */
?>

<a href="<?php print $more_url ?>" title="<?php print $link_text; ?>" class="btn btn-primary pull-right">
    <?php print $link_text; ?>
</a>