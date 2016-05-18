<div class="col-md-6 panel-login">

    <h3>Se connecter</h3>

    <?php
    print drupal_render($form['name']);
    print drupal_render($form['pass']);
    ?>


    <div class="password">
        <a href = "/user/password" title = "Mot de passe oublité ?" class="password">Mot de passe oublié ?</a>
    </div>

    <div class="submit">
        <?php
        print drupal_render($form['form_build_id']);
        print drupal_render($form['form_id']);
        print drupal_render($form['actions']);
        ?>
    </div>

</div>

<div class = "col-md-6 panel-registrer">
    <h3>Ouvrir un compte</h3>
    <div class="cf">
        <div class = "col-md-6 nopaddingleft register">
            <p><?php print t('Vous pouvez utiliser vos comptes sur les réseaux sociaux pour vous connecter.'); ?></p>
        </div>
        <div class = "col-md-6">
            <?php print drupal_render($form['hybridauth']); ?>
        </div>
    </div>
    <div class="register">
        <a href= "/user/register" title="Ouvrir un compte" class = "btn btn-primary">Ouvrir un compte gratuitement</a>
    </div>
</div>