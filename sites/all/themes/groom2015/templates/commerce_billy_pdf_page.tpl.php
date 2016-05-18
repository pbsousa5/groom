<?php
/**
 * @file
 * Template for PDFs.
 */
?><!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">
<html lang="en" dir="ltr" version="HTML+RDFa 1.1" xmlns:xsd="http://www.w3.org/2001/XMLSchema#">
<head profile="http://www.w3.org/1999/xhtml/vocab">
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <style type="text/css">
    <?php print $inline_css; ?>
  </style>
</head>
<body>

<?php
  $viewed_orders_rendered = array();
  foreach ($viewed_orders as $viewed_order) {
    $viewed_orders_rendered[] = render($viewed_order);
  }
  print implode('<div style="page-break-after: always;" />', $viewed_orders_rendered);
?>

</body></html>
