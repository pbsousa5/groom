<?php

class GroomRoom
{
    /* =======================================
     *    CONSTANTS
      ======================================= */

    const ROOM_SOLO_NID = 0;


    /* =======================================
     *    PROPERTIES
      ======================================= */

    /**
     * @var int
     */
    private $nid;

    /**
     * @var stdClass
     */
    private $node;

    /**
     * @var EntityMetadataWrapper
     */
    private $entity;


    /* =======================================
     *    CONSTRUCTOR
      ======================================= */

    /**
     * GroomRoom constructor.
     *
     * @param int                   $nid
     * @param stdClass              $node
     * @param EntityMetadataWrapper $entity
     */
    public function __construct($nid, stdClass $node = NULL, EntityMetadataWrapper $entity = NULL)
    {
        $this->nid    = $nid;
        $this->node   = $node;
        $this->entity = $entity;
    }


    /* =======================================
     *    STATIC METHODS
      ======================================= */

    /**
     * Retrieve a room for a given node id
     *
     * @param int $id The room node id
     *
     * @return GroomRoom The found room, NULL otherwise.
     */
    public static function get($id)
    {
        $room = NULL;

        $room_node = entity_load('node', array($id), array(
            'type' => 'room',
            'status' => NODE_PUBLISHED
        ));

        if ($room_node) {
            $room = new self($id, $room_node[$id], entity_metadata_wrapper('node', $room_node[$id]));
        }

        return $room;
    }

    /**
     * Retrieve all rooms from database
     *
     * @return array Array of all GroomRoom
     */
    public static function findAll()
    {
        $rooms = array();

        $queryRooms = new EntityFieldQuery();
        $queryRooms->entityCondition('entity_type', 'node')
                ->entityCondition('bundle', 'room')
                ->propertyCondition('status', NODE_PUBLISHED);
        $result = $queryRooms->execute();

        if (isset($result['node']))
        {
            $rooms = entity_load('node', array_keys($result['node']));
            foreach ($rooms as $nid => &$room) {
                $room = new self($nid, $room, entity_metadata_wrapper('node', $room));
            }
        }

        return $rooms;
    }

    /**
     * Find all rooms available for a given slot type id
     *
     * @param int $time_slot_type_id Slot type id
     *
     * @return array An associative array containing the rooms (as nodes, with nid as key).
     *               Empty one if there is not.
     */
    public static function findByTimeSlotTypeId($time_slot_type_id)
    {
        $rooms = array();

        $queryRooms = new EntityFieldQuery();
        $queryRooms->entityCondition('entity_type', 'node')
                ->entityCondition('bundle', 'room')
                ->fieldCondition('field_room_time_slot_type', 'target_id', $time_slot_type_id, '=')
                ->propertyCondition('status', NODE_PUBLISHED);
        $result = $queryRooms->execute();

        if (isset($result['node'])) {
            $resa_nids = array_keys($result['node']);
            $rooms = entity_load('node', $resa_nids);
        }

        return $rooms;
    }

    /**
     * Find rooms available for a given date and time slot.
     *
     * @param int    $time_slot_type_id Time slot type id.
     * @param string $date              Date.
     * @param string $time_slot         Time slot.
     *
     * @return array An associative array containing the rooms (as nodes, with nid as key).
     *               Empty one if there is not.
     */
    public static function findAvailableByTimeSlotTypeIdAndDateAndTimeSlot($time_slot_type_id, $date, $time_slot)
    {
        $rooms = array();

        $queryRooms = new EntityFieldQuery();
        $queryRooms->entityCondition('entity_type', 'node')
                ->entityCondition('bundle', 'room')
                ->fieldCondition('field_room_time_slot_type', 'target_id', $time_slot_type_id, '=')
                ->propertyCondition('status', NODE_PUBLISHED);
        $result = $queryRooms->execute();

        if (isset($result['node'])) {
            $resa_nids = array_keys($result['node']);
            $rooms = entity_load('node', $resa_nids);
        }

        return $rooms;
    }


    /* =======================================
     *    GETTERS & SETTERS
      ======================================= */

    /**
     * @return EntityMetadataWrapper
     */
    public function getEntity()
    {
        return $this->entity;
    }

    /**
     * @param EntityMetadataWrapper $entity
     */
    public function setEntity($entity)
    {
        $this->entity = $entity;
    }

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
     * @return stdClass
     */
    public function getNode()
    {
        return $this->node;
    }

    /**
     * @param stdClass $node
     */
    public function setNode($node)
    {
        $this->node = $node;
    }
}
