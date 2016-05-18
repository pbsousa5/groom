<?php
module_load_include('inc', 'groom', 'theme/groom.theme');


// Load reservation
$time_slots = node_load($time_slot_type);

// Display calendar
$display_month = intval(date_format($display_date, 'n'));
$display_year = intval(date_format($display_date, 'Y'));

echo groom_draw_calendar($display_month, $display_year, $time_slots, $rooms, $reservations);