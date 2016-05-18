<?php

/**
 * Draw a calendar view of a month with time slots
 *
 * @param int $month
 * @param int $year
 * @param array $record
 *
 * @return string Html content of the calendar
 */
function groom_draw_calendar($month, $year, $record, $rooms, $reservations)
{
    // Get datetime object for displayed month.
    $datetime      = \DateTime::createFromFormat('d-m-Y', '01-' . $month . '-' . $year);
    $previousMonth = clone $datetime; $previousMonth->modify('-1 month');
    $nextMonth     = clone $datetime; $nextMonth->modify('+1 month');
    $today         = new \DateTime('today');
    $is_solo       = $record->nid == variable_get('groom_solo_reservation_slot_type', 103);
    $max_solo      = variable_get('groom_solo_reservation_capacity', 23);

    // How many days does this month contain ?
    $numberDays = date('t', $datetime->getTimestamp());

    // What is the index value (1-7) of the first day of the
    // month in question ?
    $dayOfWeek = date('N', $datetime->getTimestamp());

    // Exclude days
    $excludeDays = array(
        '2015-11-11',
        '2015-12-24',
        '2015-12-25',
        '2015-12-26',
        '2015-12-28',
        '2015-12-29',
        '2015-12-30',
        '2015-12-31',
        '2016-01-01',
        '2016-01-02',
        '2016-03-28',
        '2016-05-01',
        '2016-05-08',
        '2016-05-05',
        '2016-05-16',
        '2016-07-14',
        '2016-08-15',
        '2016-11-01',
        '2016-11-11',
        '2016-12-25',
        '2017-01-01',
        '2017-03-17',
        '2017-05-01',
        '2017-05-08',
        '2017-05-25',
        '2017-06-05',
        '2017-07-14',
        '2017-08-15',
        '2017-11-01',
        '2017-11-11',
        '2017-12-25'
    );

    // Create title and months navigation.
    $calendar = '<div id="calendar" class="cal-context" style="width:100%;">';
    $calendar .= '<h2>'.t('Réservation - @date', array('@date' => $datetime->format('m/Y'))) . '</h2>'
        . '<div class="buttons cf">'
            . '<a href="?month-display=' . $previousMonth->format('Y-m') . '" title="'.t('Mois précédent').'" class="btn btn-primary pull-left">'.t('Mois précédent').'</a>'
            . '<a href="?month-display=' . $nextMonth->format('Y-m') . '" title="'.t('Mois suivant').'" class="btn btn-primary pull-right">'.t('Mois suivant').'</a>'
        . '</div>';

    // Create the calendar headers.
    $headers = array('Lundi', 'Mardi', 'Mercredi', 'Jeudi', 'Vendredi', 'Samedi', 'Dimanche');
    $calendar .= '<div class="cal-row-fluid cal-row-head">';
    foreach($headers as $header) {
        $calendar .= '<div class="cal-cell1"><strong>' . t($header) . '</strong></div>';
    }
    $calendar .= '</div>';

    // Create the rest of the calendar.
    // Initiate the day counter, starting with the 1st.
    $currentDay = 1;

    $calendar .= '<div class="cal-month-box">';
    $calendar .= '<div class="cal-row-fluid">';

    // The variable $dayOfWeek is used to ensure that the calendar
    // display consists of exactly 7 columns.
    if ($dayOfWeek > 1) {
        for ($x = 1; $x < $dayOfWeek; $x++) {
            $calendar .= '<div class="cal-cell1 cal-cell cal-day-outmonth">';
            $calendar .= '<div class="cal-month-day"></div>';
            $calendar .= '</div>';
        }
    }

    $month = str_pad($month, 2, '0', STR_PAD_LEFT);

    while ($currentDay <= $numberDays) {
        // Seventh column (Saturday) reached. Start a new row.
        if ($dayOfWeek == 8) {
            $dayOfWeek = 1;
            $calendar .= '</div><div class="cal-row-fluid">';
        }

        $fieldTimeSlots = $record->{'field_time_slot_type_plages_' . $dayOfWeek};
        $timeSlots = array();
        if ($fieldTimeSlots) {
            $timeSlots = explode("\n", $fieldTimeSlots['und'][0]['value']);
        }

        $currentDayRel = str_pad($currentDay, 2, '0', STR_PAD_LEFT);

        $date = $year . '-' . $month . '-' . $currentDayRel;
        $currentDateTime = DateTime::createFromFormat('d-m-Y', $currentDayRel . '-' . $month . '-' . $year);

        // Disable past day.
        if ($today > $currentDateTime
            || in_array($date, $excludeDays)
        ) {
            $calendar .= '<div class="cal-cell1 cal-cell cal-day-inmonth cal-day-disable" data-date="' . $date . '" tooltip="'.t('Ce jour n\'est pas/plus disponible à la réservation').'">';
        } else {
            if ($today->format('Y-m-d') === $date) {
                $calendar .= '<div class="cal-cell1 cal-cell cal-day-inmonth today" data-date="' . $date . '">';
            } else {
                $calendar .= '<div class="cal-cell1 cal-cell cal-day-inmonth" data-date="' . $date . '">';
            }
        }
        $calendar .= '<div class="cal-month-day"><strong class="day">' . $currentDay . '</strong></div>';

        if ($today < $currentDateTime
            && !in_array($date, $excludeDays)
        ) {
            foreach ($timeSlots as $timeSlot) {
                $class_disabled = '';
                $link_class     = '';
                $timeSlot       = str_replace("\r", '', $timeSlot);

                if (isset($reservations[$date][$timeSlot]) && (
                    count($reservations[$date][$timeSlot]) == count($rooms)
                    || $is_solo && groom_utils_sum_reservation_quantity($reservations[$date][$timeSlot]) >= $max_solo
                )) {
                    $class_disabled = 'disabled';
                } else {
                    $link_class = 'fancy';
                }

                $link = l(
                    t($timeSlot),
                    "groom/reservation-choose/$record->nid/$date/$timeSlot",
                    array('attributes' => array('class' => $link_class))
                );
                $calendar .= '<div class="field_time_slot_type_plages '.$class_disabled.'">' . $link . '</div>';
            }
        }

        $calendar .= '</div>';

        // Increment counters
        $currentDay++;
        $dayOfWeek++;
    }

    // Complete the row of the last week in month, if necessary
    if ($dayOfWeek != 8) {
        $remainingDays = 8 - $dayOfWeek;
        for ($x = 1; $x < $remainingDays; $x++) {
            $calendar .= '<div class="cal-cell1 cal-cell cal-day-outmonth">';
            $calendar .= '<div class="cal-month-day"></div>';
            $calendar .= '</div>';
        }
    }

    $calendar .= '</div>';
    $calendar .= '</div>';

    return $calendar;
}