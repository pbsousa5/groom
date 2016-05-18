<div class="profile-panel">

    <nav id="profile-navbar" class="navbar">
        <ul class="nav navbar-nav">
            <li id="identite" class="active-trail active"><a href="#identite" title="Identité">1. <?php print t('Identity'); ?></a></li>
            <li id="coordonnees"><a href="#coordonnees" title="Coordonnées">2. <?php print t('Coordonnées'); ?></a></li>
            <li id="password"><a href="#password" title="Mot de passe">3. <?php print t('Password'); ?></a></li>
            <li id="bio"><a href="#bio" title="Bio">4. <?php print t('Bio'); ?></a></li>
            <li id="facturation"><a href="#facturation" title="Facturation">5. <?php print t('Facturation'); ?></a></li>
        </ul>
    </nav>

    <?php
    global $user;
    $user = user_load($user->uid);
    form_load_include($form_state, 'inc', 'user', 'user.pages');
    $user_form = drupal_get_form('user_profile_form', $user);

//    var_dump($user_form);
    ?>

    <div class="panels">

        <form enctype="multipart/form-data" action="/mon-compte/" method="post" id="user-profile-form" accept-charset="UTF-8" class="cf">

            <fieldset id="panel-identite" class="panel-fieldset active cf">
                <h2>1. <?php print t('Identity'); ?></h2>
                <div class="col-md-6 nopaddingleft">
                    <?php print drupal_render($user_form['field_user_prenom']); ?>
                </div>
                <div class="col-md-6 nopaddingright">
                    <?php print drupal_render($user_form['field_user_nom']); ?>
                </div>
                <div class="col-md-12 nopadding">
                    <?php print drupal_render($user_form['field_user_newsletter']); ?>
                    <?php print drupal_render($user_form['field_user_visibilite_identite']); ?>
                </div>
            </fieldset>

            <fieldset id="panel-coordonnees" class="panel-fieldset">
                <h2>2. <?php print t('Coordonnées'); ?></h2>
                <?php print drupal_render($user_form['field_user_adresse']); ?>
                <div class="col-md-4 nopaddingleft">
                    <?php print drupal_render($user_form['field_user_telephone']); ?>
                </div>
                <div class="col-md-4 nopadding">
                    <?php print drupal_render($user_form['field_user_portable']); ?>
                </div>
                <div class="col-md-4 nopaddingright">
                    <?php print drupal_render($user_form['account']['mail']); ?>
                </div>
                <div class="col-md-12 nopadding">
                    <?php print drupal_render($user_form['field_user_visibilite_coor']); ?>
                </div>
            </fieldset>

            <fieldset id="panel-password" class="panel-fieldset">
                <h2>3. <?php print t('Mot de passe'); ?></h2>
                <div class="col-md-4 nopaddingleft">
                    <?php print render($user_form['account']['current_pass']); ?>
                </div>
                <div class="col-md-8 nopaddingright">
                    <?php print render($user_form['account']['pass']); ?>
                </div>
            </fieldset>

            <fieldset id="panel-bio" class="panel-fieldset">
                <h2>4. <?php print t('Bio'); ?></h2>
                <div class="col-md-4 nopaddingleft">
                    <?php print drupal_render($user_form['field_user_societe']); ?>
                </div>
                <div class="col-md-4 nopadding">
                    <?php print drupal_render($user_form['field_user_statut']); ?>
                </div>
                <div class="col-md-4 nopaddingright">
                    <?php // print drupal_render($user_form['field_user_avatar']); ?>
                    <?php print drupal_render($user_form['picture']); ?>
                </div>
                <div class="col-md-4 nopaddingleft">
                    <?php print drupal_render($user_form['field_user_siret']); ?>
                </div>
                <div class="col-md-4 nopadding">
                    <?php print drupal_render($user_form['field_user_rcs']); ?>
                </div>
                <div class="col-md-4 nopaddingright">
                    <?php print drupal_render($user_form['field_user_tva']); ?>
                </div>
                <div class="col-md-12 nopadding">
                    <?php print drupal_render($user_form['field_user_bio']); ?>
                </div>
                <div class="col-md-4 nopaddingleft">
                    <?php print drupal_render($user_form['field_user_site_internet']); ?>
                </div>
                <div class="col-md-4 nopadding">
                    <?php print drupal_render($user_form['field_user_facebook']); ?>
                </div>
                <div class="col-md-4 nopaddingright">
                    <?php print drupal_render($user_form['field_user_twitter']); ?>
                </div>
                <div class="col-md-4 nopaddingleft">
                    <?php print drupal_render($user_form['field_user_viadeo']); ?>
                </div>
                <div class="col-md-4 nopadding">
                    <?php print drupal_render($user_form['field_user_linkedin']); ?>
                </div>
                <div class="col-md-12 nopadding">
                    <?php print drupal_render($user_form['field_user_visibilite_bio']); ?>
                </div>
            </fieldset>

            <fieldset id="panel-facturation" class="panel-fieldset">
                <div class="col-md-6 nopaddingleft bloc facturation">
                    <h2>5. <?php print t('Facturation'); ?></h2>
                    <?php echo views_embed_view('commerce_user_orders', $display_id = 'block_order_history'); ?>
                </div>
                <div class="col-md-6 bloc nopaddingright solde">
                    <h2><?php print t('Unités Solo'); ?></h2>
                    <?php echo views_embed_view('users_points', $display_id = 'block_unit_solo'); ?>
                </div>
                <div class="col-md-12 nopadding">
                    <h2><?php print t('Coordonnées facturation'); ?></h2>
                    <?php print drupal_render($user_form['field_user_adresse_facturation']); ?>
                    <div class="col-md-4 nopaddingleft">
                        <?php print drupal_render($user_form['field_user_siret_facturation']); ?>
                    </div>
                    <div class="col-md-4 nopadding">
                        <?php print drupal_render($user_form['field_user_rcs_facturation']); ?>
                    </div>
                    <div class="col-md-4 nopaddingright">
                        <?php print drupal_render($user_form['field_user_tva_facturation']); ?>
                    </div>
                </div>
            </fieldset>

            <div class="pull-right">
                <?php
                print drupal_render($user_form['form_id']);
                print drupal_render($user_form['form_build_id']);
                print drupal_render($user_form['form_token']);
                print drupal_render($user_form['options']['status']);
                print drupal_render($user_form['actions']);
                ?>
            </div>

        </form>
    </div>

</div>