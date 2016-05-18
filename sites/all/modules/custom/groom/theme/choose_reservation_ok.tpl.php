<div id="messages">
    <div class="alert">
        <h1><?= t('Réservation ajoutée au panier') ?></h1>
        <p><?= t('Votre réservation a bien été ajoutée au panier.') ?></p>
        <div id="groom-list-products-messages"></div>
        <?= render($products_form) ?>
        <div class="buttons cf">
            <a href="<?= url('cart') ?>" class="btn btn-primary pull-left" title="<?= t('Finir la commande') ?>">
                <?= t('Finir la commande') ?>
            </a>
            <a href="/produits-et-services" class="btn btn-primary pull-right" title="<?= t('Commander un autre produit') ?>">
                <?= t('Commander un autre produit') ?>
            </a>
        </div>
    </div>
</div>