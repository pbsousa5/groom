<?php

class GroomReservation
{
    /* =======================================
     *    CONSTANTS
      ======================================= */

    const STATUS_PENDING   = 'pending';
    const STATUS_VALIDATED = 'validated';
    const STATUS_CANCELED  = 'canceled';


    /* =======================================
     *    PROPERTIES
      ======================================= */

    /** @var int */
    private $nid;

    /** @var EntityMetadataWrapper */
    private $user;

    /** @var DateTime */
    private $date;

    /** @var GroomRoom */
    private $room;

    /** @var string */
    private $time_slot;

    /** @var GroomSlotType */
    private $time_slot_type;

    /** @var int */
    private $quantity;

    /** @var string */
    private $status;


    /* =======================================
     *    CONSTRUCTOR
      ======================================= */

    /**
     * GroomReservation constructor.
     *
     * @param int                   $nid
     * @param EntityMetadataWrapper $user
     * @param DateTime              $date
     * @param GroomRoom             $room
     * @param string                $time_slot
     * @param GroomSlotType         $time_slot_type
     * @param int                   $quantity
     * @param string                $status
     */
    public function __construct($nid, $user, $date, $room, $time_slot, $time_slot_type, $quantity, $status)
    {
        $this->nid            = $nid;
        $this->user           = $user;
        $this->date           = $date;
        $this->room           = $room;
        $this->time_slot      = $time_slot;
        $this->time_slot_type = $time_slot_type;
        $this->quantity       = $quantity;
        $this->status         = $status;
    }


    /* =======================================
     *    METHODS
      ======================================= */

    /**
     * Persist the GroomReservation entity into the database.
     *
     * @return bool TRUE if the peristance has succeed, FALSE otherwise
     */
    public function save()
    {
        $reservation = $this->nid ? self::get($this->nid) : null;
        $query       = db_insert('groom_reservation');
        $is_insert   = $reservation === null;

        $fields = array(
            'uid'            => $this->user->getIdentifier(),
            'date'           => $this->date->format('Y-m-d'),
            'room'           => $this->room !== null ? $this->room->getNid() : GroomRoom::ROOM_SOLO_NID,
            'time_slot'      => $this->time_slot,
            'time_slot_type' => $this->time_slot_type->getNid(),
            'quantity'       => $this->quantity,
            'status'         => $this->status,
        );

        if (!$is_insert)
        {
            $query = db_update('groom_reservation');
            $query->condition('nid', $this->nid, '=');
        }

        $return = $query->fields($fields)->execute();

        if ($is_insert && $return != 0) {
            $this->nid = intval($return);
        }

        if ($return !== false) {
            return true;
        }

        return false;
    }

    /**
     * Persist the GroomReservation entity into the database.
     *
     * @return bool TRUE if the peristance has succeed, FALSE otherwise
     */
    public function delete()
    {
        db_delete('groom_reservation')
            ->condition('nid', $this->nid, '=')
	        ->execute();
    }


    /* =======================================
     *    STATIC METHODS
      ======================================= */

    /**
     * Retrieve a specific Groom reservation from database.
     *
     * @param int $id The GroomReservation id
     *
     * @return GroomReservation The found reservation, NULL otherwise.
     */
    public static function get($id)
    {
        $reservation = null;

        $reservations = self::getMultiple(array($id));

        if (!empty($reservations))
        {
            reset($reservations);
            $reservation = current($reservations);
        }

        return $reservation;
    }

    /**
     * Retrieve specifics Groom reservations from database.
     *
     * @param array $ids The GroomReservation ids
     *
     * @return array Array of founds GroomReservation.
     */
    public static function getMultiple(array $ids)
    {
        $reservations = array();

        $query_result = db_select('groom_reservation', 'r')
            ->fields('r')
            ->condition('nid', $ids, 'IN')
            ->execute();

        if ($query_result)
        {
            $reservations = $query_result->fetchAllAssoc('nid', PDO::FETCH_ASSOC);
            $reservations = self::dbToInstances($reservations);
        }

        return $reservations;
    }

    /**
     * Retrieve Groom reservations filtering by date, time slot and status.
     *
     * @param string $date      Date under "yyyy-mm-dd" format
     * @param string $time_slot Time slot value
     * @param array  $status    Status
     *
     * @return array Associative array containing each Groom reservation
     */
    public static function findByDateAndTimeSlotAndStatus($date, $time_slot, $status)
    {
        $reservations     = self::findBy($date, $time_slot, 0, $status);
        $reservations_out = array();

       foreach ($reservations as $reservation) {
            $reservations_out[] = $reservation->getRoom()->getNid();
        }

        return $reservations_out;
    }

    /**
     * Retrieve Groom reservations with use of filters.
     *
     * @param string $date           Date under "yyyy-mm-dd" format
     * @param string $time_slot      Time slot value
     * @param int    $time_slot_type Time slot type id
     * @param array  $status         Status
     * @param int    $room           Room
     * @param int    $user           User id
     * @param string $index_by       Index results by the given field name
     * @param int    $offset         Position of first record
     * @param int    $limit          Number of records to return
     * @param array  $sort           Array of fields sorting. Key as field name, value as order (ASC or DESC)
     *
     * @return array Associative array containing each Groom reservation, indexed by nid (default)
     */
    public static function findBy(
        $date = null,
        $time_slot = null,
        $time_slot_type = 0,
        $status = array(),
        $room = null,
        $user = 0,
        $index_by = 'nid',
        $offset = 0,
        $limit = -1,
        $sort = array('date' => 'desc')
    ) {
        $reservations = array();

        $query = db_select('groom_reservation', 'reservation')
            ->fields('reservation');

        if ($date !== null) {
            $query->condition('date', $date, '=');
        }

        if ($time_slot !== null) {
            $query->condition('time_slot', $time_slot, '=');
        }

        if ($room !== null) {
            $query->condition('room', $room, '=');
        }

        if ($time_slot_type > 0) {
            $query->condition('time_slot_type', $time_slot_type, '=');
        }

        if (!empty($status)) {
            $query->condition('status', $status, 'IN');
        }

        if ($user > 0) {
            $query->condition('uid', $user, '=');
        }

        if ($offset > 0 || $limit > -1)
        {
            $limit  = $limit === -1 ? null : $limit;
            $offset = $offset === 0 ? null : $offset;
            $query->range($offset, $limit);
        }

        if (!empty($sort))
        {
            foreach ($sort as $sort_field => $sort_order) {
                $query->orderBy($sort_field, $sort_order);
            }
        }

        $result = $query->execute();

        if ($result)
        {
            $reservations = $result->fetchAllAssoc($index_by, PDO::FETCH_ASSOC);
            $reservations = self::dbToInstances($reservations);
        }

        return $reservations;
    }

    /**
     * Retrieve Groom reservations filtered by date (equal, before, after) and status.
     *
     * @param string       $date          Date under "yyyy-mm-dd" format
     * @param string       $date_operator Operator for the date comparison
     * @param array|string $status        Status, by array of status or single one
     * @param string       $index_by      Index results by the given field name
     * @param int          $offset        Position of first record
     * @param int          $limit         Number of records to return
     * @param array        $sort          Array of fields sorting. Key as field name, value as order (ASC or DESC)
     * @param bool         $exclude_solo  Exclude solo reservation from the search
     * @param int          $uid           User id to filter
     *
     * @return array Associative array containing each Groom reservation, indexed by nid (default)
     */
    public static function findByDateAndStatus(
        $date,
        $date_operator = '=',
        $status        = array(),
        $index_by      = 'nid',
        $offset        = 0,
        $limit         = -1,
        $sort          = array('date' => 'desc'),
        $exclude_solo  = true,
        $uid           = -1
    ) {
        $reservations = array();

        $query = db_select('groom_reservation', 'reservation')
            ->fields('reservation')
            ->condition('date', $date, $date_operator);

        $status = is_string($status) ? array($status) : $status;
        if (!empty($status)) {
            $query->condition('status', $status, 'IN');
        }

        if ($offset > 0 || $limit > -1)
        {
            $limit  = $limit === -1 ? null : $limit;
            $offset = $offset === 0 ? null : $offset;
            $query->range($offset, $limit);
        }

        if (!empty($sort))
        {
            foreach ($sort as $sort_field => $sort_order) {
                $query->orderBy($sort_field, $sort_order);
            }
        }

        if ($exclude_solo) {
            $query->condition('room', GroomRoom::ROOM_SOLO_NID, '>');
        }

        if ($uid > 0) {
            $query->condition('uid', $uid, '=');
        }

        $result = $query->execute();

        if ($result)
        {
            $reservations = $result->fetchAllAssoc($index_by, PDO::FETCH_ASSOC);
            $reservations = self::dbToInstances($reservations);
        }

        return $reservations;
    }

    /**
     * Count the amount of reservations using differents filters.
     *
     * @param string      $date           Date under "yyyy-mm-dd" format
     * @param string      $time_slot      Time slot value
     * @param int         $time_slot_type Time slot type id
     * @param array       $status         Status
     * @param int         $room           Room
     * @param int         $user           User id
     * @param int         $offset         Position of first record
     * @param int         $limit          Number of records to return
     *
     * @return int Number of Groom reservations
     */
    public static function countBy(
        $date = null,
        $time_slot = null,
        $time_slot_type = 0,
        $status = array(),
        $room = null,
        $user = 0,
        $offset = 0,
        $limit = -1
    ) {
        $count = 0;

        $query = db_select('groom_reservation', 'reservation');

        if ($date !== null) {
            $query->condition('date', $date, '=');
        }

        if ($time_slot !== null) {
            $query->condition('time_slot', $time_slot, '=');
        }

        if ($room !== null) {
            $query->condition('room', $room, '=');
        }

        if ($user > 0) {
            $query->condition('uid', $user, '=');
        }

        if ($time_slot_type > 0) {
            $query->condition('time_slot_type', $time_slot_type, '=');
        }

        if (!empty($status)) {
            $query->condition('status', $status, 'IN');
        }

        if ($offset > 0 || $limit > -1)
        {
            $limit  = $limit === -1 ? null : $limit;
            $offset = $offset === 0 ? null : $offset;
            $query->range($offset, $limit);
        }

        $result = $query->countQuery()->execute();

        if ($result) {
            $count = $result->fetchField();
        }

        return $count;
    }

    /**
     * Retrieve the quantity (sum) of Groom reservations
     *
     * @param string $date           Date under "yyyy-mm-dd" format
     * @param string $time_slot      Time slot value
     * @param int    $time_slot_type Time slot type id
     * @param int    $uid            User id
     * @param string $status         Status of the reservation (STATUS_PENDING, STATUS_VALIDATED or STATUS_CANCELED)
     *
     * @return int Number of reservations quantity for the given parameters
     */
    public static function getQuantityBy($date = null, $time_slot = null, $time_slot_type = 0, $uid = -1, $status = null)
    {
        $reservations_quantity = 0;

        $query = db_select('groom_reservation', 'r');
        $query->addExpression('SUM(r.quantity)');

        if ($date !== null) {
            $query->condition('date', $date, '=');
        }

        if ($time_slot !== null) {
            $query->condition('time_slot', $time_slot, '=');
        }

        if ($time_slot_type > 0) {
            $query->condition('time_slot_type', $time_slot_type, '=');
        }

        if ($uid > 0) {
            $query->condition('uid', $uid, '=');
        }

        if ($status !== null) {
            $query->condition('status', $status, '=');
        }

        $result = $query->execute();

        if ($result) {
            $reservations_quantity = intval($result->fetchField(0));
        }

        return $reservations_quantity;
    }

    /**
     * Retrieve co-workers (users) associated with Groom reservations
     *
     * @param array $reservations Array of GroomReservation
     *
     * @return array Array of users (of EntityMetadataWrapper instaces) indexed by their uid
     */
    public static function findCoWorkers(array $reservations)
    {
        $co_workers = array();

        foreach ($reservations as $reservation)
        {
            $co_worker                               = $reservation->getUser();
            $co_workers[$co_worker->getIdentifier()] = $co_worker;
        }

        return $co_workers;
    }

    /**
     * Converts array of reservations, retrieved from the database, into an array of GroomReservation instances
     *
     * @param array $reservations Reservations retrieved from database
     *
     * @return array Array of GroomReservation
     */
    private static function dbToInstances(array $reservations)
    {
        $groom_reservations = array();

        foreach ($reservations as $id => $reservation) {
            $groom_reservations[$id] = self::dbToInstance($reservation);
        }

        return $groom_reservations;
    }

    /**
     * Convert a reservation array into a GroomReservation instance
     *
     * @param array $reservation Array representing a db reservation
     *
     * @return GroomReservation A GroomReservation intance
     */
    private static function dbToInstance(array $reservation)
    {
        return new self(
            $reservation['nid'],
            entity_metadata_wrapper('user', user_load($reservation['uid'])),
            new DateTime($reservation['date']),
            GroomRoom::get($reservation['room']),
            $reservation['time_slot'],
            GroomSlotType::get($reservation['time_slot_type']),
            intval($reservation['quantity']),
            $reservation['status']
        );
    }


    /* =======================================
     *    GETTERS & SETTERS
      ======================================= */

    /**
     * @return int
     */
    public function getNid()
    {
        return $this->nid;
    }

    /**
     * @param int $nid
     */
    public function setNid($nid)
    {
        $this->nid = $nid;
    }

    /**
     * @return EntityMetadataWrapper
     */
    public function getUser()
    {
        return $this->user;
    }

    /**
     * @param EntityMetadataWrapper $user
     */
    public function setUser($user)
    {
        $this->user = $user;
    }

    /**
     * @return DateTime
     */
    public function getDate()
    {
        return $this->date;
    }

    /**
     * @param DateTime $date
     */
    public function setDate($date)
    {
        $this->date = $date;
    }

    /**
     * @return GroomRoom
     */
    public function getRoom()
    {
        return $this->room;
    }

    /**
     * @param GroomRoom $room
     */
    public function setRoom($room)
    {
        $this->room = $room;
    }

    /**
     * @return string
     */
    public function getTimeSlot()
    {
        return $this->time_slot;
    }

    /**
     * @param string $time_slot
     */
    public function setTimeSlot($time_slot)
    {
        $this->time_slot = $time_slot;
    }

    /**
     * @return GroomSlotType
     */
    public function getTimeSlotType()
    {
        return $this->time_slot_type;
    }

    /**
     * @param GroomSlotType $time_slot_type
     */
    public function setTimeSlotType($time_slot_type)
    {
        $this->time_slot_type = $time_slot_type;
    }

    /**
     * @return int
     */
    public function getQuantity()
    {
        return $this->quantity;
    }

    /**
     * @param int $quantity
     */
    public function setQuantity($quantity)
    {
        $this->quantity = $quantity;
    }

    /**
     * @return string
     */
    public function getStatus()
    {
        return $this->status;
    }

    /**
     * @param string $status
     */
    public function setStatus($status)
    {
        $this->status = $status;
    }
}
