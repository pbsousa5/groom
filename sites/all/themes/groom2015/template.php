<?php

/**
 * @file
 * template.php
 */

/**
 * Drupal commerce quantity field (input text to select box)
 */
function groom2015_form_commerce_cart_add_to_cart_form_alter(&$form, &$form_state, $form_id) {
    if (strpos($form_id, 'commerce_cart_add_to_cart_form_') === 0) {
        if (isset($form['quantity']) && ($form['quantity']['#type'] == 'textfield')) {
            $form['quantity']['#title'] = t('Indiquez la quantité');
        }
    }
}

/**
 * User login template
 */
function groom2015_theme() {
    $items = array();
    // create custom user-login.tpl.php
    $items['user_login'] = array(
        'render element' => 'form',
        'path' => drupal_get_path('theme', 'groom2015') . '/templates',
        'template' => 'user-login',
        'preprocess functions' => array(
            'groom2015_preprocess_user_login'
        ),
    );
    return $items;
}

/**
 * Change class of submit button login
 */
function groom2015_form_alter(&$form, &$form_state, $form_id) {
    if ($form_id = 'user_login') {
        $form['actions']['submit']['#attributes']['class'][] = 'btn-primary';
    }
}

/**
 * Custom template for the frontpage
 */
function groom2015_preprocess_node(&$vars) {
    if ($vars["is_front"]) {
        $vars["theme_hook_suggestions"][] = "node__front";
    }
}

function groom2015_preprocess_html(&$variables)
{
    $bootstrap_select_lib = libraries_get_path('bootstrap-select');
    if ($bootstrap_select_lib)
    {
        drupal_add_js($bootstrap_select_lib.'/js/bootstrap-select.min.js');
        drupal_add_js($bootstrap_select_lib.'/js/i18n/defaults-fr_FR.min.js');
        drupal_add_css($bootstrap_select_lib.'/css/bootstrap-select.min.css');
    }
}

/**
 * Change label commerce line item summary
 */
function groom2015_preprocess_commerce_line_item_summary(&$variables) {
    $variables['total_label'] = t('Commande :');
}

/**
 * Implements hook_field_widget_WIDGET_TYPE_form_alter().
 */
function groom2015_field_widget_addressfield_standard_form_alter(&$element, &$form_state, $context) {
    $element['#type'] = 'container';
}

/**
 * Remove password strength
 */
function groom2015_element_info_alter(&$types) {
    if (isset($types['password_confirm']['#process']) && (($position = array_search('user_form_process_password_confirm', $types['password_confirm']['#process'])) !== FALSE)) {
        unset($types['password_confirm']['#process'][$position]);
    }
}

/*
 * Add primary class on btn commerce
 */

function groom2015_form_views_form_commerce_cart_form_default_alter(&$form, &$form_state, $form_id) {
    $form['actions']['checkout']['#attributes']['class'] = array('btn-primary');
}

/*
 * Change label Order total
 */

function groom2015_commerce_price_formatted_components_alter(&$components, $price, $entity) {
    $components['commerce_price_formatted_amount']['title'] = t('Total TTC');
}

/*
 * Disallow users to edit Quantity field on reservation product
 */

function groom2015_quantity_available_for_row($row) {
    ($line_item = $row->commerce_line_item_field_data_commerce_line_items_line_item_) && ($line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item)) && ($product = $line_item_wrapper->commerce_product->value());

    if ($product) {
        switch ($product->type) {
            case 'salle':
                return FALSE; // Disallow users to edit Quantity field.
                break;
        }
    }
    return TRUE; // Allow users to edit Quantity field.
}

function groom2015_select($variables)
{
    $element = $variables['element'];
    element_set_attributes($element, array('id', 'name', 'size'));
    _form_set_class($element, array('form-select'));

    return '<select' . drupal_attributes($element['#attributes']) . '>' . groom2015_form_select_options($element) . '</select>';
}


function groom2015_form_select_options($element)
{
    if (!isset($choices)) {
        $choices = $element['#options'];
    }

    $value_valid    = isset($element['#value']) || array_key_exists('#value', $element);
    $value_is_array = $value_valid && is_array($element['#value']);
    $options        = '';

    foreach ($choices as $key => $choice)
    {
        if (is_array($choice) && !array_key_exists('#has_attr', $choice))
        {
            $options .= '<optgroup label="' . check_plain($key) . '">';
            $options .= form_select_options($element, $choice);
            $options .= '</optgroup>';
        }
        else if (is_array($choice) && array_key_exists('#has_attr', $choice))
        {
            $key        = (string) $key;
            $selected   = '';
            $attributes = array();

            if (array_key_exists('#attributes', $choice)) {
                $attributes = $choice['#attributes'];
            }

            if ($value_valid && (!$value_is_array && (string) $element['#value'] === $key || ($value_is_array && in_array($key, $element['#value'])))) {
                $selected = ' selected="selected"';
            }
            $options .= '<option value="' . check_plain($key) . '"' . $selected . ' ' . drupal_attributes($attributes) .'>'
                . check_plain($choice['#title'])
                . '</option>';
        }
        elseif (is_object($choice)) {
            $options .= form_select_options($element, $choice->option);
        }
        else
        {
            $key = (string) $key;
            if ($value_valid && (!$value_is_array && (string) $element['#value'] === $key || ($value_is_array && in_array($key, $element['#value'])))) {
                $selected = ' selected="selected"';
            }
            else {
                $selected = '';
            }
            $options .= '<option value="' . check_plain($key) . '"' . $selected . '>' . check_plain($choice) . '</option>';
        }
    }
    return $options;
}

function groom2015_status_messages($variables)
{
    groom_add_groom_messages();
    return bootstrap_status_messages($variables);
}