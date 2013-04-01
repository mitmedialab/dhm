
jQuery(document).ready(function($) {
  if ($("#block-superfish-1 .content ul").length > 0) {
    // Create the dropdown base
    $("<select id=\"main-menu-mobile\" />").appendTo("#region-menu .region-inner");
    $("#main-menu-mobile").mobileMenu({ ulsource: "#block-superfish-1 .content ul", maxLevel : 4 });
  }
  if ($('input[name="search_api_views_fulltext"]').length > 0) {
    $('input[name="search_api_views_fulltext"]').val('Search');
    $('input[name="search_api_views_fulltext"]').focus(function() {
      $(this).val('');
    });
  }
});
