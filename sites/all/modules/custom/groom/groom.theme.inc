<?php

function _groom_inject_views_ui()
{
    if (module_exists('views_ui'))
    {
        if (module_load_include('inc', 'views_ui', 'includes/admin'))
        {
            foreach (views_ui_get_admin_css() as $file => $options) {
                drupal_add_css($file, $options);
            }
        }
    }
}

function _groom_admin_table_add_classes($line)
{
    $count = 0;
    foreach ($line as $column_name => &$column)
    {
        $count++;
        $class = array();

        $class[] = 'col-'.$count;
        $class[] = 'col-'.$column_name;

        if ($count === 1) {
            $class[] = 'col-first';
        }

        if ($count === count($line)) {
            $class[] = 'col-last';
        }

        if (!isset($column['class'])) {
            $column['class'] = $class;
        } else {
            $column['class'] = array_merge($class, $column['class']);
        }
    }

    return $line;
}

function _groom_admin_reservations_filters_values()
{
    $headers = array(
        'date' => array(
            'data'  => t('Date'),
            'field' => 'date',
            'sort'  => 'desc',
        ),
        'time_slot' => array(
            'data' => t('Créneau'),
        ),
        'time_slot_type' => array(
            'data' => t('Type'),
        ),
        'room' => array(
            'data' => t('Salle'),
        ),
        'quantity' => array(
            'data' => t('Places'),
        ),
        'status' => array(
            'data' => t('Statut'),
        ),
        'name' => array(
            'data' => t('Nom'),
        ),
        'email' => array(
            'data' => t('Email'),
        ),
        'company' => array(
            'data' => t('Société'),
        ),
        'actions' => array(
            'data' => t('Actions'),
        ),
    );

    return _groom_admin_table_add_classes($headers);
}

function theme_groom_admin_reservations_table($variables)
{
    $output       = '';
    $reservations = $variables['reservations'];
    $pager_total  = $variables['pager_total'];
    $pager_limit  = $variables['pager_limit'];
    $header       = _groom_admin_reservations_filters_values();
    $rows         = array();
    $rows_inputs  = array();
    $status       = groom_get_status();

    _groom_inject_views_ui();
    pager_default_initialize($pager_total, $pager_limit);

    foreach ($reservations as $reservation)
    {
        $full_name = groom_get_user_fullname($reservation->getUser());;
        $room      = t('Open space SOLO');

        if ($reservation->getRoom() !== null) {
            $room = $reservation->getRoom()->getEntity()->title->value();
        }

        $actions_content = theme('links__ctools_dropbutton', array(
            'links' => array(
                array(
                    'title'    => t('Voir'),
                    'href'     => '#',
                    'external' => true,
                ),
                array(
                    'title'    => t('Modifier'),
                    'href'     => '#',
                    'external' => true,
                ),
                array(
                    'title'    => t('Supprimer'),
                    'href'     => '#',
                    'external' => true,
                ),
            ),
        ));

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
                    '#markup' => $reservation->getTimeSlotType()->getEntity()->title->value(),
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
                'data'  => array(
                    '#markup' => $status[$reservation->getStatus()],
                ),
                'class' => array('status-'.strtolower($reservation->getStatus()))
            ),
            'name' => array(
                'data' => array(
                    '#markup' => $full_name,
                ),
            ),
            'email' => array(
                'data' => array(
                    '#markup' => l($reservation->getUser()->mail->value(), 'mailto:'.$reservation->getUser()->mail->value()),
                ),
            ),
            'company' => array(
                'data' => array(
                    '#markup' => $reservation->getUser()->field_user_societe->value(),
                ),
            ),
            'actions' => array(
                'data' => array(
                    '#markup' => $actions_content,
                ),
            ),
        );

        $rows[$reservation->getNid()] = _groom_admin_table_add_classes($row);
        $rows_inputs[$reservation->getNid()] = array(
            'data' => array(
                '#children' => $reservation->getNid()
            )
        );
    }

    // Reservations table list
    $output .= theme('tableselect', array(
        'element' => array(
            '#header'     => $header,
            '#options'    => $rows,
            '#js_select'  => true,
            '#empty'      => t('Aucunes réservations Groom pour le moment.'),
            '#attributes' => array(
                'class' => array( 'groom-table', 'table-has-actions'),
                'id'    => 'groom-reservations-list',
            ),
        ) + $rows,
    ));

    // Table pager
    $output .= '<div class="pager-wrap">';
    $output .= theme('pager', array(
        'element'    => 0,
        'tags'       => array(),
        'parameters' => array(),
        'quantity'   => $pager_total,
    ));
    $output .= '</div>';

    return $output;
}

function theme_groom_input_number($variables)
{
    $element = $variables['element'];
    $element['#attributes']['type'] = 'number';
    element_set_attributes($element, array('id', 'name', 'value', 'step', 'min', 'max', 'placeholder'));
    _form_set_class($element, array('form-number', 'form-text'));

    return '<input' . drupal_attributes($element['#attributes']) . ' />';
}

function theme_groom_status_messages($variables)
{
    $display        = $variables['display'];
    $messages_total = $variables['messages'];
    $output         = '';

    $status_heading = array(
        'status'  => t('Status message'),
        'error'   => t('Error message'),
        'warning' => t('Warning message'),
    );

    foreach ($messages_total as $type => $messages)
    {
        if (!empty($display) && $display !== $type) {
            continue;
        }

        $output .= "<div class=\"messages $type\">\n";
        if (!empty($status_heading[$type])) {
            $output .= '<h2 class="element-invisible">' . $status_heading[$type] . "</h2>\n";
        }

        if (count($messages) > 1)
        {
            $output .= " <ul>\n";
            foreach ($messages as $message) {
                $output .= '  <li>' . $message . "</li>\n";
            }
            $output .= " </ul>\n";
        }
        else {
            $output .= reset($messages);
        }

        $output .= "</div>\n";
    }

    return $output;
}

function groom_add_groom_messages()
{
    if (isset($_SESSION['groom']) && isset($_SESSION['groom']['messages']))
    {
        $groom_messages       = $_SESSION['groom']['messages'];
        unset($_SESSION['groom']['messages']);
        $_SESSION['messages'] = array_merge_recursive($_SESSION['messages'], $groom_messages);
    }
}