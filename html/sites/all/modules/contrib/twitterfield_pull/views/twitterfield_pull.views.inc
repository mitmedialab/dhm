<?php

/**
 * @file
 * Provide views data and handlers for field_tracker.module
 */

/**
 * Implements hook_views_data().
 */
function twitterfield_pull_views_data() {
  $data = array();

  $data['twitterfield_pull_tweet']['table']['group'] = t('Twitterfield Pull');

  // Advertise this table as a possible base table.
  $data['twitterfield_pull_tweet']['table']['base'] = array(
    'field' => 'tid',
    'title' => t('Twitterfield Pull message'),
    'help' => t('Twitterfield Pull status messages.'),
    'weight' => 10,
  );

  $data['twitterfield_pull_tweet']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // Field Name.
  $data['twitterfield_pull_tweet']['field_name'] = array(
    'title' => t('Field Name'),
    'help' => t('The label of the field which pull the tweets.'),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'twitterfield_pull_handler_filter_field',
    ),
  );

  // Tweet ID.
  $data['twitterfield_pull_tweet']['tid'] = array(
    'title' => t('Twitter status message ID'),
    'help' => t('The ID of the Twitter status message.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Tweet timestamp.
  $data['twitterfield_pull_tweet']['timestamp'] = array(
    'title' => t('Creation Date'),
    'help' => t('Display the time the Twitter message was created.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Twitter text.
  $data['twitterfield_pull_tweet']['text'] = array(
    'title' => t('Message text'),
    'help' => t('The text of the Twitter message.'),
    'field' => array(
      'handler' => 'twitterfield_pull_views_handler_field_xss',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // Twitter source.
  $data['twitterfield_pull_tweet']['source'] = array(
    'title' => t('Source'),
    'help' => t('The name of the application that posted the Twitter message.'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Twitter account full name.
  $data['twitterfield_pull_tweet']['user_screen_name'] = array(
    'title' => t('Full name'),
    'help' => t('The full name Twitter account user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Twitter login name.
  $data['twitterfield_pull_tweet']['name'] = array(
    'title' => t('Login name'),
    'help' => t('The login account of the Twitter user.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // Twitter account profile image.
  $data['twitterfield_pull_tweet']['user_profile_pic'] = array(
    'title' => t('Profile image'),
    'help' => t('The image used by the Twitter account.'),
    'field' => array(
      'handler' => 'twitterfield_pull_views_handler_field_profile_image',
      'click sortable' => TRUE,
    ),
  );

  return $data;
}


/**
 * Implements hook_views_handlers().
 */
function twitterfield_pull_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'track_field_changes') . '/views',
    ),
    'handlers' => array(
      'twitterfield_pull_views_handler_field_xss' => array(
        'parent' => 'views_handler_field',
      ),
      'twitterfield_pull_views_handler_field_profile_image' => array(
        'parent' => 'views_handler_field',
      ),
      'twitterfield_pull_handler_filter_field' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}
