<?php

/**
 * @file
 * Include payment method settings callback, redirect form callbacks...
 */

/**
 * Payment method callback: settings form.
 */
function commerce_atos_settings_form($settings = NULL) {
  $form = array();
  $default_currency = commerce_atos_get_currency_code(commerce_default_currency());
  // Merge default settings into the stored settings array.
  $settings = (array) $settings + array(
    'commerce_atos_automatic_redirect' => TRUE,
    'commerce_atos_bank' => 'webaffaires',
    'commerce_atos_bin_path' => '',
    'commerce_atos_currency' => $default_currency,
    'commerce_atos_delayed_days' => 99,
    'commerce_atos_delayed_payment' => FALSE,
    'commerce_atos_display_means' => variable_get('commerce_atos_display_means', TRUE),
    'commerce_atos_generate_pathfile' => TRUE,
    'commerce_atos_header_flag' => 'yes',
    'commerce_atos_language_iso' => 'fr',
    'commerce_atos_logo_id' => '',
    'commerce_atos_logo_id2' => '',
    'commerce_atos_logo_path' => drupal_get_path('module', 'commerce_atos') . '/logos',
    'commerce_atos_merchant_id' => commerce_atos_mid('webaffaires'),
    'commerce_atos_merchant_country' => 'fr',
    'commerce_atos_mode' => COMMERCE_ATOS_DEFAULT_MODE,
    'commerce_atos_param_path' => '',
    'commerce_atos_payment_means' => 'CB,2,VISA,2,MASTERCARD,2',
  );

  // Demonstration or production ?
  $form['commerce_atos_mode'] = array(
    '#type' => 'select',
    '#title' => t('Mode'),
    '#options' => array(
      'demo' => t('Demonstration'),
      'prod' => t('Production'),
    ),
    '#default_value' => $settings['commerce_atos_mode'],
  );

  // Display or not the ssl text (header flag) above the payment means.
  $form['commerce_atos_header_flag'] = array(
    '#type' => 'select',
    '#title' => t('Display header flag'),
    '#description' => t('Display or not the ssl text (header flag) above the payment means.'),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
    '#default_value' => $settings['commerce_atos_header_flag'],
  );

  // Automatic redirect after payment.
  $form['commerce_atos_automatic_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Automatic redirect'),
    '#description' => t('Automatic redirect the customer to your website after payment.'),
    '#default_value' => $settings['commerce_atos_automatic_redirect'],
  );

  // Generate pathfile ?
  $form['commerce_atos_generate_pathfile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate automatically the pathfile'),
    '#description' => t("Uncheck this option only if you know what you're doing"),
    '#default_value' => $settings['commerce_atos_generate_pathfile'],
  );

  $form['commerce_atos_delayed_payment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delayed payment'),
    '#default_value' => $settings['commerce_atos_delayed_payment'],
  );

  $form['commerce_atos_delayed_days'] = array(
    '#type' => 'textfield',
    '#title' => t('Delayed days'),
    '#default_value' => $settings['commerce_atos_delayed_days'],
    '#states' => array(
      'invisible' => array(
        'input[name="parameter[payment_method][settings][payment_method][settings][commerce_atos_delayed_payment]"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['commerce_atos_bank'] = array(
    '#type' => 'select',
    '#title' => t('Bank'),
    '#options' => array(
      'etransactions' => t('Crédit Agricole (e-transactions)'),
      'sogenactif' => t('Société Générale (Sogenactif)'),
      'cyberplus' => t('Banque Populaire (Cyberplus)'),
      'mercanet' => t('BNP Paribas (Mercanet)'),
      'scelliusnet' => t('La Poste (Scellius)'),
      'sherlocks' => t('LCL (Sherlocks)'),
      'webaffaires' => t('Crédit du Nord (Webaffaires)'),
      'elysnet' => t('HSBC (Elys Net)'),
      'citelis' => t('Crédit Mutuel de Bretagne (Citélis)'),
      'sips' => t('Atos Worldline (Sips)'),
    ),
    '#default_value' => $settings['commerce_atos_bank'],
  );

  // Merchant ID.
  $form['commerce_atos_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID'),
    '#description' => t("Used only on production mode."),
    '#size' => 20,
    '#maxlength' => 15,
    '#default_value' => $settings['commerce_atos_merchant_id'],
    '#required' => TRUE,
    '#states' => array(
      'invisible' => array(
        'select[name="parameter[payment_method][settings][payment_method][settings][commerce_atos_mode]"]' => array('value' => COMMERCE_ATOS_DEFAULT_MODE),
      ),
    ),
  );

  // Merchant country.
  $form['commerce_atos_merchant_country'] = array(
    '#type' => 'select',
    '#title' => t('Merchant Country'),
    '#description' => t("Select the merchant country."),
    '#options' => commerce_atos_get_merchant_countries(),
    '#default_value' => $settings['commerce_atos_merchant_country'],
  );

  // Language on the payment server.
  $form['commerce_atos_language_iso'] = array(
    '#type' => 'select',
    '#title' => t('Language'),
    '#description' => t("Select the language used on payment pages. If you run a multilingual website, the user's language will override this setting."),
    '#options' => commerce_atos_get_languages(),
    '#default_value' => $settings['commerce_atos_language_iso'],
  );

  $form['commerce_atos_display_means'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display payment means logos'),
    '#description' => t('Display payment means logos next to the credit cart label on the checkout payment pane.'),
    '#default_value' => $settings['commerce_atos_display_means'],
  );

  $form['commerce_atos_payment_means'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment means'),
    '#description' => t("The payment methods allowed. See the documentation for that parameter."),
    '#size' => 30,
    '#maxlength' => 128,
    '#default_value' => $settings['commerce_atos_payment_means'],
    '#required' => TRUE,
  );

  $form['commerce_atos_currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#description' => t("Select the currency used, wil be overriden by the order currency if supported."),
    '#options' => commerce_atos_get_currency_code(NULL, TRUE),
    '#default_value' => $settings['commerce_atos_currency'],
  );

  $form['commerce_atos_logo_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to the logo directory'),
    '#description' => t('Path to the bank images directory (by default the module logos path). No beginning/trailing slash.'),
    '#default_value' => $settings['commerce_atos_logo_path'],
    '#required' => TRUE,
  );

  // The logo path on the bank server.
  $form['commerce_atos_logo_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to your logo 1 on the bank server'),
    '#description' => t('You can send your logo to the bank and insert the path here.'),
    '#default_value' => $settings['commerce_atos_logo_id'],
    '#maxlength' => 32,
    '#required' => FALSE,
  );

  // The logo path on the bank server.
  $form['commerce_atos_logo_id2'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to your logo 2 on the bank server'),
    '#description' => t('You can send your logo to the bank and insert the path here.'),
    '#default_value' => $settings['commerce_atos_logo_id2'],
    '#maxlength' => 32,
    '#required' => FALSE,
  );

  $form['commerce_atos_bin_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to the binaries'),
    '#description' => t('Full server-side path to the binaries (The directory must contains the request & response binaries, No trailing slash).'),
    '#default_value' => $settings['commerce_atos_bin_path'],
    '#element_validate' => array('commerce_atos_bin_path_validate'),
    '#required' => TRUE,
  );
  $form['commerce_atos_param_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to config files (param folder)'),
    '#description' => t('Full server-side path to the param directory (No trailing slash).'),
    '#default_value' => $settings['commerce_atos_param_path'],
    '#element_validate' => array('commerce_atos_param_path_validate'),
    '#required' => TRUE,
  );
  return $form;
}

/**
 * Array walk callback for the payment means.
 */
function _commerce_atos_payment_means(&$item, $key, $logo_path) {
  $item = (is_numeric($item) ? NULL : $logo_path . '/' . $item . '.gif');
}

/**
 * Validation callback for commerce_atos_bin_path element.
 */
function commerce_atos_bin_path_validate($element, &$form_state, $form) {
  // Check if there's a trailing slash in the path.
  if (substr($element['#value'], -1) == '/') {
    form_error($element, t("The binary path should not contain a trailing slash."));
  }
  // Check the binaries location.
  if (!is_file($element['#value'] . '/request') || !is_file($element['#value'] . '/response')) {
    form_error($element, t('Cannot locate the binaries.'));
  }
  // Check if the request binary is executable.
  if (!is_executable($element['#value'] . '/request')) {
    form_error($element, t('Cannot execute the request binary.'));
  }
  // Check if the request binary is executable.
  if (!is_executable($element['#value'] . '/response')) {
    form_error($element, t('Cannot execute the response binary.'));
  }
}

/**
 * Validation callback for commerce_atos_param_path element.
 */
function commerce_atos_param_path_validate($element, &$form_state, $form) {
  // Check if there's a trailing slash in the path.
  if (substr($element['#value'], -1) == '/') {
    form_error($element, t("The param path should not contain a trailing slash."));
  }
  // Retrieve the payment method settings form state values.
  $pm_settings = &$form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  if (!is_dir($element['#value'])) {
    form_error($element, t("The param path directory doesn't exist."));
  }
  // Check if the parmcom.<bank> is readable.
  $bank = $pm_settings['commerce_atos_bank'];
  if (!is_readable($element['#value'] . '/parmcom.' . $bank)) {
    form_error($element, t("The parmcom.%bank file couldn't be located", array('%bank' => $bank)));
  }
  // Now check the certif file.
  $merchant_id = ($pm_settings['commerce_atos_mode'] == COMMERCE_ATOS_DEFAULT_MODE ? commerce_atos_mid($bank) : $pm_settings['commerce_atos_merchant_id']);
  $merchant_country = $pm_settings['commerce_atos_merchant_country'];
  if (!is_readable($element['#value'] . '/certif.' . $merchant_country . '.' . $merchant_id)) {
    form_error($element, t("The certif.%merchant_country.%merchant_id file couldn't be located", array('%merchant_country' => $merchant_country, '%merchant_id' => $merchant_id)));
  }
}

/**
 * Submit callback for the payment method settings form.
 */
function commerce_atos_settings_form_submit($form, &$form_state) {
  $pm_settings = &$form_state['values']['parameter']['payment_method']['settings']['payment_method']['settings'];
  variable_set('commerce_atos_display_means', $pm_settings['commerce_atos_display_means']);
  // Set the payment means variable.
  if (!empty($pm_settings['commerce_atos_display_means'])) {
    $means = explode(',', $pm_settings['commerce_atos_payment_means']);
    $means = drupal_map_assoc($means);
    array_walk($means, '_commerce_atos_payment_means', $pm_settings['commerce_atos_logo_path']);
    variable_set('commerce_atos_payment_means_logos', array_filter($means));
  }
  // Generate the pathfile and the parmcom file only if checked.
  if (!empty($pm_settings['commerce_atos_generate_pathfile'])) {
    _commerce_atos_generate_files($pm_settings, $form_state);
  }
  // We need to refresh the static cache of the payment methods.
  drupal_static_reset('commerce_payment_methods');
}

/**
 * Payment method callback: redirect form.
 */
function commerce_atos_redirect_form($form, &$form_state, $order, $payment_method) {
  // Retrieve the user to maybe override the language of the payment platform.
  global $user;
  // Get the payment method settings.
  $pm_settings = &$payment_method['settings'];

  // Return an error if one of the path is not filled.
  if (empty($pm_settings['commerce_atos_bin_path']) || empty($pm_settings['commerce_atos_param_path'])) {
    drupal_set_message(t('Atos credit card payment is not configured for use yet.'), 'error');
    watchdog('commerce_atos', 'Atos credit card payment is not configured for use', array(), WATCHDOG_ERROR);
    return array();
  }

  // Retrieve the order informations.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Try to override the currency code with the order's currency.
  $currency_code = $order_wrapper->commerce_order_total->currency_code->value();
  $currency_codes = commerce_atos_get_currency_code();
  $currency_code = isset($currency_codes[$currency_code]) ? commerce_atos_get_currency_code($currency_code) : $pm_settings['commerce_atos_language_iso'];
  $merchant_id = $pm_settings['commerce_atos_mode'] == COMMERCE_ATOS_DEFAULT_MODE ? commerce_atos_mid($pm_settings['commerce_atos_bank']) : $pm_settings['commerce_atos_merchant_id'];

  // Check if the request binary is executable.
  if (!is_executable($pm_settings['commerce_atos_bin_path'] . '/request')) {
    drupal_set_message(t('A problem occurred during your payment request.'), 'error');
    watchdog('commerce_atos', 'Cannot execute the request binary.', array(), WATCHDOG_ERROR);
    return array();
  }

  // Trying to read the certif file.
  if (!is_readable($pm_settings['commerce_atos_param_path'] . '/certif.' . $pm_settings['commerce_atos_merchant_country'] . '.' . $merchant_id)) {
    drupal_set_message(t('A problem occurred during your payment request.'), 'error');
    watchdog('commerce_atos', 'Cannot read the certificate file.', array(), WATCHDOG_ERROR);
    return array();
  }

  // Trying to read the parmcom files.
  if (!is_readable($pm_settings['commerce_atos_param_path'] . '/parmcom.' . $pm_settings['commerce_atos_bank'])) {
    drupal_set_message(t('A problem occurred during your payment request.'), 'error');
    watchdog('commerce_atos', 'Cannot read the bank parmcom file.', array(), WATCHDOG_ERROR);
    return array();
  }
  // The request binary needs the realpath...
  if (!empty($pm_settings['commerce_atos_generate_pathfile'])) {
    $path = drupal_realpath('public://commerce_atos');
  }
  else {
    $path = $pm_settings['commerce_atos_param_path'];
  }
  if (!is_readable($path . '/parmcom.' . $merchant_id)) {
    drupal_set_message(t('A problem occurred during your payment request.'), 'error');
    watchdog('commerce_atos', "The parmcom file couldn't be readed.", array(), WATCHDOG_ERROR);
    return array();
  }
  // Retrieve the order balance instead of the order total, this allows you
  // to pay your order with multiple payment methods.
  $balance = commerce_payment_order_balance($order);
  $amount = str_pad($balance['amount'], 3, "0", STR_PAD_LEFT);
  // Trying to override the language with the user's language.
  $languages = commerce_atos_get_languages();
  $language = ((!empty($user->language) && isset($languages[_commerce_atos_map_iso_to_language($user->language)])) ? _commerce_atos_map_iso_to_language($user->language) : $pm_settings['commerce_atos_language_iso']);
  $settings = array(
    'amount' => $amount,
    'automatic_response_url' => url('commerce_atos/autoresponse/' . $order->order_id . '/' . $payment_method['instance_id'], array('absolute' => TRUE, 'https' => FALSE)),
    'cancel_return_url' => url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
    'customer_email' => $order->mail,
    'customer_id' => $order->uid,
    'payment_means' => $pm_settings['commerce_atos_payment_means'],
    'currency_code' => $currency_code,
    'header_flag' => $pm_settings['commerce_atos_header_flag'],
    'language' => $language,
    'merchant_country' => $pm_settings['commerce_atos_merchant_country'],
    'merchant_id' => $merchant_id,
    'normal_return_url' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
    'order_id' => $order->order_id,
    'transaction_id' => mt_rand(0, 9) . substr(time(), -5),
    'pathfile' => $path . '/pathfile',
  );

  // Automatic redirect after payment.
  if (!empty($pm_settings['commerce_atos_automatic_redirect'])) {
    $settings += array(
      'data' => 'NO_RESPONSE_PAGE',
    );
  }

  // Delayed payment.
  if (!empty($pm_settings['commerce_atos_delayed_payment'])) {
    $settings += array(
      'capture_mode' => 'VALIDATION',
      'capture_day' => $pm_settings['commerce_atos_delayed_days'],
    );
  }

  // Add your logo on the bank server.
  if (!empty($pm_settings['commerce_atos_logo_id'])) {
    $settings += array(
      'logo_id' => $pm_settings['commerce_atos_logo_id'],
    );
  }

  // Add a second logo stored on the bank server.
  if (!empty($pm_settings['commerce_atos_logo_id2'])) {
    $settings += array(
      'logo_id2' => $pm_settings['commerce_atos_logo_id2'],
    );
  }
  // Allow other modules to alter the settings sent to atos.
  drupal_alter('commerce_atos_request', $settings);

  // Check the pathfile after the hook_alter to allow developers
  // to alter the pathfile path.
  if (!is_readable($settings['pathfile'])) {
    drupal_set_message(t('A problem occurred during your payment request.'), 'error');
    watchdog('commerce_atos', 'Cannot read the pathfile.', array(), WATCHDOG_ERROR);
    return array();
  }
  // Store the request parameters in the order object.
  $order->data['atos']['request'] = $settings;
  $order->data['atos']['payment_method'] = $payment_method;
  commerce_order_save($order);
  return commerce_atos_order_form($form, $form_state, $order, $settings, $pm_settings);
}

/**
 * Build the payment form.
 *
 * @param $order
 *   The fully loaded order being paid for.
 * @param array $settings
 *   An array of settings used for the request.
 * @param array $pm_settings
 *   An array containing the payment method settings.
 *
 * @return array
 *   A renderable form array.
 */
function commerce_atos_order_form($form, &$form_state, $order, $settings, $pm_settings) {
  // Execute the request.
  $results = _commerce_atos_exec($pm_settings['commerce_atos_bin_path'] . '/request', $settings);
  $code = '';
  $error = '';
  if (is_array($results) && isset($results[1])) {
    $code = $results[1];
    $error = $results[2];
    $message = $results[3];
  }
  if (($code == '') && ($error == '')) {
    drupal_set_message(t('Request call error: request executable file not found.'), 'error');
    watchdog('commerce_atos', 'Request call error: request executable file not found.', array(), WATCHDOG_ERROR);
  }
  elseif ($code != 0) {
    drupal_set_message(t('Atos payment API call error.'), 'error');
    watchdog('commerce_atos', 'Atos payment API call error %error', array('%error' => $error), WATCHDOG_ERROR);
  }
  // Match the action url.
  if (!empty($message) && preg_match('/< *form[^>]*action *= *["\']?([^"\']*)/i', $message, $attribute)) {
    $form['payment_form'] = array(
      '#markup' => $message,
    );
    // Fill the action with the action attribute of the response.
    $form['#action'] = $attribute[1];
    return $form;
  }
  else {
    return array();
  }
}

/**
 * Payment method callback validation.
 */
function commerce_atos_redirect_form_validate($order, $payment_method) {
  $datas = array();
  if (!empty($payment_method['settings']['commerce_atos_automatic_redirect']) && isset($_GET['DATA'])) {
    $datas = $_GET['DATA'];
  }
  elseif (isset($_POST['DATA'])) {
    $datas = $_POST['DATA'];
  }
  if (empty($datas)) {
    watchdog('commerce_atos', 'Response attempted without any data.', array(), WATCHDOG_WARNING);
    return FALSE;
  }
  // Retrieve the binaries path.
  $bin_path = $payment_method['settings']['commerce_atos_bin_path'];
  // Retrieve the request params.
  $request = &$order->data['atos']['request'];
  $response = commerce_atos_parse_response($datas, $bin_path . '/response', $request['pathfile']);
  // Go back to the previous page if the response code isn't correct.
  if (!isset($response['code']) || ($response['code'] != 0)) {
    watchdog('commerce_atos', 'Response error: %error', array('%error' => $response['error']), WATCHDOG_ERROR);
    commerce_payment_redirect_pane_previous_page($order);
    return FALSE;
  }
  $dump_vars = array(
    'request' => $request,
    'response' => $response,
  );
  $dump = print_r($dump_vars, TRUE);
  // Check if the request and the response match.
  $invalid = _commerce_atos_request_response_match($request, $response, $dump);
  if ($invalid) {
    return FALSE;
  }
  $charge = array(
    'amount' => $request['amount'],
    'currency_code' => commerce_atos_get_currency_code($request['currency_code'], TRUE),
  );

  if (!isset($response['response_code']) || ($response['response_code'] != '00') || empty($response['authorisation_id'])) {
    commerce_atos_transaction($payment_method, $order, $charge, COMMERCE_PAYMENT_STATUS_FAILURE, $response);
    watchdog('commerce_atos', 'Invalid response code : <pre>%dump</pre>', array('%dump' => $dump), WATCHDOG_ERROR);
    return FALSE;
  }
  else {
    _commerce_atos_finalize_process($payment_method, $order, $charge, $response, $dump);
    return TRUE;
  }
}
