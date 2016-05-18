<?php
$adresse = render($content['field_lieu_adresse']);
$coordonneesGps = render($content['field_lieu_geolocation']);
$latitude = $content['field_lieu_carte']['#items'][0]['lat'];
$longitude = $content['field_lieu_carte']['#items'][0]['lon'];
$map = render($content['field_lieu_carte']);
$email = render($content['field_lieu_email']);
$telephone = render($content['field_lieu_telephone']);
$responsable = render($content['field_lieu_responsables']);
$espace_coworking = render($content['field_lieu_nb_places_cowork']['#items'][0]['value']);
$salles = render($content['field_lieu_salles']);
$horaires = render($content['field_lieu_horaires']);
$logo = render($content['field_lieu_logo']['#items'][0]['uri']);
// $photos = render($content['field_lieu_photos']);
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

    <?php print render($title_prefix); ?>
    <?php if (!$page): ?>
        <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <div class="content"<?php print $content_attributes; ?>>

        <?php $logo_path = file_create_url($logo); ?>
        <!--<img src="<?php print $logo_path; ?>" alt="Logo <?php print $title; ?>" class="thumbnail img-responsive center-block"/>-->

        <div class="clearfix">

            <div class="col-md-6 nopaddingleft">
                <h2>Informations</h2>
                <p><?php print $salles; ?></p>
                <p><strong>Nombre de places de coworking : </strong><?php print $espace_coworking; ?></p>

                <h2>Responsable(s)</h2>

                <ul>
                    <?php $responsables = field_get_items('node', $node, 'field_lieu_responsables'); ?>
                    <?php if (!empty($responsables)): ?>
                        <?php foreach ($responsables as $item) : ?>
                            <li><?php print $item['value']; ?></li>
                        <?php endforeach; ?>
                    <?php endif; ?>
                </ul>

                <h2>Horaires</h2>

                <?php print $horaires; ?>

            </div>

            <div class="col-md-6 nopaddingright">
                <h2>Localisation</h2>
                <?php print $adresse; ?>
                <?php print $map; ?>
                <p><strong> Coordonnées GPS :</strong> <?php print $latitude; ?> / <?php print $longitude; ?></p>
                <p><?php print $email; ?></p>
                <p><a href="tel:<?php print $telephone; ?>"><?php print $telephone; ?></a></p>
            </div>

        </div>

        <div class="gallery">
            <ul class="thumbnails clearfix">
                <?php $photos = field_get_items('node', $node, 'field_lieu_photos'); ?>
                <?php if (!empty($photos)): ?>
                    <?php foreach ($photos as $photo) : ?>
                        <?php $file_path = file_create_url($photo['uri']); ?>
                        <?php $file_title = file_create_url($photo['title']); ?>
                        <li class="span3">
                            <img src="<?php print $file_path; ?>" alt="<?php print $file_title; ?>" />
                        </li>
                    <?php endforeach; ?>
                <?php endif; ?>
            </ul>
        </div>

    </div>

</div>
