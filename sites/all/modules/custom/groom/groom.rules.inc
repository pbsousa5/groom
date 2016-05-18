<?php

function groom_rules_action_info()
{
    $actions['groom_manage_reservation'] = array(
        'label'     => t('Gestion des réservation'),
        'parameter' => array(
            'variables' => array(
                'type'  => 'commerce_order',
                'label' => t('Commande'),
            ),
            'status' => array(
                'type'         => 'text',
                'label'        => t('Statut des réservations après l\'événement'),
                'options list' => 'groom_rules_options_list_status',
            ),
            'line_item' => array(
                'type'          => 'commerce_line_item',
                'label'         => t('Produit panier'),
                'optional'      => true,
                'default value' => null,
                'allow null'    => true,
            ),
        ),
        'group' => t('Commerce Cart'),
        'callbacks' => array(
            'execute' => 'groom_manage_reservation',
        ),
    );

    $actions['groom_manage_vat'] = array(
        'label' => t('Set the VAT percentage'),
        'parameter' => array(
            'commerce_line_item' => array(
                'type' => 'commerce_line_item',
                'label' => t('Line item'),
            ),
            'quantity' => array(
                'label' => t('Quantity'),
                'type' => 'decimal',
            ),
            'price_table' => array(
                'label' => t('Price table'),
                'type' => 'list<commerce_price_table>',
            ),
            'component_name' => array(
                'type' => 'text',
                'label' => t('Price component type'),
                'description' => t('Price components track changes to prices made during the price calculation process, and they are carried over from the unit price to the total price of a line item. When an order total is calculated, it combines all the components of every line item on the order. When the unit price is altered by this action, the selected type of price component will be added to its data array and reflected in the order total display when it is formatted with components showing. Defaults to base price, which displays as the order Subtotal.'),
                'options list' => 'commerce_line_item_price_component_options_list',
                'default value' => 'tva',
            ),
        ),
        'group' => t('Commerce price table'),
        'callbacks' => array(
            'execute' => 'groom_manage_vat',
        ),
    );

    return $actions;
}


function groom_rules_options_list_status($rules_action, $field_name)
{
    return groom_get_status();
}

function groom_manage_vat($line_item, $quantity, $price_table, $component_name)
{
    if (!in_array($line_item->type, commerce_product_line_item_types())) {
        return;
    }

    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    $product           = entity_metadata_wrapper('commerce_product', $line_item_wrapper->commerce_product->value());
    $unit_price        = commerce_price_wrapper_value($line_item_wrapper, 'commerce_unit_price');

    if (empty($unit_price)) {
        return;
    }

    $table_price = commerce_price_table_get_amount_qty($product->raw(), $quantity, $price_table);

    if (empty($table_price)) {
        return;
    }

    $item_final_price = intval($table_price['amount']);
    $product_tva_rate = commerce_tax_rate_load(explode('|', $component_name)[1]);
    $vat_amount      = ceil($item_final_price - ($item_final_price / ($product_tva_rate['rate'] + 1)));

    $vat_price_new = array(
        'amount'        => $vat_amount,
        'currency_code' => $table_price['currency_code'],
        'data'          => array(
            'tax_rate' => $product_tva_rate,
        ),
    );

    $item_price_new = array(
        'amount'        => $item_final_price - $vat_amount,
        'currency_code' => $table_price['currency_code'],
        'data'          => array(),
    );

    $line_item_wrapper->commerce_unit_price->data = commerce_price_component_delete($line_item_wrapper->commerce_unit_price->value(), 'base_price');
    $line_item_wrapper->commerce_unit_price->data = commerce_price_component_delete($line_item_wrapper->commerce_unit_price->value(), $component_name);

    $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
        $line_item_wrapper->commerce_unit_price->value(),
        'base_price',
        $item_price_new,
        TRUE
    );

    $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
        $line_item_wrapper->commerce_unit_price->value(),
        $component_name,
        $vat_price_new,
        TRUE
    );
}

/**
 *
 * @param commerce_order $order
 * @param string         $status
 */
function groom_manage_reservation($order, $status, $line_item)
{
    if ($line_item !== null && property_exists($line_item, 'type') && $line_item->type === 'product')
    {
        groom_manage_reservation_status($line_item->data, $order, $status);
        return;
    }

    foreach ($order->commerce_line_items as $commerce_line_item)
    {
        foreach ($commerce_line_item as $l)
        {
            $line_item = commerce_line_item_load($l['line_item_id']);

            if (!$line_item) {
                continue;
            }

            groom_manage_reservation_status($line_item->data, $order, $status);
        }
    }
}

function groom_manage_reservation_status($data, $order, $status)
{
    if (array_key_exists('date', $data)
        && array_key_exists('slot_range', $data)
        && array_key_exists('room_id', $data)
        && array_key_exists('slot_type_id', $data)
    ) {
        if ($date = _groom_string_to_date($data['date'])) {
            $data['date'] = $date->format('Y-m-d');
        }

        switch ($status)
        {
            case GroomReservation::STATUS_PENDING:
                _groom_manage_reservation_pending($order, $data);
                break;

            case GroomReservation::STATUS_VALIDATED:
                _groom_manage_reservation_validated($data);
                break;

            case GroomReservation::STATUS_CANCELED:
                _groom_manage_reservation_canceled($data);
                break;
        }
    }
}

/**
 * @param commerce_order $order
 * @param array          $data
 * @return int
 */
function _groom_manage_reservation_pending($order, $data)
{
    $reservation = GroomReservation::findBy($data['date'], $data['slot_range'], $data['slot_type_id'], array(), $data['room_id']);
    $reservation = reset($reservation);

    if (empty($reservation))
    {
        $nid = db_insert('groom_reservation')
            ->fields(array(
                'uid'            => $order->uid,
                'date'           => $data['date'],
                'room'           => $data['room_id'],
                'time_slot'      => $data['slot_range'],
                'time_slot_type' => $data['slot_type_id'],
                'status'         => GroomReservation::STATUS_PENDING,
            ))
            ->execute();
    }
    else if ($reservation->getStatus() === GroomReservation::STATUS_CANCELED)
    {
        $reservation->setStatus(GroomReservation::STATUS_PENDING);
        $reservation->save();
    }
}

/**
 * @param array $data
 */
function _groom_manage_reservation_validated($data)
{
    $reservation = GroomReservation::findBy($data['date'], $data['slot_range'], $data['slot_type_id'], array(), $data['room_id']);
    $reservation = reset($reservation);

    if ($reservation)
    {
        $reservation->setStatus(GroomReservation::STATUS_VALIDATED);
        $reservation->save();
    }
}

/**
 * @param array $data
 */
function _groom_manage_reservation_canceled($data)
{
    $reservation = GroomReservation::findBy($data['date'], $data['slot_range'], $data['slot_type_id'], array(), $data['room_id']);
    $reservation = reset($reservation);

    if ($reservation)
    {
        $reservation->setStatus(GroomReservation::STATUS_CANCELED);
        $reservation->save();
    }
}


function _groom_string_to_date($date)
{
    $dateOutput = null;

    // If it's already a DateTime object
    if ($date instanceof DateTime) {
        $dateOutput = $date;
    }
    // If it's under 'Y-m-d' format
    else if (preg_match('/^[0-9]{4}-[0-9]{2}-[0-9]{2}$/', $date)) {
        $dateOutput = DateTime::createFromFormat('Y-m-d', $date);
    }
    // If it's under 'd/m/Y' format
    else if (preg_match('/^[0-9]{2}\/[0-9]{2}\/[0-9]{4}$/', $date)) {
        $dateOutput = DateTime::createFromFormat('d/m/Y', $date);
    }
    // If it's unkown format, tries to create with the string value
    else if (!empty($date)) {
        $dateOutput = new DateTime($date);
    }

    return $dateOutput;
}