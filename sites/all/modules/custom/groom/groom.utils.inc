<?php

function groom_get_status($status = null)
{
    $statuses = array(
        GroomReservation::STATUS_PENDING   => t('En attente'),
        GroomReservation::STATUS_VALIDATED => t('Validée'),
        GroomReservation::STATUS_CANCELED  => t('Annulée')
    );

    if ($status !== null && array_key_exists($status, $statuses)) {
        return $statuses[$status];
    }

    return $statuses;
}

function groom_get_user_fullname(EntityMetadataWrapper $user, $capitalize = false, $lastname_first = false)
{
    $user_first_name = $user->field_user_prenom->value();
    $user_last_name  = $user->field_user_nom->value();
    $fullname        = $user_last_name;

    if (isset($user_first_name) && !empty(trim($user_first_name)))
    {
        if ($lastname_first) {
            $fullname = $user_last_name.', '.$user_first_name;
        } else {
            $fullname = $user_first_name.' '.$user_last_name;
        }
    }

    if ($capitalize) {
        $fullname = ucwords(strtolower($fullname));
    }

    return $fullname;
}

function groom_export_user($user)
{
    $fields = field_info_instances('user', 'user');
    $output = array();
    $roles  = user_roles();

    $output['uid']      = $user->getIdentifier();
    $output['name']     = $user->name->value();
    $output['email']    = $user->mail->value();
    $output['roles']    = array_intersect_key($roles, array_flip($user->roles->value()));
    $output['status']   = $user->status->value();
    $output['language'] = $user->language->value();
    $output['fields']   = array();

    foreach ($fields as  $field_name => $field_infos)
    {
        $name = implode('', explode('field_', $field_name, 2));
        $output['fields'][$name] = $user->{$field_name}->value();
    }

    return drupal_json_encode($output);
}

/**
 * @see drupal_set_message()
 */
function groom_add_message($message = null, $type = 'status', $repeat = true)
{
    if ($message)
    {
        if (!isset($_SESSION['groom_messages'][$type])) {
            $_SESSION['groom_messages'][$type] = array();
        }

        if ($repeat || !in_array($message, $_SESSION['groom_messages'][$type])) {
            $_SESSION['groom_messages'][$type][] = $message;
        }

        // Mark this page as being uncacheable.
        drupal_page_is_cacheable(true);
    }

    // Messages not set when DB connection fails.
    return isset($_SESSION['groom_messages']) ? $_SESSION['groom_messages'] : null;
}

/**
 * @see drupal_get_messages()
 */
function groom_get_message($type = null, $clear_queue = true)
{
    if ($messages = groom_add_message())
    {
        if ($type)
        {
            if ($clear_queue) {
                unset($_SESSION['groom_messages'][$type]);
            }
            if (isset($messages[$type])) {
                return array($type => $messages[$type]);
            }
        }
        else
        {
            if ($clear_queue) {
                unset($_SESSION['groom_messages']);
            }
            return $messages;
        }
    }

    return array();
}

function groom_add_product_linked_to_reservation($reservation_line_item, $product_line_item)
{
    global $user;

    $product_line_item->data['groom_reservation_id'] = $reservation_line_item->data['groom_reservation_id'];

    return commerce_cart_product_add($user->uid, $product_line_item, false);
}

function groom_get_room_in_cart($output_type = 'line_item')
{
    global $user;

    $return_object = null;
    $room_in_cart  = null;
    $cart          = commerce_cart_order_load($user->uid);

    if (!$cart) {
        return $return_object;
    }

    $cart          = entity_metadata_wrapper('commerce_order', $cart);

    foreach (array_reverse($cart->commerce_line_items->value()) as $line_item)
    {
        $line_item = entity_metadata_wrapper('commerce_line_item', $line_item);
        $product   = $line_item->commerce_product;

        if ($product->type->value() === 'salle')
        {
            switch ($output_type)
            {
                case 'line_item':
                    $return_object = $line_item->value();
                    break;

                case 'product':
                    $return_object = $product->value();
                    break;
            }
            break;
        }
    }

    return $return_object;
}

function groom_get_last_in_cart($output_type = 'line_item', $product_type = null)
{
    global $user;

    $return_object = null;
    $room_in_cart  = null;
    $cart          = commerce_cart_order_load($user->uid);

    if (!$cart) {
        return $return_object;
    }

    $cart = entity_metadata_wrapper('commerce_order', $cart);

    foreach (array_reverse($cart->commerce_line_items->value()) as $line_item)
    {
        $line_item = entity_metadata_wrapper('commerce_line_item', $line_item);
        $product   = $line_item->commerce_product;

        if ($product->type->value() === $product_type)
        {
            switch ($output_type)
            {
                case 'line_item':
                    $return_object = $line_item->value();
                    break;

                case 'product':
                    $return_object = $product->value();
                    break;
            }
            break;
        }
    }

    return $return_object;
}


function groom_get_linked_products_to_reservation($reservation_id, $output_type = 'line_item', $output_wrapped = false)
{
    global $user;

    $linked_products = array();
    $cart            = commerce_cart_order_load($user->uid);

    if (!$cart) {
        return $linked_products;
    }

    $cart = entity_metadata_wrapper('commerce_order', $cart);

    foreach ($cart->commerce_line_items->value() as $line_item)
    {
        $line_item = entity_metadata_wrapper('commerce_line_item', $line_item);
        $product   = $line_item->commerce_product;

        if ($product->type->value() === 'product' && $line_item->value()->data['groom_reservation_id'] == $reservation_id)
        {
            switch ($output_type)
            {
                case 'line_item':
                    $linked_products[] = $line_item->value();
                    break;

                case 'product':
                    $linked_products[] = $product->value();
                    break;
            }
        }
    }

    if ($output_wrapped)
    {
        foreach ($linked_products as &$linked_product) {
            $linked_product = entity_metadata_wrapper('commerce_'.$output_type, $linked_product);
        }
    }

    return $linked_products;
}

function groom_utils_sum_reservation_quantity($reservations)
{
    $quantity = 0;

    foreach ($reservations as $reservation) {
        $quantity += $reservation->getQuantity();
    }

    return $quantity;
}
