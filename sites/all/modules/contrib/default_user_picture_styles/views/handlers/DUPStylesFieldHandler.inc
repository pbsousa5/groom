<?php
/**
 * @file
 * Provides User profile image or default profile image as views field.
 */

/**
 * Field handler to provide simple renderer.
 */
class DUPStylesFieldHandler extends views_handler_field_user_picture {

  /**
   * Calls parent init method.
   */
  public function init(&$view, &$options) {
    parent::init($view, $options);
  }

  /**
   * Loads parent options form. 
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  /**
   * Gets parent options. 
   */
  public function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Ensures required fields are added to the query. 
   */
  public function query() {
    $this->query->add_table('users', NULL, NULL, 'users');
    $join = new views_join();
    $join->construct('users', $this->query->base_table, 'uid', 'uid', array(), 'LEFT');
    $users_alias = $this->query->ensure_table('users', $this->relationship, $join);
    $this->query->add_field($users_alias, 'picture', 'users_picture');
    $this->query->add_field($users_alias, 'uid', 'uid');
  }

  /**
   * Renders the actual field. 
   */
  public function render($values) {
    if ($values->users_picture != '0') {
      $file = file_load((int) $values->users_picture);
    }
    else {
      $file = file_load(variable_get('dup_default_user_image_fid', 0));
    }
    if ($this->options['image_style']) {
      $image = theme('image_style', array('style_name' => $this->options['image_style'], 'path' => $file->uri));
    }
    else {
      $image = theme('image', array('path' => $file->uri));
    }
    $return_string = '';
    if ($this->options['link_photo_to_profile'] && user_access('access user profiles')) {
      $return_string = l($image, 'user/' . $values->uid, array('html' => TRUE));
    }
    else {
      $return_string = $image;
    }
    return $return_string;
  }
}
