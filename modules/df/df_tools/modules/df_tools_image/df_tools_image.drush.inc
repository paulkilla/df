<?php

/**
 * Implements hook_drush_command().
 */
function df_tools_image_drush_command() {
  $items = array();

  $items['df_seed_derivatives'] = array(
    'description' => "Create image derivatives",
    'aliases' => array('df-sd', 'seed_derivatives'),
    'options' => array(
      'styles' => array(
        'description' => 'A comma separated list of all image styles needing generation, leaving this blank will use all styles',
        'example_value' => 'small,medium,thumbnail',
      ),
      'exclude' => array(
        'description' => 'A comma separated list of all image styles to be excluded. Leaving this blank will use all styles from the "styles" parameter.',
        'example_value' => 'large',
      ),
      'dir' => array(
        'description' => 'Set to the file path (relative to your public files directory) under which all files will have an image derivative created. Alternatively, set to "public" to just have your public files directory looked into with no recursion. With no option set, the default will run through every single file.',
        'example_value' => 'field/image',
      ),
    ),
    'examples' => array(
      'drush seed_derivatives' => 'Creates image derivatives for every single image style for every file within the public file system.',
      'drush seed_derivatives --styles=large,medium --dir=field' => 'Creates large and medium image derivatives for every single image stored underneath the "field" directory in the public file directory.',
      'drush seed_derivatives --exclude=thumbnail --dir=public' => 'Creates image derivatives for every single image style except "thumbnail" for every file within the public file system directory but not in subdirectories.',
    ),
    'core' => array('7'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
  );

  return $items;
}

/**
 * Command callback to perform the file migration.
 *
 * The options for this script are styles, exclude and dir. The styles and exclude
 * options will explicitly set or prohibit image styles to be created. The default
 * is that all styles will be used.
 * The dir parameter may be used to specify paths under the public files directory
 * that processing may occur on. The default for this is all paths and the special
 * path "public" will only process the public files directory without recursion.
 */
function drush_df_tools_image_df_seed_derivatives() {

  if (!module_exists("image")) {
    return drush_set_error("This script requires the image module to be enabled.");
  }

  $extensions = array('jpeg', 'jpg', 'gif', 'png');
  $mimetypes = array('image/jpeg', 'image/jpg', 'image/gif', 'image/png');

  $dir = rtrim(drush_get_option('dir'), '/');
  if ($dir == 'public') {
    $file_pattern = "[^\/]*"; // Finds anything that does not contain "/", should be fine.
  }
  else {
    $file_pattern = $dir ? $dir . ".+" : ".+";
  }

  $regex = "^public:\/\/(" . $file_pattern . ")\.(" . implode($extensions, '|') . ")$";

  // Query the database for files that match this pattern.
  $query = db_select('file_managed', 'f')
    ->condition('filemime', $mimetypes , 'IN')
    ->condition('uri', $regex, 'REGEXP');
  $total_count = $query->countQuery()->execute()->fetchField();

  drush_print(format_plural($total_count,
    '1 entry is to have its image derivatives created.',
    '@count entries are to have their image derivatives created.'));

  // Select the files to have derivatives created..
  $files = $query->fields('f', array('fid', 'filename', 'uri', 'timestamp'))
    ->execute()
    ->fetchAll();

  $excludes = explode(",", drush_get_option('exclude'));
  $includes = explode(",", drush_get_option('styles'));
  foreach (image_styles() as $name => $style) {
    if (!empty($excludes) && in_array($style['name'], $excludes)) {
      drush_print('Excluding ' . $name);
      continue;
    }
    if (empty($includes[0]) || in_array($style['name'], $includes)) {
      drush_print('Processing ' . $name, 0, NULL, FALSE);
      $count = 1;
      $last_progress = 0;
      foreach ($files as $file) {
        $derivative_uri = image_style_path($style['name'], $file->uri);

        if (!file_exists($derivative_uri)) {
          image_style_create_derivative($style, $file->uri, $derivative_uri);
        }

        $progress = round(($count / $total_count) * 100);
        if (($progress % 25 === 0 || $progress % 7 === 0) && $progress != $last_progress) {
          drush_print('...' . $progress . '%', 0, NULL, ($progress == 100));
          $last_progress = $progress;
        }
        $count++;
      }
    }
  }
}