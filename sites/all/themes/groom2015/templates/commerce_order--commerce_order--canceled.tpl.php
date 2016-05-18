<?php
/**
 * @file
 * Template for credit memos (=canceled orders).
 */
?>
<div class="invoice-footer"><?= render($content['invoice_footer']); ?></div>

<div class="invoice">
    <div class="invoice-canceled">
        <div class="header">
            <img src="<?= $content['invoice_logo']['#value']; ?>" />

            <div class="invoice-header">
                <p><?= render($content['invoice_header']); ?></p>
            </div>
        </div>

        <hr />

        <div class="invoice-header-date"><?= render($content['invoice_header_date']); ?></div>
        <div class="customer"><?= render($content['commerce_customer_billing']); ?></div>
        <h1 class="credit-memo"><?= t('Avoir') ?></h1>
        <div>
            <?=
            t('Remboursement sur facture n°!order du !date', array(
                '!order' => $commerce_order->order_number,
                '!date' => $commerce_order->order_date->format('d/m/Y')
            ));
            ?>
        </div>

        <div class="order-id"><?= render($content['order_id']); ?></div>

        <div class="line-items">
            <div class="line-items-view"><?php print render($content['commerce_line_items']); ?></div>
        </div>

        <div class="order-total"><?= render($content['commerce_order_total']); ?></div>

        <div class="invoice-text">
            <?= t('Montant en euros remboursé sur votre carte bancaire au plus tard sous 5 jours.') ?>
        </div>
    </div>
</div>