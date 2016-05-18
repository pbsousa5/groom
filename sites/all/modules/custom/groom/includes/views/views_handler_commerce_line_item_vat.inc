<?php

/**
 * Custom handler class.
 *
 * @ingroup views_field_handlers
 */
class views_handler_commerce_line_item_vat extends views_handler_field
{
    function init(&$view, &$options)
    {
        parent::init($view, $options);
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
     * Render callback handler.
     * Return the markup that will appear in the rendered field.
     */
    function render($values)
    {
        $data      = $values->_field_data['line_item_id']['entity'];
        $line_item = entity_metadata_wrapper('commerce_line_item', $data);
        $price     = $line_item->commerce_unit_price->value();
        $vat       = 0;

        if (isset($price['data']) && isset($price['data']['include_tax']))
        {
            foreach ($price['data']['components'] as $component)
            {
                if ($component['name'] === 'tax|' . $price['data']['include_tax']) {
                    $tva_rate = $component['price']['data']['tax_rate'];
                }
            }
        }

        if (isset($tva_rate))  {
            $vat = $tva_rate['rate'] * 100;
        }
        else
        {
            $vat_price = 0;

            foreach (commerce_tax_rates() as $vat_tax)
            {
                $tax_price = commerce_price_component_total($line_item->commerce_unit_price->value(), $vat_tax['price_component']);
                $vat_price += $tax_price['amount'];
            }

            $raw_price = commerce_price_component_total($line_item->commerce_unit_price->value(), 'base_price');
            $vat = ceil($vat_price/$raw_price['amount'] * 100);
        }

        if ($vat > 0) {
            $vat .= '%';
        } else {
            $vat = '-';
        }

        return $vat;
    }
}