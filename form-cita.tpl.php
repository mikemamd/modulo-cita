<?php
/*
 *
 */
drupal_add_js(drupal_get_path('module', 'cita') . '/js/cita.js');
?>

<div class="input-field">
  <?php print render($form["nombrecliente_cita"]);?>
  <label for="nombrecliente_cita" class="">Ingresa tu nombre completo</label>
</div>
<div class="input-field">
  <?php print render($form["email_cita"]);?>
  <label for="email_cita" data-error="Ingresa un correo válido">¿Cu&aacute;l es tu correo electr&oacute;nico?</label>
</div>
<div class="input-field">
  <?php print render($form["telefono_cita"]);?>
  <label for="telefono_cita" class="">T&eacute;lefono de contacto</label>
</div>
<div class="input-field">
  <?php print render($form["modelo_cita"]);?>
</div>
<div class="input-field">
  <?php print render($form["version_cita"]);?>
</div>
<!--<div class="input-field">
  <?php //print render($form["vin_cita"]);?>
  <label for="vin_cita" class="">Ingresa tu VIN</label>
</div>-->
<div class="contenedor">
   <div class="input-field">
     <?php print render($form["fecha_cita"]);?>
     <label for="fecha_cita" class="">Fecha</label>
   </div>
   <div class="input-field">
     <?php print render($form["hora_cita"]);?>
   </div>
</div>
<p><b>Selecciona un distribuidor</b></p>
<div class="codigo">
  <?php print render($form["distribuidor_cita"]);?><span></span>
  <dl>

  </dl>
  <button type="reset"></button>
</div>
<div class="input-field">
  <?php print render($form["submit"]); ?>
</div>
<?php print render($form["distribuidor_tid_cita"]);?>
<?php print drupal_render_children($form); ?>
