jQuery(document).ready(function ($)
{
    var $body = $('body');

    $('a.fancy').fancybox(
    {
        type      : 'ajax',
        autoSize  : false,
        width     : '50%',
        height    : 'auto',
        onUpdate  : function ()
        {
            var $form        = this.wrap.find('#groom-reservation-form');
            var selectedRoom = $form.find('select[name="groom_reservation"]').val();

            offFancyboxListeners(this);
            onFancyboxListeners(this);

            // Handle if a room has already been selected (VIP room, errors on form)
            if (selectedRoom !== '' && !$form.hasClass('active')) {
                this.wrap.find('.room[data-room-id="'+selectedRoom+'"]').trigger('click', [true]);
            }
        }
    });

    // On submission of a reservation form
    $body.on('submit', '#groom-reservation-form', function (e)
    {
        var $reservationContainer = $('#reservation-container');
        var $submitBtn            = $(this).find('.form-submit');
        var $formControls         = $(this).find('.form-control');

        $submitBtn.addBack($formControls).prop('disabled', true);

        e.preventDefault();
        $reservationContainer.addClass('form-pending');
        $.fancybox.showLoading();

        $.post(
            $(this).attr('action'),
            $(this).serialize(),
            function (data, status, jqXHR)
            {
                $.fancybox.inner.html(data);
                $reservationContainer.removeClass('form-pending');
                $.fancybox.update();
                groomRefreshBootstrapSelect();
            }
        )
        .always(function ()
        {
            $reservationContainer.removeClass('form-pending');
            $submitBtn.addBack($formControls).prop('disabled', false);
            $.fancybox.hideLoading();
        });
    });

    // On submission of a Nomade SOLO reservation form
    $body.on('submit', '#groom-reservation-solo-form', function (e)
    {
        var $reservationContainer = $('#reservation-container');

        e.preventDefault();

        if ($(this).data('confirm') !== true)
        {
            $reservationContainer.children('.reservation-content').addClass('hidden');
            var quantity = $(this).find('input[name=quantity]').val();

            if (Drupal.settings.groom.user_points < quantity) {
                $reservationContainer.children('.reservation-insufficient').removeClass('hidden');
            }
            else
            {
                $reservationContainer.children('.reservation-confirm').removeClass('hidden');
                $(this).data('confirm', true);
            }

            $.fancybox.update();
        }
        else
        {
            $.post(
                $(this).attr('action'),
                $(this).serialize(),
                function (data, status, jqXHR)
                {
                    $.fancybox.inner.html(data);
                    $.fancybox.update();
                }
            )
            .always(function ()
            {
                $.fancybox.hideLoading();
                $reservationContainer.removeClass('form-pending');
            });

            $reservationContainer.addClass('form-pending');
        }
    });

    $('.cal-day-disable a, .field_time_slot_type_plages.disabled a').on('click', function (e)
    {
        e.preventDefault();
    });

    function offFancyboxListeners(fancybox)
    {
        fancybox.wrap.off('click', '.btn-cancel');
        fancybox.wrap.off('click', '.btn-confirm');
        fancybox.wrap.off('input', '#solo-quantity-input');
        fancybox.wrap.off('submit', 'form.commerce-add-to-cart');
        fancybox.wrap.off('click', '.room:not(.disabled, .readonly)');
    }

    function onFancyboxListeners(fancybox)
    {
        var $form = fancybox.wrap.find('.groom-form');

        fancybox.wrap.on('click', '.room:not(.disabled, .readonly)', function (event, triggeredManually)
        {
            $('.room').removeClass('selected');
            $(this).addClass('selected');
            $form.find('select[name="groom_reservation"]').val($(this).data('room-id'));
            $form.addClass('active');

            $.fancybox.update();
        });

        fancybox.wrap.on('click', '.btn-close, .btn-cancel', function (e)
        {
            e.preventDefault();
            $.fancybox.close();
        });

        fancybox.wrap.on('click', '.btn-confirm', function (e)
        {
            e.preventDefault();
            $.fancybox.showLoading();
            $form.submit();
        });

        fancybox.wrap.on('input', '#solo-quantity-input', function (e)
        {
            $('#solo-units-price').text($(this).val());
            $('#groom-reservation-solo-form input[name=quantity]').val($(this).val());
        });
    }

    $.fn.groomModalUpdate = function ()
    {
        $.fancybox.update();
    };

    function groomRefreshBootstrapSelect()
    {
        $('.selectpicker').selectpicker();
    }
});
