jQuery(document).ready(function ($) {
    $('#profile-navbar li').on('click', function () {
        var hash = $('a', this).prop('hash');
        hash = hash.replace('#', '');
        $(this).parent().children().removeClass('active-trail active');
        $(this).addClass('active-trail active');
        $('.panels .panel-fieldset').removeClass('active');
        $('.panels #panel-' + hash).addClass('active');
    });

    if (window.location.hash) {
        var hash = window.location.hash.substring(1);
        $('#profile-navbar li').removeClass('active-trail active');
        $('#profile-navbar li#' + hash).addClass('active-trail active');
        $('.panels .panel-fieldset').removeClass('active');
        $('.panels #panel-' + hash).addClass('active');
    }
    
    // Add Boostrap class
    $('#panel-coordonnees .street-block').addClass('col-md-4 nopaddingleft');
    $('#panel-coordonnees .addressfield-container-inline').addClass('col-md-8 nopadding');
    $('#panel-coordonnees .addressfield-container-inline .form-type-textfield').addClass('col-md-6');
    $('#panel-coordonnees .addressfield-container-inline .form-item-field-user-adresse-und-0-locality').addClass('nopaddingright');
    $('#panel-coordonnees .addressfield-container-inline .form-item-field-user-adresse-und-0-postal-code').addClass('nopadding');
    $('#panel-facturation .name-block').addClass('col-md-4 nopaddingleft');
    $('#panel-facturation .locality-block').addClass('col-md-12 cf nopadding');
    $('#panel-facturation .locality-block .form-type-textfield').addClass('col-md-4');
    $('#panel-facturation .locality-block .form-item-field-user-adresse-facturation-und-0-postal-code').addClass('nopaddingleft');
    $('#panel-facturation .locality-block .form-item-field-user-adresse-facturation-und-0-locality').addClass('nopadding');
    $('.form-type-password-confirm .form-type-password').addClass('col-md-6');
    
    // Reconstruct form 
    var adressBlock = $('.street-block .form-item-field-user-adresse-facturation-und-0-thoroughfare').html();
    $('.street-block .form-item-field-user-adresse-facturation-und-0-thoroughfare').remove();
    $('.name-block').after('<div class="form-type-textfield form-item-field-user-adresse-facturation-und-0-thoroughfare form-item form-group col-md-8 nopadding">' + adressBlock + '</div>')

    var countryBlock = $('.form-item-field-user-adresse-facturation-und-0-country').html();
    $('.form-item-field-user-adresse-facturation-und-0-country').remove();
    $('#panel-facturation .locality-block').append('<div class="form-type-select form-item-field-user-adresse-facturation-und-0-country form-item form-group col-md-4 nopaddingright">' + countryBlock + '</div>');

});