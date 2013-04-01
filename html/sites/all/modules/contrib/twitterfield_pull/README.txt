-----------------------------------------------------------------------------
ABOUT TwitterField Pull
-----------------------------------------------------------------------------
This module downloads all tweets from any preselected Twitterfield.
The module also provide an integration with views.
You can build any kind of block or page through the powerfull views module.

-----------------------------------------------------------------------------
CURRENT FEATURES
-----------------------------------------------------------------------------
- Support http://drupal.org/project/twitter_username
- Support http://drupal.org/project/twitterfield
- Show the user's Twitter account picture.
- Support views
- Views integration for unlimited displays.
- Determine how often tweets will be downloaded.
- Support #Hashtags and @Username
- Various datetime format for tweet creation time.

-----------------------------------------------------------------------------
REQUIREMENT
-----------------------------------------------------------------------------
Module: job_scheduler - 7.x-2.0-alpha3 (http://drupal.org/project/job_scheduler)
!!! There is currently a bug in job_scheduler
http://drupal.org/node/1423422
You need to apply the path
http://drupal.org/files/job_scheduler-1423422-2.patch
!! Bug in job_scheduler

Module: twitterfield (http://drupal.org/project/twitterfield)
Module: twitter_username (http://drupal.org/project/twitter_username)
Module: views (http://drupal.org/project/views)

-----------------------------------------------------------------------------
INSTALLATION
-----------------------------------------------------------------------------
1. Download and Enable the module

-----------------------------------------------------------------------------
USAGE
-----------------------------------------------------------------------------
1. Set up the content type and twitterfield you would like to pull:
/admin/config/system/twitterfield_pull
2. Use view to create your blocks or pages.

-----------------------------------------------------------------------------
Future roadmap
-----------------------------------------------------------------------------
1) Support multiple value field.

-----------------------------------------------------------------------------
Credit
-----------------------------------------------------------------------------
Maintainer and developer: targoo
