<?= t('Vous n\'avez pas encore ajouté de réservation de salle à votre panier.') ?>
<br />

<?php if ($has_reservations): ?>
    <hr />
    <?= render($reservations_form) ?>
<?php else: ?>
    <?= t('Afin de pouvoir ajouter des services à votre panier, vous devez au préalable réserver une salle') ?>
<?php endif; ?>
