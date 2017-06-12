<?php

function ding_wayf_admin_settings_form() {
  $form = array(
    '#tree' => TRUE,
  );

  $default = variable_get('ding_wayf', array(
    'installdir' => '/var/simplesamlphp',
    'redirect' => 'user/%uid/status',
  ));

  $form['ding_wayf']['installdir'] = array(
    '#type' => 'textfield',
    '#title' => t('Simple SAML php installation directory'),
    '#default_value' => $default['installdir'],
    '#description' => t('Absolute path with no trailing slash to simple SAML installation.'),
    '#required' => FALSE,
  );

  $form['ding_wayf']['sp'] = array(
    '#type' => 'textfield',
    '#title' => t('Autenticaton source for this SP'),
    '#default_value' => isset($default['sp']) ? $default['sp'] : 'default-sp',
    '#description' => t('The name of the service provider (SP) to use from /var/simplesamlphp/config/authsources.php.'),
    '#required' => TRUE,
  );

  $form['ding_wayf']['redirect'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect fallback URL'),
    '#default_value' => $default['redirect'],
    '#description' => t('Redirect to this fallback URL after login. This URL is only used if distination is not found. The token %uid is avaliable'),
    '#required' => TRUE,
  );
  
  $form['ding_wayf']['gatewayf'] = array(
    '#type' => 'textfield',
    '#title' => t('URL to wayf gateway'),
    '#default_value' => isset($default['gatewayf']) ? $default['gatewayf'] : 'https://dringo-ssl.dbc.dk/gatewayf',
    '#description' => t('Where to go for authentication via wayf',array(),array('context'=>'ding_wayf')),
    '#required' => TRUE,
  );

  $form['ding_wayf']['safetyhash'] = array(
    '#type' => 'textfield',
    '#title' => t('Salt to use for safety'),
    '#default_value' => isset($default['safetyhash']) ? $default['safetyhash'] : 'some salt',
    '#description' => t('Used for safety reasons',array(),array('context'=>'ding_wayf')),
    '#required' => TRUE,
  );
  
  // checkboxes wayf- and nemid-login
  $form['ding_wayf']['wayflogin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Wayf login', array(), array('context'=>'ding_wayf')),
    '#description' => t('Enable wayflogin',array(),array('context'=>'ding_wayf')),
    '#default_value' => isset($default['wayflogin']) ? $default['wayflogin'] : 0,
  );  
  

  $form['ding_wayf']['nemlogin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Nemlogin', array(), array('context'=>'ding_wayf')),
    '#description' => t('Enable Nemlogin',array(),array('context'=>'ding_wayf')),
    '#default_value' => isset($default['nemlogin']) ? $default['nemlogin'] : 0,
  );
  
  //*** Access platform ***//

  $form['ding_wayf']['hosturl'] = array(
    '#type' => 'textfield',
    '#title' => t('Host URL access platform'),
    '#default_value' => isset($default['host']) ? $default['host'] : 'https://login.bib.dk',
    '#description' => t('The name of the service provider (SP)'),
    '#required' => TRUE,
  );

  $form['ding_wayf']['loginpath'] = array(
    '#type' => 'textfield',
    '#title' => t('Logon path'),
    '#default_value' => isset($default['path']) ? $default['path'] : '/login',
    '#description' => t('Login path'),
    '#required' => TRUE,
  );

  $form['ding_wayf']['clienttoken'] = array(
    '#type' => 'textfield',
    '#title' => t('Token to use for access'),
    '#default_value' => isset($default['clienttoken']) ? $default['clienttoken'] : '1d33e62469d0e10d9b5899492fd43170a55a308a',
    '#description' => t('Token for accessing the access platform',array(),array('context'=>'access_platform')),
    '#required' => FALSE,
  );

  $form['ding_wayf']['clientsecret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client secret'),
    '#default_value' => isset($default['clientsecret']) ? $default['clientsecret'] : 'c9de7bd38020b03a8067b015a0e9b6b0e1ed785578c241f36e3f79a0ad1c485e',
    '#description' => t('Used for accessing a new token',array(),array('context'=>'access_platform')),
    '#required' => TRUE,
  );
  
  $form['ding_wayf']['clientid'] = array(
    '#type' => 'textfield',
    '#title' => t('Client id'),
    '#default_value' => isset($default['clientid']) ? $default['clientid'] : '2a26eef2-b5eb-4710-8389-bd237e566654',
    '#description' => t('Used for accessing a new token',array(),array('context'=>'access_platform')),
    '#required' => TRUE,
  );

  $form['ding_wayf']['clientplatform'] = array(
    '#type' => 'textfield',
    '#title' => t('Client platform'),
    '#default_value' => isset($default['clientplatform']) ? $default['clientplatform'] : 'http://platform-i01:3000/oauth/token',
    '#description' => t('Used for accessing a new token',array(),array('context'=>'access_platform')),
    '#required' => TRUE,
  );

  $form['ding_wayf']['heimdallogin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Adgangsplatform', array(), array('context'=>'access_platform')),
    '#description' => t('Enable Adgangsplatform',array(),array('context'=>'access_platform')),
    '#default_value' => isset($default['heimdallogin']) ? $default['heimdallogin'] : 0,
  );

  return system_settings_form($form);
}

/**
 * Validation function for the administration settings form above.
 */
function ding_wayf_admin_settings_form_validate($form, $form_state) {
  $wayf = isset($form_state['values']['ding_wayf']['wayflogin']) ? $form_state['values']['ding_wayf']['wayflogin'] : NULL;
  $nemlogin = isset($form_state['values']['ding_wayf']['nemlogin']) ? $form_state['values']['ding_wayf']['nemlogin'] : NULL;
  if( empty($wayf) && empty($nemlogin) ){
    form_set_error('wayflogin', t('Either check wayf, nemlogin or both.',array(),array('context' => 'ding_wayf')));
  }
  
 /* if (!ding_wayf_check_simplesamlphp($form_state['values']['ding_wayf']['installdir'])) {
    form_set_error('installdir', t('Simple SAML php was not found at the location given.'));
  }*/
}
