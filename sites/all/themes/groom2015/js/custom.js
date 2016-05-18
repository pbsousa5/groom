/**
 * @file
 * A JavaScript file for the theme.
 *
 * In order for this JavaScript to be loaded on pages, see the instructions in
 * the README.txt next to this file.
 */

// JavaScript should be made compatible with libraries other than jQuery by
// wrapping it with an "anonymous closure". See:
// - https://drupal.org/node/1446420
// - http://www.adequatelygood.com/2010/3/JavaScript-Module-Pattern-In-Depth
(function ($, Drupal, window, document, undefined) {


// To understand behaviors, see https://drupal.org/node/756722#behaviors
    Drupal.behaviors.generalBehavior = {
        attach: function (context, settings) {

            // Active data-target boostrap in nav link
            $('#page-navigation .nav a').each(function ()
            {
                var hrefLink = $(this).attr('href');

                if (hrefLink.indexOf('#') !== 0)
                {
                    $(this).on('click', function (e) {
                        window.location = hrefLink;
                    });
                }
            });

            // Close modal window if user want purchase
            $(document).on('click', '.fancybox-wrap .btn-close', function () {
                $('.fancybox-close').trigger('click');
            });

            // Detail user in modal window
            if ($('body.page-membres').length)
            {
                $('a.fancybox').fancybox(
                        {
                            type: 'ajax',
                            autoSize: false,
                            width: '50%',
                            height: 'auto',
                            helpers: {
                                title: null
                            },
                            ajax: {
                                dataFilter: function (data) {
                                    return $(data).find('#profile')[0];
                                }
                            }
                        });

            }

            if ($('body.page-checkout').length)
            {
                // User profile fields
                var valueName = $('#edit-commerce-user-profile-pane-field-user-adresse-facturation-und-0-name-line').val();
                var valueAdress = $('#edit-commerce-user-profile-pane-field-user-adresse-facturation-und-0-thoroughfare').val();
                var valueCp = $('#edit-commerce-user-profile-pane-field-user-adresse-facturation-und-0-postal-code').val();
                var valueVille = $('#edit-commerce-user-profile-pane-field-user-adresse-facturation-und-0-locality').val();
                var valueSociete = $('#edit-commerce-user-profile-pane-field-user-adresse-facturation-und-0-organisation-name').val();
                var valueSiret = $('#edit-commerce-user-profile-pane-field-user-siret-facturation-und-0-value').val();
                var valueRcs = $('#edit-commerce-user-profile-pane-field-user-rcs-facturation-und-0-value').val();
                var valueTva = $('#edit-commerce-user-profile-pane-field-user-tva-facturation-und-0-value').val();

                // User billing information
                $('#edit-customer-profile-billing-commerce-customer-address-und-0-name-line').val(valueName);
                $('#edit-customer-profile-billing-commerce-customer-address-und-0-thoroughfare').val(valueAdress);
                $('#edit-customer-profile-billing-commerce-customer-address-und-0-postal-code').val(valueCp);
                $('#edit-customer-profile-billing-commerce-customer-address-und-0-locality').val(valueVille);
                $('#edit-customer-profile-billing-commerce-customer-address-und-0-organisation-name').val(valueSociete);
                $('#edit-customer-profile-billing-field-customer-siret-und-0-value').val(valueSiret);
                $('#edit-customer-profile-billing-field-customer-rcs-und-0-value').val(valueRcs);
                $('#edit-customer-profile-billing-field-customer-tva-communautaire-und-0-value').val(valueTva);

            }

            if ($('body.page-produits-et-services').html()) {
                var divs = $("div.product");
                for (var i = 0; i < divs.length; i += 3) {
                    divs.slice(i, i + 3).wrapAll("<div class='row-products clearfix'></div>");
                }
            }

        }
    };


})(jQuery, Drupal, this, this.document);