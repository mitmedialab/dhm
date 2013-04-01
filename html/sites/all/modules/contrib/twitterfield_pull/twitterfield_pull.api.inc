<?php

/**
 * @file
 * Implementation of Twitter Field Pull API methods.
 */

/**
 * Update the Twitter Field Pull Tweet table with new tweets.
 *
 * Only tweets newer than last update will be saved.
 */
function _twitterfield_pull_tweet(&$job = array()) {

  $retrieve_name = $job['data']['name'];
  $current_tweet = (isset($job['data']['current_tweet'])) ? $job['data']['current_tweet'] : 1;

  // Check.
  if (!$retrieve_name) {
    watchdog('twitterfield_pull', 'Twitterfield Pull: No Twitter account or hashtag is set. New tweets can not be retrieved.', NULL, WATCHDOG_INFO);
    return;
  }
  elseif (strcmp(drupal_substr($retrieve_name, 0, 1), '@') === 0) {
    $retrieve_name = drupal_substr($retrieve_name, 1, drupal_strlen($retrieve_name));
    $type = 'user';
  }
  elseif (strcmp(drupal_substr($retrieve_name, 0, 1), '#') === 0) {
    $retrieve_name = drupal_substr($retrieve_name, 1, drupal_strlen($retrieve_name));
    $type = 'hashtag';
  }
  else {
    watchdog('twitterfield_pull', 'Twitterfield Pull: No valid twitter (@retrieve_name) account or hashtag is set. New tweets can not be retrieved.', array('@retrieve_name' => $retrieve_name), WATCHDOG_ERROR);
    return;
  }

  $current_tweet = _twitterfield_pull_get_last_tid($retrieve_name, $type);

  switch ($type) {
    case 'user':
      $rss_url = "http://api.twitter.com/1/statuses/user_timeline/{$retrieve_name}.xml?since_id={$current_tweet}";
      break;

    case 'hashtag':
      $rss_url = "http://search.twitter.com/search.rss?q=%23{$retrieve_name}&since_id={$current_tweet}";
      break;
  }

  watchdog('twitterfield_pull', 'Try to get tweets for @retrieve_name (@type) - Last tweet id is @$current_tweet - URL is @url', array(
    '@retrieve_name' => $retrieve_name,
    '@type' => $type,
    '@$current_tweet' => $current_tweet,
    '@url' => $rss_url)
  );

  $rss = drupal_http_request($rss_url, array('timeout' => 2));

  // If success.
  if ($rss->code == 200) {
    $xml = simplexml_load_string($rss->data);
    if ($xml !== FALSE) {
      switch ($type) {
        case 'user':
          $job['data']['progress'] = 0;
          $job['data']['max'] = count($xml->status);
          foreach ($xml->status as $tweet) {
            $created = strtotime($tweet->created_at);
            // Get tweet only if it is newer than the last update.
            try {
              $postid = db_merge(TWITTERFIELD_PULL_TABLE)
                  ->key(array('tid' => $tweet->id))
                  ->fields(array(
                    'nid' => $job['id'],
                    'field_name' => $job['type'],
                    'tid' => $tweet->id,
                    'timestamp' => $created,
                    'text' => $tweet->text,
                    'source' => $tweet->source,
                    'replyto' => $tweet->in_reply_to_screen_name,
                    'name' => $retrieve_name,
                    'type' => 'user',
                    'user_id' => $tweet->user->id,
                    'user_screen_name' => $tweet->user->name,
                    'user_profile_pic' => $tweet->user->profile_image_url,
                  ))
                  ->execute();
              $job['data']['progress']++;
              $job['data']['current_tweet'] = $tweet->id;
              $job['data']['message'] = t('Now processing @tweet', array('@tweet' => $tweet->id));
            } catch (Exception $e) {
              watchdog('Twitter DB: update', $e->getMessage());
            }
          }
          break;

        case 'hashtag':
          $job['data']['progress'] = 0;
          $job['data']['max'] = count($xml->channel->item);

          foreach ($xml->channel->item as $tweet) {
            $created = strtotime($tweet->pubDate);

            try {
              $urlvars = explode('/', $tweet->link);
              $tid = $urlvars[5];
              $user_name = substr($urlvars[3], 0, 50);
              $postid = db_merge(TWITTERFIELD_PULL_TABLE)
                  ->key(array('tid' => $tid))
                  ->fields(array(
                    'nid' => $job['id'],
                    'field_name' => $job['type'],
                    'tid' => $tid,
                    'timestamp' => $created,
                    'text' => $tweet->description,
                    'source' => '',
                    'replyto' => '',
                    'name' => $retrieve_name,
                    'type' => 'hashtag',
                    'user_id' => 0,
                    'user_screen_name' => $user_name,
                    'user_profile_pic' => '',
                  ))
                  ->execute();

              $job['data']['progress']++;
              $job['data']['current_tweet'] = $tid;
              $job['data']['message'] = t('Now processing @tweet', array('@tweet' => $tid));

            } catch (Exception $e) {
              watchdog('Twitter DB: update', $e->getMessage());
            }
          }

          break;
      }
      watchdog('twitterfield_pull', 'Finished pulling tweets for @retrieve_name (@type) - @progress new tweet',array(
        '@retrieve_name' => $retrieve_name,
        '@type' => $type,
        '@progress' => $job['data']['progress'])
      );
    }
  }
  else {
    $xml = simplexml_load_string($rss->data);
    if ($rss->code == 400) {
      $message = 'Error when retrieving tweets. ' . $xml->error;
    }
    else {
      $message = 'Error when retrieving tweets.';
    }
    watchdog('twitterfield_pull', $message);
  }

}


/*
 * Get Latest tweet id from specific name.
 * 
 */
function _twitterfield_pull_get_last_tid($name, $type) {
  $last_tid = db_select('twitterfield_pull_tweet','t')
    ->fields('t', array('tid'))
    ->condition('name', $name)
    ->condition('type', $type)
    ->orderBy('tid', 'DESC')
    ->range(0, 1)
    ->execute()
    ->fetchObject();
  if ($last_tid) {
    return $last_tid->tid;
  }
  else {
    return 1;
  }
}


/*
 * Save Job in schedule DB.
 * 
 */
function _twitterfield_pull_set_job($nid, $field, $data, $period = 3600) {
  $job = array(
    'id' => $nid,
    'type' => $field,
    'data' => $data,
    'period' => $period,
    'periodic' => TRUE,
  );
  JobScheduler::get('twitterfield_pull')->set($job);
}


/*
 * Delete Job in schedule DB.
 */
function _twitterfield_pull_delete_job($nid, $field) {
  $job = array(
    'id' => $nid,
    'type' => $field,
  );
  JobScheduler::get('twitterfield_pull')->remove($job);
}

/**
 * Helper hook_node_save().
 */
function _twitterfield_pull_node_save($node) {

  // Each node type which are enabled.
  $node_types = variable_get('twitterfield_pull_node_types', array());
  $frequency = variable_get('twitterfield_pull_update_frequency', 3600);

  // If content type is selected.
  if (in_array($node->type, $node_types)) {
    $selected_fields = twitterfield_pull_get_selected_field($node->type);

    foreach ($selected_fields as $field_name) {
      $field_info = field_info_field($field_name);

      // Delete old job scedulers.
      _twitterfield_pull_delete_job($node->nid, $field_name);

      $language = field_language('node', $node, $field_name);
      $fields = field_get_items('node', $node, $field_name, $language);

      foreach ($fields as $field) {

        switch ($field_info['type']) {
          case 'twitter_username':
            $name = ($field['twitter_username'] != '') ? '@' . $field['twitter_username'] : '';
            break;

          default:
            $name = ($field['value'] != '') ? $field['value'] : '';
        }

        if ($node->nid && $name != '') {
          $data = array('name' => $name, 'field_name' => $field_name);
          _twitterfield_pull_set_job($node->nid, $field_name, $data, $frequency);
        }

      }

    }
  }

}

/**
 * Batch callback for importing all the tweets.
 */
function twitterfield_pull_bacth() {
  drupal_set_message(t('start batch'));
  // Get all jobs done.
  $jobs = db_select('job_schedule', NULL, array('fetch' => PDO::FETCH_ASSOC))
    ->fields('job_schedule')
    ->condition('name', 'twitterfield_pull')
    ->condition('scheduled', 0)
    ->execute();
  foreach ($jobs as $job) {
    drupal_set_message(t('start processing @name', array('@name' => unserialize($job['data']))));
    _twitterfield_pull_tweet($job);
  }
  return '';
}


/**
 * Get selected twitter fields for a bundle.
 *
 * @param $bundle
 */
function twitterfield_pull_get_selected_field($bundle) {
  $track_field_changess = db_select('twitterfield_pull', 'a')->fields('a')->condition('bundle', $bundle)->execute()->fetchCol();
  $options = array();
  foreach ($track_field_changess as $track_field_changes) {
    $options[$track_field_changes] = $track_field_changes;
  }
  return $options;
}
