INTRODUCTION
--------------------------

Default User Picture Styles allows you to apply image styles on the default
user picture by providing you with a file id (a fid). It also enhances
the default user picture field that comes bundled with Views by enabling you to
apply any desired image style.

This module does the downloading and processing of the default user image that
was specified via URL at admin/config/people/accounts and saves the fid in a
variable for future reference. It also defines a custom field handler used to
enhance the default user picture field in Views. 

Requirements: Views.

CONFIGURATION
--------------------------

This module requires no configuration. Just install and enjoy.

INFORMATION FOR DEVELOPERS
--------------------------

Developers can get the default user picture and apply image styles or any other
desired effect at any time quite easily. The following code example illustrates
this process:

$fid = variable_get('dup_default_user_image_fid', 0);
if ($fid) {
  $file = file_load($fid);
  $image = theme('image_style', array(
    'style_name' => 'thumbnail',
    'path' => $file->uri
  ));
}
