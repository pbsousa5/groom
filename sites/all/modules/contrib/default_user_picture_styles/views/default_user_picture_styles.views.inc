<?php
/**
 * @file
 * Defines the field to Views.
 */

/**
 * Implements hook_views_data_alter().
 */
function default_user_picture_styles_views_data_alter(&$data) {
  $data['users']['picture'] = array(
    'group' => t('User'),
    'title' => t('Picture'),
    'help' => t("Renders the user default picture if found. Else it will display Drupal's user default picture."),
    'field' => array(
      'handler' => 'DUPStylesFieldHandler',
    ),
  );
}
