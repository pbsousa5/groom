(function($)
{
	Drupal.behaviors.groomReservationEditSlotTime =
	{
		attach : function (context, settings)
		{
			var $form = $('#groom-admin-reservation-form-content');

			$form.find('select[name="time_slot_type"]').on('change', function (e)
			{
				var $select = $form.find('select[name="time_slot"]');
				var label   = $(this).find('option[value="' + $(this).val() + '"]').text();

				if ($(this).val() === '')
				{
					$select.find('optgroup').prop('disabled', false);
					return;
				}

				$select.find('optgroup:not([label="' + label + '"])').prop('disabled', true);
				$select.find('optgroup[label="' + label + '"]').prop('disabled', false);

				if ($select.find('optgroup:not([label="' + label + '"]) option:selected').length)
				{
					$select.find('option:selected').prop('selected', false);
					$select.find('optgroup[label="' + label + '"] option:eq(0)').prop('selected', true);
				}
			})
			.trigger('change');
		}
	};

	Drupal.behaviors.groomUserAutoCompleteCallback =
	{
		attach : function (context, settings)
		{
			var $form    = $('#groom-admin-reservation-form-content');
			var $input   = $form.find('input[name="user-ac-input"]');
			var $inputId = $form.find('input[name="user_id"]');

			$input.on('ac-callback', function (event, data, ac)
			{
				var matches    = [];
				var nodesInfos = [];
				var searchStr  = $(ac.input).val();

				ac.db.cache[searchStr]     = [];
				ac.db.groomData            = ac.db.groomData ? ac.db.groomData : [];
				ac.db.groomData[searchStr] = data;

				for (var uid in data)
				{
					matches[uid]                = formatRow(data[uid]);
					ac.db.cache[searchStr][uid] = matches[uid];
					nodesInfos.push(data[uid]);
				}

				ac.found(matches);
			});

			if ($inputId.val() !== '' && typeof groom.users[$inputId.val()] !== 'undefined')
			{
				var userInfos = groom.users[$inputId.val()];
				var infos = {
					first_name : userInfos.fields.user_prenom,
					last_name  : userInfos.fields.user_nom,
					company    : userInfos.fields.user_societe,
					email      : userInfos.email,
				};

				$form.find('.user-infos .user-name span').text(formatUserName(infos));
				$form.find('.user-infos .user-email span').html($('<a>').text(userInfos.email).attr('href', 'mailto:'+userInfos.email));

				if (userInfos.fields.user_societe !== null) {
					$form.find('.user-infos .user-company span').text(userInfos.fields.user_societe);
				} else {
					$form.find('.user-infos .user-company span').text('-');
				}
			}
		}
	};

	var _setStatus = Drupal.jsAC.prototype.setStatus;
	Drupal.jsAC.prototype.setStatus = function (status)
	{
		_setStatus.call(this, status);

		if (status === 'found') {
			$(this.input).trigger('ac-callback', [this.db.cache[this.db.searchString], this]);
		}
	};

	var _select = Drupal.jsAC.prototype.select;
	Drupal.jsAC.prototype.select = function (node)
	{
		var searchString  = $(this.input).val();

		_select.call(this, node);

		var selectedValue = $(node).data('autocompleteValue');
		var infos         = this.db.groomData[searchString][selectedValue];
		var $form         = $('#groom-admin-reservation-form-content');

		$form.find('input[name="user_id"]').val(selectedValue);
		$form.find('.user-infos .user-name span').text(formatUserName(infos));
		$form.find('.user-infos .user-email span').html($('<a>').text(infos.email).attr('href', 'mailto:'+infos.email));

		if (infos.company !== null) {
			$form.find('.user-infos .user-company span').text(infos.company);
		} else {
			$form.find('.user-infos .user-company span').text('-');
		}

		$(this.input).val('');
	};

	function formatRow(infos)
	{
		var $output  = $('<span>');
		var $name    = $('<strong>');
		var $company = $('<em>');
		var $nid     = $('<span>').css(
		{
			color     : '#999',
			display   : 'inline-block',
			width     : 45,
			textAlign : 'right'
		});

		if (infos.first_name !== null && $.trim(infos.first_name) !== '') {
			$name.text(infos.last_name + ', ' + infos.first_name)
		} else {
			$name.text(infos.last_name);
		}

		$name.capitalize(true);

		if (infos.company !== null) {
			$company.text(' ' + infos.company);
		}

		$nid.text(infos.uid + ' | ');
		$output.append($nid);
		$output.append($name);
		$output.append($company);

		return $output;
	}

	function formatUserName(infos)
	{
		var output = infos.last_name;

		if ($.trim(infos.first_name) !== '') {
			output = infos.first_name + ' ' + infos.last_name;
		}

		return output;
	}

	$.fn.capitalize = function (toLowerBefore)
	{
		$.each(this, function ()
		{
			var split = $(this).text().split(' ');

			for (var i = 0, len = split.length; i < len; i++)
			{
				var str = split[i].slice(1);

				if (toLowerBefore) {
					str = str.toLowerCase();
				}

				split[i] = split[i].charAt(0).toUpperCase() + str;
			}

			$(this).text(split.join(' '));
		});

		return this;
	};

}) (jQuery);
