<?php
/**
 * @file
 * Template for invoiced orders.
 */
?>

<div class="invoice-footer"><?= render($content['invoice_footer']); ?></div>

<div class="invoice">
    <div class="invoice-invoiced">

        <div class="invoice-container">
            <div class="header">
                <img src="<?= $content['invoice_logo']['#value']; ?>" />

                <div class="invoice-header">
                    <p><?= render($content['invoice_header']); ?></p>
                </div>
            </div>

            <hr />

            <div class="invoice-header-date"><?= render($content['invoice_header_date']); ?></div>
            <div class="customer"><?= render($content['commerce_customer_billing']); ?></div>
            <h1 class="invoice-title"><?= render($content['order_number']); ?></h1>

            <div class="invoice-number"></div>
            <div class="order-id"><?= render($content['order_id']); ?></div>

            <div class="line-items">
                <div class="line-items-view"><?= render($content['commerce_line_items']); ?></div>
                <div class="order-total"><?= render($content['commerce_order_total']); ?></div>
            </div>

            <div class="invoice-text">
                <?= render($content['invoice_text']) ?>
            </div>
        </div>
    </div>
</div>
