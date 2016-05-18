<?php
/**
 * Used to define the calendar block.
 */

/**
 *
 */
function groom_block_info()
{
    $blocks['groom_calendar_standard'] = array(
        'info' => t('Calendrier Groom - Standard'),
    );
    $blocks['groom_calendar_solo'] = array(
        'info' => t('Calendrier Groom - Solo'),
    );
    $blocks['groom_calendar_vip'] = array(
        'info' => t('Calendrier Groom - VIP'),
    );

    return $blocks;
}

/**
 *
 */
function groom_block_configure($delta = '')
{
    // Get time slot types
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'time_slot_type')
        ->propertyCondition('status', NODE_PUBLISHED);

    $result = $query->execute();

    if (isset($result['node'])) {
        $resa_nids = array_keys($result['node']);
        $resa_items = entity_load('node', $resa_nids);
    }

    foreach($resa_items as $res) {
        $options[$res->nid] = $res->title;
    }

    // Form
    $form = array();
    switch ($delta) {
        case 'groom_calendar_standard':
            $default_value = variable_get('groom_block_calendar_standard_time_slot_type');
            break;
        case 'groom_calendar_solo':
            $default_value = variable_get('groom_block_calendar_solo_time_slot_type');
            break;
        case 'groom_calendar_vip':
            $default_value = variable_get('groom_block_calendar_vip_time_slot_type');
            break;
    }

    $form['groom_block_time_slot_type'] = array(
        '#type'          => 'select',
        '#title'         => t('Type de calendrier'),
        '#options'       => $options,
        '#default_value' => $default_value,
        '#description'   => t('Choisissez le type de calendrier.'),
    );

    return $form;
}

/**
 *
 */
function groom_block_save($delta = '', $edit = array())
{
    switch ($delta) {
        case 'groom_calendar_standard':
            variable_set('groom_block_calendar_standard_time_slot_type', $edit['groom_block_time_slot_type']);
            break;
        case 'groom_calendar_solo':
            variable_set('groom_block_calendar_solo_time_slot_type', $edit['groom_block_time_slot_type']);
            break;
        case 'groom_calendar_vip':
            variable_set('groom_block_calendar_vip_time_slot_type', $edit['groom_block_time_slot_type']);
            break;
    }
}

/**
 *
 */
function groom_block_view($delta = '')
{
    $block = array();
    switch ($delta) {
        case 'groom_calendar_standard':
            $block['subject'] = t('<none>');
            $block['content'] = _groom_block_build_calendar($delta);
            break;
        case 'groom_calendar_solo':
            $block['subject'] = t('<none>');
            $block['content'] = _groom_block_build_calendar($delta);
            break;
        case 'groom_calendar_vip':
            $block['subject'] = t('<none>');
            $block['content'] = _groom_block_build_calendar($delta);
            break;
    }

    return $block;
}

/**
 *
 */
function _groom_block_build_calendar($delta)
{
    drupal_add_js('misc/ajax.js');
    drupal_add_js('sites/all/themes/bootstrap/js/misc/ajax.js');
    drupal_add_js(drupal_get_path('module', 'groom').'/js/groom.js');
    drupal_add_css(drupal_get_path('module', 'groom') . '/css/groom.css', array(
        'group'      => CSS_DEFAULT,
        'every_page' => false
    ));

    switch ($delta) {
        case 'groom_calendar_standard':
            $time_slot_type = variable_get('groom_block_calendar_standard_time_slot_type');
            break;
        case 'groom_calendar_solo':
            $time_slot_type = variable_get('groom_block_calendar_solo_time_slot_type');
            break;
        case 'groom_calendar_vip':
            $time_slot_type = variable_get('groom_block_calendar_vip_time_slot_type');
            break;
    }

    $reservations = array();
    foreach (GroomReservation::findBy(null, null, $time_slot_type, array('pending', 'validated')) as $reservation)
    {
        $time_slot = $reservation->getTimeSlot();
        $date      = $reservation->getDate()->format('Y-m-d');

        $reservations[$date][$time_slot][] = $reservation;
    }

    $rooms = GroomRoom::findByTimeSlotTypeId($time_slot_type);

    return theme('groom_calendar', array(
        'time_slot_type' => $time_slot_type,
        'reservations'   => $reservations,
        'rooms'          => $rooms,
        'display_date'   => _groom_get_reference_date()
    ));
}

/**
 * Retrieve date in order to display calendar according to that.
 * Can override the display for differents months and year by setting GET parameter "month-display".<br>
 * Example: month-display=2015-06
 *
 * @return DateTime Date to show for current month display
 */
function _groom_get_reference_date()
{
    $date       = new DateTime();
    $get_params = drupal_get_query_parameters();

    if (!empty($get_params['month-display']) && preg_match('/^[0-9]{4}-[0-9]{2}$/', $get_params['month-display'])) {
        $date  = new DateTime($get_params['month-display']);
    }

    return $date;
}
