<?php
/**
 * @file
 * Default theme implementation to present all user profile data.
 *
 * This template is used when viewing a registered member's profile page,
 * e.g., example.com/user/123. 123 being the users ID.
 *
 * Use render($user_profile) to print all profile items, or print a subset
 * such as render($user_profile['user_picture']). Always call
 * render($user_profile) at the end in order to print all remaining items. If
 * the item is a category, it will contain all its profile items. By default,
 * $user_profile['summary'] is provided, which contains data on the user's
 * history. Other data can be included by modules. $user_profile['user_picture']
 * is available for showing the account picture.
 *
 * Available variables:
 *   - $user_profile: An array of profile items. Use render() to print them.
 *   - Field variables: for each field instance attached to the user a
 *     corresponding variable is defined; e.g., $account->field_example has a
 *     variable $field_example defined. When needing to access a field's raw
 *     values, developers/themers are strongly encouraged to use these
 *     variables. Otherwise they will have to explicitly specify the desired
 *     field language, e.g. $account->field_example['en'], thus overriding any
 *     language negotiation rule that was previously applied.
 *
 * @see user-profile-category.tpl.php
 *   Where the html is handled for the group.
 * @see user-profile-item.tpl.php
 *   Where the html is handled for each item in the group.
 * @see template_preprocess_user_profile()
 *
 * @ingroup themeable
 */
$usr = user_load(arg(1));
$email = $usr->mail;

$avatar = file_create_url($usr->picture->uri);

$nom = render($user_profile['field_user_nom']);
$prenom = render($user_profile['field_user_prenom']);
$societe = render($user_profile['field_user_societe']);
$telephone = render($user_profile['field_user_telephone']);
$portable = render($user_profile['field_user_portable']);
$site_internet = render($user_profile['field_user_site_internet']);
$bio = render($user_profile['field_user_bio']);
$facebook = render($user_profile['field_user_facebook']);
$twitter = render($user_profile['field_user_twitter']);
$viadeo = render($user_profile['field_user_viadeo']);
$linkedin = render($user_profile['field_user_linkedin']);

$visibilite_identite = render($user_profile['field_user_visibilite_identite']);
$visibilite_coor = render($user_profile['field_user_visibilite_coor']);
$visibilite_bio = render($user_profile['field_user_visibilite_bio']);
?>

<div id="profile" class="profile"<?php print $attributes; ?>>

    <div class="col-md-9 nopaddingleft">
        <h2><?php print $prenom; ?> <?php print $nom; ?></h2>

        <ul class="info-user">
            <?php if (!empty($societe)) : ?><li><?php print $societe; ?></li><?php endif; ?>

            <?php if ($visibilite_bio == 'Afficher ces informations pour tous les membres') : ?>

                <?php if (!empty($site_internet)) : ?><li><?php print $site_internet; ?></li><?php endif; ?>
                <?php if (!empty($twitter)) : ?><li><span>Twitter : </span><a href="http://www.twitter.com/<?php print $twitter; ?>" target="blank" title="Voir le profil Twitter">@<?php print $twitter; ?></a></li><?php endif; ?>
                <?php if (!empty($facebook)) : ?><li><span>Facebook : </span><?php print $facebook; ?></li><?php endif; ?>
                <?php if (!empty($viadeo)) : ?><li><span>Viadeo : </span><?php print $viadeo; ?></li><?php endif; ?>
                <?php if (!empty($linkedin)) : ?><li><span>Linkedin : </span><?php print $linkedin; ?></li><?php endif; ?>
            <?php endif; ?>

            <?php if ($visibilite_coor == 'Afficher ces informations pour tous les membres') : ?>
                <?php if (!empty($email)) : ?><li><?php print $email; ?></li><?php endif; ?>
                <?php if (!empty($telephone)) : ?><li><?php print $telephone; ?></li><?php endif; ?>
                <?php if (!empty($portable)) : ?><li><?php print $portable; ?></li><?php endif; ?>
            <?php endif; ?>

        </ul>

    </div>

    <div class="col-md-3 nopaddingright">
        <?php if ( $usr->picture != null ) { ?>
            <img src="<?php print $avatar; ?>" />
        <?php } else { ?>
            <img src="/sites/default/files/default_images/avatar.jpg" />
        <?php } ?>
    </div>

    <div class="col-md-12 nopadding">
        <?php if ($visibilite_bio == 'Afficher ces informations pour tous les membres') : ?>

            <?php if (!empty($bio)) : ?>

                <h4>Bio</h4>
                <?php print $bio; ?>

            <?php endif; ?>

        <?php endif; ?>
    </div>

</div>
