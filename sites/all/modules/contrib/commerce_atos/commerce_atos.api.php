<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Allows modules to alter the request sent to atos.
 *
 * You can send additional settings like templates block order background etc.
 */
function hook_commerce_atos_request_alter(&$settings) {
  // Example code.
  $settings['templatefile'] = 'foo';
}
