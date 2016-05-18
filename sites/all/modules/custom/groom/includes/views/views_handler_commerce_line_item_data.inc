<?php

/**
 * Custom handler class.
 *
 * @ingroup views_field_handlers
 */
class views_handler_commerce_line_item_data extends views_handler_field
{
    private $information_display_options;

    function init(&$view, &$options)
    {
        parent::init($view, $options);

        $this->information_display_options = array(
            'slot_type'  => t('Slot time type'),
            'slot_range' => t('Slot range'),
            'room_name'  => t('Room name'),
            'date'       => t('Booking date'),
            'capacity'   => t('Capacity')
        );
    }

    /**
     * {@inheritdoc}
     * Perform any database or cache data retrieval here. In this example there is
     * none.
     */
    function query()
    {
    }

    /**
     * {@inheritdoc}
     * Modify any end user views settings here. Debug $options to view the field
     * settings you can change.
     */
    function option_definition()
    {
        $options                         = parent::option_definition();
        $options['informations_display'] = array('default' => array());

        return $options;
    }

    /**
     * {@inheritdoc}
     * Make changes to the field settings form seen by the end user when adding
     * your field.
     */
    function options_form(&$form, &$form_state)
    {
        parent::options_form($form, $form_state);

        $form['informations_display'] = array(
            '#type'          => 'select',
            '#title'         => t('Informations à afficher'),
            '#multiple'      => true,
            '#default_value' => $this->options['informations_display'],
            '#options'       => $this->information_display_options
        );
    }

    /**
     * Render callback handler.
     * Return the markup that will appear in the rendered field.
     *
     * @param array $values Values retrieved from Views
     *
     * @return string Output content to display
     */
    function render($values)
    {
        $output = '';
        $entity = NULL;

        if (array_key_exists('line_item_id', $values->_field_data)) {
            $entity = $values->_field_data['line_item_id']['entity'];
        } elseif (array_key_exists('commerce_line_item_field_data_commerce_line_items_line_item_', $values->_field_data)) {
            $entity = $values->_field_data['commerce_line_item_field_data_commerce_line_items_line_item_']['entity'];
        }

        if (!$entity || $entity->type !== 'product') {
            return $output;
        }

        $line_item = entity_metadata_wrapper('commerce_line_item', $entity);
        $product   = $line_item->commerce_product;

        if ($product->type->value() !== 'salle' || empty($this->options['informations_display'])) {
            return $output;
        }

        $output = '<ul>';
        foreach ($this->information_display_options as $information_name => $information_label)
        {
            if (!isset($entity->data[$information_name])) {
                continue;
            }

            // Format date displaying in cart
            if ($information_name === 'date')
            {
                if (!preg_match('/^[0-9]{2}\/[0-9]{2}\/[0-9]{4}$/', $entity->data[$information_name])) {
                    $date                            = new \DateTime($entity->data[$information_name]);
                    $entity->data[$information_name] = $date->format('d/m/Y');
                }
            }

            $output .= '<li><strong>'.$information_label.' : </strong>'.$entity->data[$information_name].'</li>';
        }
        $output .= '</ul>';

        return $output;
    }
}