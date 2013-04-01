
function showlayer (layer, box) {
  var visible = (box.checked) ? "block" : "none";
  document.getElementById(layer).style.display = visible;
}
