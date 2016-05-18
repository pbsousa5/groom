<?php

function groom_admin_solo_form($form, &$form_state)
{
    $form['groom_solo_reservation_capacity'] = array(
        '#type'          => 'textfield',
        '#title'         => t('Capacité maximale de réservations Groom SOLO'),
        '#description'   => t('Nombre de réservations maximum possible pour les réservations SOLO.'),
        '#default_value' => variable_get('groom_solo_reservation_capacity', 20),
        '#size'          => 2,
        '#required'      => true,
    );

    $form['groom_solo_reservation_slot_type'] = array(
        '#type'          => 'select',
        '#title'         => t('Plage horaire SOLO'),
        '#description'   => t('Quel type de plage horaire est à utiliser pour les réservations SOLO'),
        '#default_value' => variable_get('groom_solo_reservation_slot_type', 103),
        '#options'       => array(),
        '#required'      => true,
    );

    $time_slot_types = entity_load('node', FALSE, array(
        'type' => 'time_slot_type',
    ));

    foreach ($time_slot_types as $nid => $time_slot_type) {
        $form['groom_solo_reservation_slot_type']['#options'][$nid] = $time_slot_type->title;
    }

    return system_settings_form($form);
}

function groom_admin_reservations_form($form, &$form_state) {
    $form['#attached']['css'][] = drupal_get_path('module', 'groom') . '/css/admin.css';

    // Attach filters on top of the page
    $form['filters'] = groom_admin_reservations_form_filters();

    // Attach mass actions on top ot the select table
//    $form['actions'] = groom_admin_reservation_form_actions();
    // Fill the table rows
    $form['table'] = groom_admin_reservations_form_table();

    // Prevent wrong paging when submitting new filter values
    $form['#action'] = url(current_path(), array('query' => drupal_get_query_parameters(null, array('q', 'page'))));

    $form['#submit'] = array('groom_admin_reservations_form_submit');

    return $form;
}

function groom_admin_reservations_form_filters()
{
    $time_slot_types = GroomSlotType::findAll();

    // Container
    $filters = array(
        '#type' => 'fieldset',
        '#title' => t('Filter'),
        '#attributes' => array(
            'id' => 'groom-reservations-filters'
        )
    );

    // Date
    $filters['date_from'] = array(
        '#type' => 'date_popup',
        '#title' => t('Par date'),
        '#default_value' => '',
        '#date_format' => 'd/m/Y',
    );

    /*
      TODO: Still not effective. Uncomment when developped
      $filters['date_to'] = array(
      '#type'          => 'date_popup',
      '#title'         => t('Au'),
      '#default_value' => '',
      '#date_type'     => DATE_DATETIME,
      '#date_format'   => 'd/m/Y',
      );
      // Status
      $filters['status'] = array(
      '#type'         => 'select',
      '#title'        => t('Statut'),
      '#multiple'     => true,
      '#empty_option' => '- ' . t('All') . ' -',
      '#options'      => groom_get_status(),
      );

      // Salle
      $filters['room'] = array(
      '#type'  => 'textfield',
      '#title' => t('Salle'),
      );

      // Type
      $filters['type'] = array(
      '#type'         => 'select',
      '#title'        => t('Type'),
      '#multiple'     => true,
      '#empty_option' => '- ' . t('All') . ' -',
      '#options'      => array()
      );

      // Timeslot
      $filters['time_slot'] = array(
      '#type'         => 'select',
      '#title'        => t('Créneau'),
      '#multiple'     => true,
      '#empty_option' => '- ' . t('All') . ' -',
      '#options'      => array()
      );

      // Adds time slot type options into "type" filter, and time slots otpion into "time_slot" filter
      foreach ($time_slot_types as $time_slot_type)
      {
      $filters['type']['#options'][$time_slot_type->getNid()] = $time_slot_type->getEntity()->title->value();

      foreach ($time_slot_type->getSlotRanges() as $day_of_week => $time_slot_times)
      {
      foreach ($time_slot_times as $time_slot_time)
      {
      if (!empty(trim($time_slot_time)) && !in_array($time_slot_time, $filters['status']['#options'])) {
      $filters['time_slot']['#options'][$time_slot_time] = $time_slot_time;
      }
      }
      }
      }
     */

    $filters['actions'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'id' => 'groom-filters-actions'
        ),
        'submit' => array(
            '#type' => 'submit',
            '#value' => t('Filter'),
        ),
        'reset' => array(
            '#type' => 'submit',
            '#value' => t('Reset'),
        ),
    );

    return $filters;
}

function groom_admin_reservations_form_submit($form, &$form_state)
{
    switch ($form_state['values']['op'])
    {
        case t('Filter'):
            $form_state['rebuild'] = true;
            break;

        case t('Reset'):
        case t('Update'):
            break;
    }
}

function groom_admin_reservations_form_actions()
{
    $actions = array(
        '#type'  => 'fieldset',
        '#title' => t('Actions sur la sélection'),
    );
    $actions['actions_list'] = array(
        '#type'    => 'select',
        '#options' => array(
            'delete'   => t('Supprimer'),
            'validate' => t('Valider'),
            'cancel'   => t('Annuler'),
            'pending'  => t('Mettre en attente'),
        ),
    );

    return $actions;
}

function groom_admin_reservations_form_table()
{
    module_load_include('inc', 'groom', 'groom.theme');

    $output        = array();
    $table_filters = _groom_admin_reservations_param_filters();
    $table_headers = _groom_admin_reservations_filters_values();
    $sort          = tablesort_get_sort($table_headers);
    $order         = tablesort_get_order($table_headers);
    $pager_limit   = 20;
    $pages_limit   = 10;
    $pager_page    = pager_find_page();
    $pager_offset  = $pager_page * $pager_limit;
    $date          = $table_filters['date_from'];

    if ($date !== null)
    {
        $date = DateTime::createFromFormat('d/m/Y h:i:s', $date . ' 00:00:00');
        $date = $date->format('Y-m-d');
    }

    $pager_total  = GroomReservation::countBy($date);
    $reservations = GroomReservation::findBy($date, null, 0, array(), null, 0, 'nid', $pager_offset, $pager_limit, array(
        $order['sql'] => $sort
    ));

    _groom_inject_views_ui();
    pager_default_initialize($pager_total, $pager_limit);

    $header = _groom_admin_reservations_filters_values();
    $rows   = _groom_admin_reservations_form_table_rows($reservations);

    // Reservations table list
    $output[] = array(
        '#type'       => 'tableselect',
        '#header'     => $header,
        '#options'    => $rows,
        '#js_select'  => true,
        '#empty'      => t('Aucunes réservations Groom.'),
        '#attributes' => array(
            'id'    => 'groom-reservations-list',
            'class' => array('groom-table', 'table-has-actions'),
        ),
    );

    // Table pager
    $output[] = array(
        '#theme'      => 'pager',
        '#prefix'     => '<div class="pager-wrap">',
        '#suffix'     => '</div>',
        '#element'    => 0,
        '#tags'       => array(),
        '#parameters' => array(),
        '#quantity'   => $pages_limit,
    );

    return $output;
}

function _groom_admin_reservations_param_filters()
{
    $post_params = drupal_get_query_parameters($_POST);
    $params      = array(
        'date_from' => null,
    );

    if (isset($post_params['op']) && $post_params['op'] === t('Filter'))
    {
        foreach ($params as $param_name => &$param_value)
        {
            if (!array_key_exists($param_name, $post_params)) {
                continue;
            }

            $param_value = $post_params[$param_name];
            switch ($param_name)
            {
                case 'date_from':
                    $param_value = $param_value['date'];
                    break;
            }
        }
    }

    return $params;
}

function _groom_admin_reservations_form_table_rows(array $reservations)
{
    $rows             = array();
    $status           = groom_get_status();
    $operations_links = array(
//        'view' => array(
//            'title' => t('Voir'),
//            'href'  => 'admin/groom/reservation/@id/@action',
//        ),
        'edit' => array(
            'title' => t('Modifier'),
            'href' => 'admin/groom/reservation/@id/@action',
        ),
//        'validate' => array(
//            'title' => t('Valider'),
//            'href'  => 'admin/groom/reservation/@id/@action',
//        ),
//        'pending' => array(
//            'title' => t('Mettre en attente'),
//            'href'  => 'admin/groom/reservation/@id/@action',
//        ),
//        'cancel' => array(
//            'title' => t('Annuler'),
//            'href'  => 'admin/groom/reservation/@id/@action',
//        ),
//        'delete' => array(
//            'title' => t('Supprimer'),
//            'href'  => 'admin/groom/reservation/@id/@action',
//        ),
    );

    foreach ($reservations as $reservation)
    {
        $first_name  = $reservation->getUser()->field_user_prenom->value();
        $last_name   = $reservation->getUser()->field_user_nom->value();
        $full_name   = $last_name;
        $room        = t('Open space SOLO');
        $row_actions = $operations_links;

        foreach ($operations_links as $operation_name => $operations_link)
        {
            $row_actions[$operation_name]['href'] = format_string($row_actions[$operation_name]['href'], array(
                '@id'     => $reservation->getNid(),
                '@action' => $operation_name,
            ));
        }

        if (isset($first_name) && !empty(trim($first_name))) {
            $full_name = $first_name . ' ' . $last_name;
        }

        if ($reservation->getRoom() !== null) {
            $room = $reservation->getRoom()->getEntity()->title->value();
        }

        $time_slot_type = '<em style="color: #BBB">' . t('- inexistant -') . '</em>';

        if ($reservation->getTimeSlotType() !== null) {
            $time_slot_type = $reservation->getTimeSlotType()->getEntity()->title->value();
        }

        $row = array(
            'date' => array(
                'data' => array(
                    '#markup' => $reservation->getDate()->format('d/m/Y'),
                ),
            ),
            'time_slot' => array(
                'data' => array(
                    '#markup' => $reservation->getTimeSlot(),
                ),
            ),
            'time_slot_type' => array(
                'data' => array(
                    '#markup' => $time_slot_type,
                ),
            ),
            'room' => array(
                'data' => array(
                    '#markup' => $room,
                ),
            ),
            'quantity' => array(
                'data' => array(
                    '#markup' => $reservation->getQuantity(),
                ),
            ),
            'status' => array(
                'data' => array(
                    '#markup' => $status[$reservation->getStatus()],
                ),
                'class' => array('status-' . strtolower($reservation->getStatus()))
            ),
            'name' => array(
                'data' => array(
                    '#markup' => $full_name,
                ),
            ),
            'email' => array(
                'data' => array(
                    '#markup' => l($reservation->getUser()->mail->value(), 'mailto:' . $reservation->getUser()->mail->value()),
                ),
            ),
            'company' => array(
                'data' => array(
                    '#markup' => $reservation->getUser()->field_user_societe->value(),
                ),
            ),
            'actions' => array(
                'data' => array(
                    '#theme' => 'links__ctools_dropbutton',
                    '#links' => $row_actions,
                ),
            ),
        );

        $rows[$reservation->getNid()] = _groom_admin_table_add_classes($row);
    }

    return $rows;
}

function groom_admin_reservation_form_content($form, &$form_state, $operation, $reservation = null)
{
    groom_admin_reservation_form($form, $form_state);

    switch ($operation)
    {
        case 'add':
            groom_admin_reservation_add_form($form, $form_state);
            break;

        case 'edit':
            groom_admin_reservation_edit_form($form, $form_state, $reservation);
            break;
    }

    return $form;
}

function groom_admin_reservation_form(&$form, &$form_state)
{
    $now             = new \DateTime();
    $rooms           = GroomRoom::findAll();
    $time_slot_types = GroomSlotType::findAll();

    $form['#attached']['css'][] = drupal_get_path('module', 'groom') . '/css/admin.css';
    $form['#attached']['js'][]  = drupal_get_path('module', 'groom') . '/js/admin.js';

    // Operation type (add, edit)
    $form['operation'] = array(
        '#access' => false,
        '#type'   => 'hidden',
    );

    // Groom reservation id
    $form['groom_reservation_id'] = array(
        '#type' => 'hidden',
    );

    // Date
    $form['date'] = array(
        '#type'               => 'date_popup',
        '#title'              => t('Date'),
        '#default_value'      => $now->format('Y-m-d'),
        '#date_format'        => 'd/m/Y',
        '#required'           => true,
        '#datepicker_options' => array(
            'firstDay' => 1,
        ),
    );

    // Status
    $form['status'] = array(
        '#type'          => 'select',
        '#title'         => t('Statut'),
        '#options'       => groom_get_status(),
        '#default_value' => GroomReservation::STATUS_PENDING,
        '#required'      => true,
    );

    // Time slot type
    $form['time_slot_type'] = array(
        '#type'          => 'select',
        '#title'         => t('Type'),
        '#options'       => array(),
        '#default_value' => null,
        '#required'      => true,
    );

    // Room
    $form['room'] = array(
        '#type'     => 'select',
        '#title'    => t('Salle'),
        '#options'  => array('Solo' => array(t('Open space SOLO'))),
        '#required' => true,
    );

    foreach ($rooms as $room)
    {
        $room_type = $room->getEntity()->field_room_time_slot_type;
        $form['room']['#options'][$room_type->label()][$room->getNid()] = $room->getEntity()->label();
    }

    // Time slot
    $form['time_slot'] = array(
        '#type'          => 'select',
        '#title'         => t('Créneau'),
        '#default_value' => null,
        '#options'       => array(),
        '#required'      => true,
    );

    // Quantity
    $form['quantity'] = array(
        '#type'           => 'groom_input_number',
        '#title'          => t('Places'),
        '#theme'          => 'groom_input_number',
        '#theme_wrappers' => array('form_element'),
        '#default_value'  => 1,
        '#min'            => 1,
        '#max'            => variable_get('groom_solo_reservation_capacity', 23),
        '#required'       => true,
        '#input'          => true,
    );

    // User
    $form['user']['autocomplete'] = array(
        '#type'              => 'textfield',
        '#autocomplete_path' => 'admin/groom/autocomplete/user',
        '#title'             => t('Membre'),
        '#size'              => 40,
        '#name'              => 'user-ac-input',
        '#pre_render'        => array('groom_admin_reservation_form_user_pre_render'),
        '#post_render'       => array('groom_admin_reservation_form_user_post_render'),
        '#attributes'        => array(
            'placeholder' => t('Entrez le nom d\'un membre'),
        ),
    );

    $form['user']['user_id'] = array(
        '#type'             => 'hidden',
        '#element_validate' => array('groom_admin_reservation_form_user_validate'),
    );

    $form['user']['infos'] = array(
        '#markup' => '<div class="user-infos">'
        . '<div class="user-name"><label>' . t('Nom') . ' : </label><span>-</span></div>'
        . '<div class="user-company"><label>' . t('Société') . ' : </label><span>-</span></div>'
        . '<div class="user-email"><label>' . t('Email') . ' : </label><span>-</span></div>'
        . '<br />',
    );

    // Adds time slot type options into "type" filter, and time slots otpion into "time_slot" filter
    foreach ($time_slot_types as $nid => $time_slot_type)
    {
        $form['time_slot_type']['#options'][$nid] = $time_slot_type->getEntity()->title->value();
        $slot_type_name                           = $time_slot_type->getEntity()->title->value();

        foreach ($time_slot_type->getSlotRanges() as $day_of_week => $time_slot_times)
        {
            foreach ($time_slot_times as $time_slot_time)
            {
                if (!empty(trim($time_slot_time)) && !in_array($time_slot_time, $form['status']['#options'])) {
                    $form['time_slot']['#options'][$slot_type_name][$time_slot_time] = $time_slot_time;
                }
            }
        }
    }

    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
    );

    $form['#submit'] = array('groom_admin_reservation_form_submit');
}

function groom_admin_reservation_form_user_pre_render($element)
{
    $element['#required'] = true;
    return $element;
}

function groom_admin_reservation_form_user_post_render($element, &$children)
{
    $children['#required'] = false;
    return $element;
}

function groom_admin_reservation_form_user_validate($element, &$form_state, $form)
{
    $is_empty_string = (is_string($element['#value']) && drupal_strlen(trim($element['#value'])) == 0);
    $is_empty_value  = ($element['#value'] === 0);
    if (!is_numeric($element['#value']) || $is_empty_string || $is_empty_value) {
        form_error($form['user']['autocomplete'], t(
            '!name field is required.',
            array(
                '!name' => $form['user']['autocomplete']['#title']
            )
        ));
    }
    else
    {
        $user      = entity_metadata_wrapper('user', user_load($element['#value']));
        $user_json = groom_export_user($user);
        drupal_add_js('var groom = groom || {};'
            . 'groom.users = groom.users || {};'
            . 'groom.users[' . $element['#value'] . '] = ' . $user_json . ';',
            array(
                'type'       => 'inline',
                'cache'      => false,
                'preprocess' => false,
            ));
    }
}

function groom_admin_reservation_add_form(&$form, &$form_state)
{
    $form['#attributes']['class'][]      = 'form-add';
    $form['operation']['#default_value'] = 'add';

    unset($form['groom_reservation_id']);
}

function groom_admin_reservation_edit_form(&$form, &$form_state, $reservation)
{
    if ($reservation === null) {
        drupal_not_found();
        exit();
    }

    $user_full_name = groom_get_user_fullname($reservation->getUser(), true);
    $time_slot_type = '';

    if ($reservation->getTimeSlotType() !== null) {
        $time_slot_type = $reservation->getTimeSlotType()->getEntity()->title->value();
    }

    // Title
    drupal_set_title(t('Modification de la réservation %type du %date pour %time_slot (%user)', array(
        '%type' => $time_slot_type,
        '%date' => $reservation->getDate()->format('d/m/Y'),
        '%time_slot' => $reservation->getTimeSlot(),
        '%user' => $user_full_name,
            )), PASS_THROUGH);

    // Operation type
    $form['#attributes']['class'][] = 'form-edit';
    $form['operation']['#default_value'] = 'edit';

    // Groom reservation id
    $form['groom_reservation_id']['#default_value'] = $reservation->getNid();

    // Date
    $form['date']['#default_value'] = $reservation->getDate()->format('Y-m-d');

    // Status
    $form['status']['#default_value'] = $reservation->getStatus();

    // Time slot type
    $form['time_slot_type']['#default_value'] = $time_slot_type !== '' ? $reservation->getTimeSlotType()->getNid() : null;

    // Room
    if ($reservation->getRoom() !== null) {
        $form['room']['#default_value'] = $reservation->getRoom()->getNid();
    } else {
        $form['room']['#default_value'] = 0;
    }

    // Time slot
    $form['time_slot']['#default_value'] = $reservation->getTimeSlot();

    // Quantity
    $form['quantity']['#default_value'] = $reservation->getQuantity();

    // User
    $form['user']['user_id']['#default_value'] = $reservation->getUser()->getIdentifier();
    $form['user']['infos']['#markup'] = '<div class="user-infos">'
            . '<div class="user-name"><label>' . t('Nom') . ' : </label><span>' . $user_full_name . '</span></div>'
            . '<div class="user-company"><label>' . t('Société') . ' : </label><span>' . $reservation->getUser()->field_user_societe->value() . '</span></div>'
            . '<div class="user-email"><label>' . t('Email') . ' : </label><span>' . l($reservation->getUser()->mail->value(), 'mailto:' . $reservation->getUser()->mail->value()) . '</span></div>'
            . '<br />';
}

function groom_admin_reservation_form_submit(&$form, &$form_state)
{
    $nid = null;
    if (array_key_exists('groom_reservation_id', $form_state['input'])) {
        $nid = intval($form_state['input']['groom_reservation_id']);
    }

    $date           = $form_state['input']['date']['date'];
    $status         = $form_state['input']['status'];
    $room           = intval($form_state['input']['room']);
    $time_slot_type = intval($form_state['input']['time_slot_type']);
    $time_slot      = $form_state['input']['time_slot'];
    $uid            = intval($form_state['input']['user_id']);
    $quantity       = intval($form_state['input']['quantity']);
    $user           = entity_metadata_wrapper('user', user_load($uid));
    $time_slot_type = GroomSlotType::get($time_slot_type);
    $date           = DateTime::createFromFormat('d/m/Y h:i:s', $date . ' 00:00:00');
    $room           = GroomRoom::get($room);
    $saved          = false;
    $errors         = array();
    $reservation    = new GroomReservation($nid, $user, $date, $room, $time_slot, $time_slot_type, $quantity, $status);

    try
    {
        $saved           = $reservation->save();
        $from['rebuild'] = false;
    }
    catch (PDOException $e)
    {
        // Error on unicity constraint
        if ($e->getCode() == 23000) {
            $errors[] = t('La réservation n\'a pu être enregistrée, il en existe déjà une avec ces paramètres');
        }
    }
    catch (Exception $e) {

    }

    $transaction = array(
        'status'    => $saved ? 'ok' : 'ko',
        'operation' => $form_state['values']['operation'],
        'errors'    => $errors
    );

    if (!$saved) {
        $form_state['rebuild'] = true;
    }

    groom_admin_reservation_form_submit_messages($transaction, $reservation);
}

function groom_admin_reservation_form_submit_messages($transaction, $reservation)
{
    switch ($transaction['operation'])
    {
        case 'add':
            if ($transaction['status'] == 'ok')
            {
                drupal_set_message(t('La réservation a bien été enregistrée'));
                drupal_goto('admin/groom/reservation/' . $reservation->getNid() . '/edit');
            }
            else
            {
                if (!empty($transaction['errors']))
                {
                    foreach ($transaction['errors'] as $name => $message) {
                        drupal_set_message($message, 'error');
                    }
                }
                else {
                    drupal_set_message(t('La réservation n\'a pas pu être enregistrée'), 'error');
                }
            }
            break;

        case 'edit':
            if ($transaction['status'] == 'ok') {
                drupal_set_message(t('La réservation a bien été modifiée'));
            } else {
                drupal_set_message(t('La réservation n\'a pas pu être enregistrée'), 'error');
            }
            break;
    }
}

function groom_admin_reservation_status_form($form, &$form_states)
{
    return $form;
}

function _groom_admin_autotomplete_user($search)
{
    $matches = array();
    $query   = db_select('users', 'u')
        ->fields('u', array('uid'));
    $query->leftJoin(
        'field_data_field_user_nom',
        'field_nom',
        'field_nom.entity_id = u.uid AND field_nom.entity_type = :user AND field_nom.bundle = :user',
        array(':user' => 'user')
    );
    $query->leftJoin(
        'field_data_field_user_prenom',
        'field_prenom',
        'field_prenom.entity_id = u.uid AND field_prenom.entity_type = :user AND field_prenom.bundle = :user',
        array(':user' => 'user')
    );

    $or = db_or();
    $or->condition('field_nom.field_user_nom_value', '%' . db_like($search) . '%', 'LIKE');
    $or->condition('field_prenom.field_user_prenom_value', '%' . db_like($search) . '%', 'LIKE');

    if (is_numeric($search)) {
        $or->condition('u.uid', $search, '=');
    }

    $query->condition($or);
    $query->orderBy('field_nom.field_user_nom_value', 'ASC');

    $result = $query->execute();

    if ($result)
    {
        $uids  = $result->fetchCol();
        $users = entity_load('user', $uids);

        foreach ($users as $uid => $user)
        {
            $entity        = entity_metadata_wrapper('user', $user);
            $matches[$uid] = array(
                'uid'        => $uid,
                'first_name' => $entity->field_user_prenom->value(),
                'last_name'  => $entity->field_user_nom->value(),
                'company'    => $entity->field_user_societe->value(),
                'email'      => $entity->mail->value(),
            );
        }
    }

    drupal_json_output($matches);
}
