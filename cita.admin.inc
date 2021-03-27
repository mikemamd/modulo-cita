<?php

/**
 * Implements hook_form()
 */
function cita_admin_settings($form){
  $form['table'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Tabla Citas'),
    '#weight'      => 1,
    '#collapsible' => TRUE,
    '#collapsed'   => FALSE,
  );
  $form['table']['table'] = array(
    '#type'   => 'markup',
    '#prefix' => cita_admin_table(),
  );

  return $form;
}

/**
 * Implements theme_table()
 */
function cita_admin_table() {
  $headers = array(
    array(
      'data'  => t('ID'),
      'field' => 'id',
    ),
    array(
      'data'  => t('Cliente'),
      'field' => 'nombre_cliente',
    ),
    array(
      'data'  => t('Correo'),
      'field' => 'correo',
    ),
    array(
      'data'  => t('Telefono'),
      'field' => 'telefono',
    ),
    array(
      'data'  => t('Modelo'),
      'field' => 'tid_modelo',
    ),
    array(
      'data'  => t('Version'),
      'field' => 'tid_version',
    ),/*
    array(
      'data'  => t('VIN'),
      'field' => 'vin',
    ),*/
    array(
      'data'  => t('Fecha'),
      'field' => 'fecha',
    ),
    array(
      'data'  => t('Horario'),
      'field' => 'hora',
    ),
    array(
      'data'  => t('Distribuidor'),
      'field' => 'tid_dilers',
    ),
    array(
      'data' => t('Delete'),
    ),
  );

  $qry = db_select('cita', 'ac')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('ac')
    ->limit(10)
    ->orderByHeader($headers);
  $agenda   = $qry->execute();
  $rows     = [];
  $horarios = ['1' => 'Matutino', '2' => 'Vespertino'];
  foreach ($agenda as $row) {
    $rows[] = array(
      $row->id,
      $row->nombre_cliente,
      $row->correo,
      $row->telefono,
      get_name_modelo($row->tid_modelo),
      get_name_modelo($row->tid_version),
      //$row->vin,
      format_date($row->fecha),
      $horarios[$row->hora],
      $row->tid_dilers,
      l(t('Delete'), 'admin/config/cita/delete/'.$row->id),
    );
  }

  $output = theme('table', array(
    'header' => $headers,
    'rows'   => $rows
    )
  );
  $output .= theme('pager');

  return $output;
}

/**
 * implements hook_form()
 */


function cita_admin_delete_form($form, &$form_state, $id = null) {
  $form = array();
  $agenda = !empty($id)? (int) $id : null ;
  $question = '¿Está seguro de eliminar el registro?';
  if($agenda !== NULL ) {
    $form['agenda_id_delete'] = array(
      '#type'  => 'hidden',
      '#value' => $id,
    );
  }//end if
  $form['#submit'][] = 'cita_admin_delete_form_submit';

  return confirm_form(
    $form,
    $question,
    'admin/config/cita',
    t('Esta accion es irreversible.'),
    t('Confirmar'),
    t('Cancelar')
  );
}

/**
 * Implements hook_form_submit()
 */
function cita_admin_delete_form_submit($form, &$form_state) {

  if( isset($form_state['values']['agenda_id_delete']) && !empty($form_state['values']['agenda_id_delete']) ){
    $id = (int) $form_state['values']['agenda_id_delete'];
    $result = FALSE;
    if( isset($id) && !empty($id) && is_int($id) ) {
      $delete = db_delete('cita')
      ->condition('id', $id)
      ->execute();
      $result = !empty($delete) ? TRUE : FALSE;
    }//end if

  }else{
    form_set_error('cita_submit_delete', t('No es posible eliminar el registro'));
  }//end if
  $form_state['redirect'] = 'admin/config/cita';
}


/**
 * Devuelve el nombre del vehiculo/version para la tabla de admin.
 * @return string
 */
function get_name_modelo($tid){
  $query = db_select('taxonomy_term_data', 'tx');
  $query->condition('tid', $tid, '=');
  $query->fields('tx', ['name']);
  $results = $query->execute()->fetchAll();
  if (isset($results) && !empty($results)) {
    foreach ($results as $result) {
      $name = $result->name;
    }
    return $name;
  }
}

/**
 * Formulario de adminisración de contenido de emails
 */
function cita_email_settings() {
  $form        = array();
  $text_title  = variable_get('mail_title','');
  $text_policy = variable_get('mail_policy','');

  $form['mail_content'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Configuracion general de contenido'),
    '#weight'      => 1,
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE
  );
  $form['mail_content']['spacer_img'] = array(
    '#type'              => 'managed_file',
    '#title'             => t('Imagen spacer de contenido'),
    '#required'          => FALSE,
    '#upload_location'   => file_default_scheme() . '://module/cita',
    '#default_value'     => variable_get('spacer_img', ''),
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
    ),
  );
  $form['mail_content']['logo_img'] = array(
    '#type'              => 'managed_file',
    '#title'             => t('Logo que se mostrará en el email'),
    '#required'          => FALSE,
    '#upload_location'   => file_default_scheme() . '://module/cita',
    '#default_value'     => variable_get('logo_img', ''),
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
    ),
  );
  $form['mail_footer'] = array(
    '#type'        => 'fieldset',
    '#title'       => t('Configuracion de politicas de privacidad'),
    '#weight'      => 2,
    '#collapsible' => TRUE,
    '#collapsed'   => TRUE
  );
  $form['mail_footer']['mail_title'] = array(
    '#type'          => 'text_format',
    '#title'         => t( 'Título de los términos' ),
    '#description'   => t('Tíulo que destacará de los términos en footer de email'),
    '#default_value' => !empty($text_title) ? $text_title['value'] : '' ,
    '#rows'          => 3,
    '#format'        => 'filtered_html',
  );
  $form['mail_footer']['mail_policy'] = array(
    '#type'          => 'text_format',
    '#title'         => t( 'Términos y Condiciones' ),
    '#description'   => t('Términos y condiciones que aparecerán en footer de email'),
    '#default_value' => !empty($text_policy) ? $text_policy['value'] : '' ,
    '#rows'          => 3,
    '#format'        => 'filtered_html',
  );

  $files = array(
    'logo_img',
    'spacer_img',
  );

  foreach ($files as $file) {
    $fid = variable_get($file, '');
    if (isset($fid) && !empty($fid)) {
      $file = file_load($fid);
      if ($file->status == 0) {
        $file->status = FILE_STATUS_PERMANENT;
        file_save($file);
        file_usage_add($file, 'cita', 'cita', 1);
        drupal_set_message(t('Archivo guardado con éxito'), 'status');
      }//end if
    }//end if
  }//end foreach
  return system_settings_form($form);
}
