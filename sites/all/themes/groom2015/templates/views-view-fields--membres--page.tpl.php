<?php
/**
 * @file
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->wrapper_prefix: A complete wrapper containing the inline_html to use.
 *   - $field->wrapper_suffix: The closing tag for the wrapper.
 *   - $field->separator: an optional separator that may appear before a field.
 *   - $field->label: The wrap label text to use.
 *   - $field->label_html: The full HTML of the label to use including
 *     configured element type.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */
if (!function_exists("display_project_field")) {

    function display_project_field($fields, $field_name) {
        if (!isset($fields[$field_name])) {
            return;
        }
        $field = $fields[$field_name];

        if (!empty($field->separator)) {
            print $field->separator;
        }

        print $field->wrapper_prefix;
        print $field->label_html;
        print $field->content;
        print $field->wrapper_suffix;
    }

}

?>

<div class="col-md-4 image">

    <?php display_project_field($fields, "picture"); ?>

</div>

<div class="col-md-8">

    <h4><?php display_project_field($fields, "field_user_prenom"); ?> <?php display_project_field($fields, "field_user_nom"); ?> <span><?php display_project_field($fields, "field_user_societe"); ?></span></h4>

    <?php display_project_field($fields, "field_user_visibilite_coor"); ?>
    <?php display_project_field($fields, "field_user_visibilite_bio"); ?>

</div>

<div class="col-md-12 buttons cf">
    <?php display_project_field($fields, "view_user"); ?>
    <?php display_project_field($fields, "edit_node"); ?>
    <?php display_project_field($fields, "cancel_node"); ?>
</div>