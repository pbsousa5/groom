<?php

class GroomSlotType
{
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
     * GroomSlotType constructor.
     *
     * @param int                   $nid
     * @param stdClass              $node
     * @param EntityMetadataWrapper $entity
     */
    public function __construct($nid, stdClass $node = NULL, EntityMetadataWrapper $entity = NULL)
    {
        $this->nid = $nid;
        $this->node = $node;
        $this->entity = $entity;
    }


    /* =======================================
     *    METHODS
      ======================================= */

    /**
     * Retrieve slot time ranges available for this slot type
     *
     * @return array Ranges of slot time
     */
    public function getSlotRanges()
    {
        $ranges = array();

        if ($this->entity)
        {
            // 7 for the days in a week
            for ($i = 1; $i <= 7; $i++)
            {
                $value = $this->entity->{'field_time_slot_type_plages_'.$i}->value();
                $value = str_replace("\r", '', $value);

                $ranges[$i] = explode("\n", $value);
            }
        }

        return $ranges;
    }

    /**
     * Test if the slot time exist for the current slot time type.
     *
     * @param string   $slot_range The slot range to test
     * @param int|null $day        The day in which the slot range has to be contained (1 to 7).
     *                             Leave null to test against all days in week and match the first
     *                             occurence.
     *
     * @return bool If the slot range exists or not
     */
    public function hasSlotRange($slot_range, $day = NULL)
    {
        $has_slot_time = FALSE;

        if (!$this->entity) {
            return $has_slot_time;
        }

        if ($day === NULL)
        {
            for ($i = 1; $i <= 7; $i++)
            {
                $slot_times_str = $this->entity->{'field_time_slot_type_plages_'.$i}->value();
                $slot_times_str = str_replace("\r", '', $slot_times_str);
                $slot_ranges    = explode("\n", $slot_times_str);

                if (array_search($slot_range, $slot_ranges))
                {
                    $has_slot_time = TRUE;
                    break;
                }
            }
        }
        elseif (isset($this->entity->{'field_time_slot_type_plages_'.$day}))
        {
            $slot_times_str = $this->entity->{'field_time_slot_type_plages_'.$day}->value();
            $slot_times_str = str_replace("\r", '', $slot_times_str);
            $slot_ranges    = explode("\n", $slot_times_str);

            if (array_search($slot_range, $slot_ranges)) {
                $has_slot_time = TRUE;
            }
        }

        return $has_slot_time;
    }

    /* =======================================
     *    STATIC METHODS
      ======================================= */

    /**
     * Retrieve a slot type for a given node id
     *
     * @param int $id The slot type node id
     *
     * @return GroomSlotType The found slot type, NULL otherwise.
     */
    public static function get($id)
    {
        $slot_type = NULL;

        $slot_type_node = entity_load('node', array($id), array(
            'type' => 'time_slot_type',
            'status' => NODE_PUBLISHED
        ));

        if ($slot_type_node) {
            $slot_type = new self($id, $slot_type_node[$id], entity_metadata_wrapper('node', $slot_type_node[$id]));
        }

        return $slot_type;
    }

    /**
     * Retrieve all slots types
     *
     * @return array Array of GroomSlotType.
     */
    public static function findAll()
    {
        $slot_types = array();

        $slot_types_nodes = entity_load('node', FALSE, array(
            'type'   => 'time_slot_type',
            'status' => NODE_PUBLISHED
        ));

        if ($slot_types_nodes)
        {
            foreach ($slot_types_nodes as $slot_type_id => $slot_type_node) {
                $slot_types[$slot_type_id] = new self($slot_type_id, $slot_type_node, entity_metadata_wrapper('node', $slot_type_node));
            }
        }

        return $slot_types;
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
