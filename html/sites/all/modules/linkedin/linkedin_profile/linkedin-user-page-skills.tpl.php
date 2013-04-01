<?php
$skills == $variables['skills'];
?>

<div class="linkin-skills">
  <ul>
<?php foreach ($skills as $skill) : ?>
       <li><?php print $skill; ?></li>
<?php endforeach; ?>
  </ul>
</div>
