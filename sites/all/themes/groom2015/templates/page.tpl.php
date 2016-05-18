<header id="navbar" role="banner" class="<?php // print $navbar_classes;                    ?> navbar navbar-default navbar-fixed-top">

    <div class="navbar-header">

        <?php if ($logo): ?>
            <a class="logo navbar-btn" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>">
                <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
            </a>
        <?php endif; ?>

        <?php if (!empty($site_name)): ?>
            <a class="name navbar-brand nopaddingleft" href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a>
        <?php endif; ?>

        <!-- .btn-navbar is used as the toggle for collapsed navbar content -->
        <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
            <span class="menu-item">Menu</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
        </button>
    </div>

    <h2 class="espace">Espace membre</h2>

    <?php if (user_is_logged_in()) : ?> <!-- Show Menu only if user is logged in -->

        <?php if (!empty($primary_nav) || !empty($secondary_nav) || !empty($page['navigation'])): ?>
            <div class="navbar-collapse collapse">
                <nav id="page-navigation" role="navigation">
                    <?php if (!empty($primary_nav)): ?>
                        <?php print render($primary_nav); ?>
                    <?php endif; ?>
                    <?php if (!empty($page['navigation'])): ?>
                        <?php print render($page['navigation']); ?>
                    <?php endif; ?>
                    <?php if (!empty($secondary_nav)): ?>
                        <?php print render($secondary_nav); ?>
                    <?php endif; ?>
                </nav>
            </div>
        <?php endif; ?>

    <?php endif; ?>

</header>

<div id="main" class="main-container container">

    <header role="banner" id="page-header">
        <?php if (!empty($site_slogan)): ?>
            <p class="lead"><?php print $site_slogan; ?></p>
        <?php endif; ?>

        <?php print render($page['header']); ?>
    </header> <!-- /#page-header -->

    <div class="row">

        <?php if (!empty($page['sidebar_first'])): ?>
            <aside class="col-sm-3" role="complementary">
                <?php print render($page['sidebar_first']); ?>
            </aside>  <!-- /#sidebar-first -->
        <?php endif; ?>

        <section<?php print $content_column_class; ?>>

            <?php if (user_is_logged_in()) : ?>
                <?php
                if (!empty($breadcrumb)): print $breadcrumb;
                endif;
                ?>
            <?php endif; ?>

            <?php if (user_is_logged_in()) : ?>

                <?php if (!empty($page['highlighted'])): ?>
                    <div class="highlighted jumbotron"><?php print render($page['highlighted']); ?></div>
                <?php endif; ?>

            <?php endif; ?>
            <a id="main-content"></a>
            <?php print render($title_prefix); ?>
            <?php if (!empty($title)): ?>
                <h1 class="page-header">
                    <?php if (arg(0) == 'user' && arg(1) == 'register') : ?>
                        <ul class="breadcrumb">
                            <li><a href="#" title="Nomade - Espace membre">Nomade - Espace Membre</a></li>
                        </ul>
                        Créer un compte membre
                    <?php elseif (arg(0) == 'user' && arg(1) == 'password') : ?>
                        <ul class="breadcrumb">
                            <li><a href="#" title="Nomade - Espace membre">Nomade - Espace Membre</a></li>
                        </ul>
                        Mot de passe oublié ?
                    <?php elseif (arg(0) == 'user' && arg(1) == 'login') : ?>
                        <ul class="breadcrumb">
                            <li><a href="#" title="Nomade - Espace membre">Nomade - Espace Membre</a></li>
                        </ul>
                        Bienvenue dans l'espace membre "Nomade"
                    <?php elseif (arg(0) == 'user' && $user->uid === 0) : ?>
                        <ul class="breadcrumb">
                            <li><a href="#" title="Nomade - Espace membre">Nomade - Espace Membre</a></li>
                        </ul>
                        Bienvenue dans l'espace membre "Nomade"
                    <?php elseif (arg(0) == 'user') : ?>
                        User Account
                    <?php else : ?>
                        <?php print $title ?>
                    <?php endif; ?>
                </h1>
            <?php endif; ?>
            <?php print render($title_suffix); ?>
            <?php if ($messages): ?>
                <div style="display:none">
                    <div id="messages">
                        <div class="section clearfix">
                            <?php print $messages; ?>
                        </div>
                    </div>
                </div>
            <?php endif; ?>
            <?php if (user_is_logged_in()) : ?>
                <?php
                global $user;
                $role = 'Membre';
                $user_roles = array_values($user->roles);
                ?>
                <?php if (in_array($role, $user_roles)) { ?>

                <?php } else { ?>
                    <?php if (!empty($tabs)):
                        ?>
                        <?php print render($tabs); ?>
                    <?php endif; ?>
                <?php } ?>
            <?php endif; ?>
            <?php if (!empty($page['help'])): ?>
                <?php print render($page['help']); ?>
            <?php endif; ?>
            <?php if (!empty($action_links)): ?>
                <ul class="action-links"><?php print render($action_links); ?></ul>
            <?php endif; ?>
            <?php print render($page['content']); ?>
        </section>

        <?php if (!empty($page['sidebar_second'])): ?>
            <aside class="col-sm-3" role="complementary">
                <?php print render($page['sidebar_second']); ?>
            </aside>  <!-- /#sidebar-second -->
        <?php endif; ?>

    </div>
    <div class="push"></div>
</div>
<footer class="footer navbar-default">
    <div class="container">
        <p class="copyright pull-left">
            <a href="/" title="Site Groom"><img src="/sites/all/themes/groom2015/images/logofooter.png" alt="Logo footer" /></a>&nbsp;&nbsp;Copyright © <?php echo date("Y"); ?> Groom
        </p>
        <?php print render($page['footer']); ?>
    </div>
</footer>

<script>
    (function ($) {
        if ($("#messages").length > 0) {
            $.fancybox.open([
                {
                    href: '#messages'
                }
            ]);
        }
        ;
    })(jQuery);
</script>