jQuery(document).ready(function() {
  for (var id in Drupal.settings.parss_feeds) {
    var url = Drupal.settings.parss_feeds[id].url;
    var rows = Drupal.settings.parss_feeds[id].rows;
    var date_format = Drupal.settings.parss_feeds[id].date_format;
    var descriptions = Drupal.settings.parss_feeds[id].descriptions;
    jQuery('#' + id).PaRSS(
      url,
      rows,
      date_format,
      descriptions
    );
    jQuery('#' + id).show();
  }
});