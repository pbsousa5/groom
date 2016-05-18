<div id="messages">
    <div id="reservation-container" class="resa-solo">
        <section class="reservation-content alert cf">
            <h1><?= t('Votre réservation d\'un espace solo') ?></h1>

            <p class="big">
                <?php
                    $input_qty_attr = array(
                        'id'    => 'solo-quantity-input',
                        'class' => array('groom-input', 'input-type-number'),
                        'min'   => 1,
                        'max'   => $quantity_available,
                        'value' => 1,
                        'type'  => 'number',
                    );

                    if ($quantity_available === 0 || $already_reserved_nb > 0) {
                        $input_qty_attr['disabled'] = 'disabled';
                    }

                    $input_qty = '<input '.drupal_attributes($input_qty_attr).' />';
                ?>
                <?= t('Vous souhaiter réserver !qty place(s) de coworking en open space<br />pour le !date sur le créneau horaire !time_slot.', array(
                    '!qty'       => $input_qty,
                    '!date'      => '<strong>' . $date . '</strong>',
                    '!time_slot' => '<strong>' . $time_slot . '</strong>',
                )) ?>
            </p>
            <br />

            <p>
                <?= t('Il reste @qty_avail / @qty_max place(s) disponible(s).', array(
                    '@qty_avail' => $quantity_available,
                    '@qty_max'   => $quantity_max,
                )) ?>
            </p>

            <?php if (empty($co_workers)): ?>
                <p class="description empty">
                    <?= t('Aucun autre co-worker n\'a encore réservé ce créneau') ?>
                </p>
            <?php else: ?>
                <p class="description">
                    <?php if (count($co_workers) === 1): ?>
                        <?= t('Voici le co-worker déjà inscrit') ?> :
                    <?php else: ?>
                        <?= t('Voici les !nb co-workers déjà inscrits', array('!nb' => '<strong>' . count($co_workers) . '</strong>')) ?> :
                    <?php endif; ?>
                </p>

                <table class="co-workers">
                    <thead>
                        <tr>
                            <th class="cw-name"><?= t('Nom') ?></th>
                            <th class="cw-company"><?= t('Société') ?></th>
                            <th class="cw-quantity"><?= t('Place(s)') ?></th>
                        </tr>
                    </thead>
                    <tbody>
                        <?php $cw_count = 0; ?>
                        <?php foreach ($co_workers as $co_worker): ?>
                            <?php
                                $cw_uid      = $co_worker->getIdentifier();
                                $cw_quantity = $reservations[$cw_uid]->getQuantity();
                                $cw_company  = $co_worker->field_user_societe->value();
                                $cw_name     = $co_worker->field_user_prenom->value().' '.$co_worker->field_user_nom->value();
                                $cw_classes  = array();

                                if ($cw_uid == $current_uid)
                                {
                                    $cw_classes[] = 'himself';
                                    $cw_name      = t('Vous-même');
                                }

                                if ($cw_count % 2 == 0) {
                                    $cw_classes[] = 'even';
                                } else {
                                    $cw_classes[] = 'odd';
                                }

                                if ($cw_count == 0) {
                                    $cw_classes[] = 'first';
                                }

                                if ($cw_count == (count($co_workers) - 1)) {
                                    $cw_classes[] = 'last';
                                }
                            ?>

                            <tr class="co-worker <?= implode(' ', $cw_classes) ?>">
                                <td class="cw-name"><?= $cw_name ?></td>
                                <td class="cw-company"><?= $cw_company ?></td>
                                <td class="cw-quantity"><?= $cw_quantity ?></td>
                            </tr>

                            <?php $cw_count++; ?>
                        <?php endforeach; ?>
                    </tbody>
                </table>
            <?php endif; ?>

            <br /><br />

            <?php if ($already_reserved_nb === 0): ?>
                <?= drupal_render($reservation_form); ?>
                <a href="#" title="Annuler" class="btn btn-danger btn-close">Annuler</a>
            <?php else: ?>
                <span class="btn btn-primary disabled" title="<?= t('Vous avez déjà réservé @nb place(s) pour ce créneau', array('@nb' => $already_reserved_nb)) ?>">
                    <?= t('Réserver') ?>
                </span>
                <a href="#" title="Annuler" class="btn btn-danger btn-close">Annuler</a>
            <?php endif; ?>
        </section>

        <section class="reservation-insufficient hidden">
            <h1><?= t('Votre réservation d\'une salle de réunion') ?></h1>
            <p>
                <br />
                <?= t('Vous ne disposez pas d\'assez d\'unités SOLO pour effectuer une réservation.') ?>
                <br /><br />
            </p>
            <div class="buttons">
                <a class="btn btn-primary" href="<?= url('unites-nomade-solo') ?>">
                    <?= t('Acheter vos unités SOLO') ?>
                </a>
                <a class="btn btn-cancel btn-danger" href="#"><?= t('Annuler') ?></a>
            </div>
        </section>

        <section class="reservation-confirm hidden">
            <h1><?= t('Votre réservation d\'un espace SOLO') ?></h1>
            <p>
                <br />
                <?= t('Vous disposez de !units unités SOLO.', array('!units' => '<strong>' . $user_points . '</strong>')) ?><br />
                <?= t('Il sera débité !price unités SOLO de votre compte.', array('!price' => '<strong id="solo-units-price">1</strong>')) ?>
            </p>
            <p>
                <?= t('Êtes-vous sûr(e) de vouloir réserver ?') ?>
            </p>
            <br /><br />
            <div class="buttons">
                <a class="btn btn-primary btn-confirm" href="#"><?= t('Confirmer') ?></a>
                <a class="btn btn-cancel btn-danger" href="#"><?= t('Annuler') ?></a>
            </div>
        </section>
    </div>
</div>