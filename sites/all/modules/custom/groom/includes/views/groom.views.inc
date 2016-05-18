<?php

/**
 * Implements hook_views_data().
 */
function groom_views_data_alter(&$data)
{
    $data['commerce_line_item']['product_line_item_infos'] = array(
        'field' => array(
            'title'   => t('Infos réservation'),
            'help'    => t('Informations complémentaires à propos de la ligne produit (réservation)'),
            'handler' => 'views_handler_commerce_line_item_data'
        )
    );

    $data['commerce_line_item']['product_line_item_vat'] = array(
        'field' => array(
            'title'   => t('TVA'),
            'help'    => t('Affiche la TVA de la ligne'),
            'handler' => 'views_handler_commerce_line_item_vat'
        )
    );
}
