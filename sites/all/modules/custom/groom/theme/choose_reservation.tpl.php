<div id="messages">
    <div id="reservation-container">
        <div class="resa-room alert cf">
            <h1><?= t('Votre réservation d\'une salle de réunion') ?></h1>

            <p class="big">
                <?= t('Vous souhaitez réserver une salle de réunion<br/>pour le !date sur le créneau horaire !time_slot.', array(
                        '!date'      => '<strong>'.$date.'</strong>',
                        '!time_slot' => '<strong>'.$time_slot.'</strong>',
                    ))
                ?>
            </p>

            <p>
                <?= t('Afin de terminer votre réservation, veuillez choisir une salle parmi la liste suivante') ?>:
            </p>

            <div class="rooms cf">
                <?php foreach ($rooms as $room) : ?>
                    <?php if (in_array($room->nid, $reservations)) : ?>
                        <article class="col-md-3 room disabled" data-room-id="<?= $room->nid ?>">
                            <div class="overlay">
                                <div class="overlay-content">
                                    <p><?= t('Cette salle n\'est plus disponible sur ce créneau horaire.') ?></p>
                                </div>
                            </div>
                    <?php else : ?>
                        <article class="col-md-3 room" data-room-id="<?= $room->nid ?>">
                    <?php endif ?>
                            <h2 class="room-name"><?= $room->title ?></h2>
                            <p><?= $room->field_room_description[LANGUAGE_NONE][0]['value'] ?></p>
                        </article>
                <?php endforeach; ?>
            </div>

            <?php //print render($products_form) ?>

            <div class="actions">
                <?= render($reservation_form) ?>
            </div>
        </div>
    </div>
</div>