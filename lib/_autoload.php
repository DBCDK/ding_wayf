<?php

define('DING_WAYF_BASE_URL', 'https://bibliotek.dk');

/**
 * @file
 * Mock implementation of simpleSAML.
 */
class SimpleSAML_Auth_Simple {

  /**
   * Constructor, empty for now.
   */
  public function __construct($sp) {

  }

  public function isAuthenticated() {
    // @TODO handle errors
    $error = isset($_GET['error']) ? $_GET['error'] : FALSE;
    if ($error) {
      // handle this
    }

    // gateway returns attributes in $_POST if authentication goes well
    if (!empty($_POST['eduPersonTargetedID'])) {
      $this->setAttributes($_POST);
      return TRUE;
    }
    // user might already be logged in
    //check in $_SESSION
    else {
      $wayf_id = $this->getAttribute('eduPersonTargetedID');
      if (isset($wayf_id)) {
        return TRUE;
      }
    }
    // user is not authenticated
    return FALSE;
  }

  public function getAttributes() {
    return isset($_SESSION['wayf_login']) ? $_SESSION['wayf_login'] : NULL;
  }

  private function setAttributes($attributes) {
    //enrich attriubutes with login_type
    $loginType = isset($_GET['logintype']) ? $_GET['logintype'] : NULL;
    if ($loginType == 'wayf') {
      $loginType = 'wayf_id';
    }
    elseif ($loginType == 'nemlogin') {
      $loginType = 'nem_id';
    }
    else {
      // default
      $loginType = 'wayf_id';
    }

    $attributes['login_type'] = $loginType;
    $_SESSION['wayf_login'] = $attributes;
  }

  public function getAttribute($name) {
    return isset($_SESSION['wayf_login'][$name][0]) ? $_SESSION['wayf_login'][$name][0] : NULL;
  }

  /* \brief redirect to gatewayf for authentication via wayf
   *
   */

  public function requireAuth($idp = NULL) {
    global $base_url;
    $home = DING_WAYF_BASE_URL . '/' . current_path();
    $config = variable_get('ding_wayf');
    $gateway = $config['gatewayf'];

    header('Location:' . $gateway . '?returnUrl=' . $home . '&idp=' . $idp);
    exit;
  }

  /**
   * NOTICE; this logout does a redirect to log out of wayf, and thus
   * must fake a drupal-user logout to log drupal user out in a proper way
   */
  public function logout($url = NULL) {

    $type = $_SESSION['wayf_login']['login_type'];
    $idp = ($type == 'nem_id') ? 'nemlogin' : 'wayf';
    // unset session variables
    if (isset($_SESSION['wayf_login'])) {
      unset($_SESSION['wayf_login']);
    }

    global $base_url;
    $config = variable_get('ding_wayf');
    $gateway = $config['gatewayf'];

    // and now we fake a drupal logout before
    // the redirect takes place
    // log out drupal user
    // @see user/user.pages.inc::user_logout()
    global $user;
    if (isset($user->mail))
      watchdog('wayf', 'Session closed for %name.', array('%name' => $user->mail));
    watchdog('WAYF', $gateway . '?returnUrl=' . DING_WAYF_BASE_URL . '&op=logout', array(), WATCHDOG_ERROR);
    module_invoke_all('user_logout', $user);
    // Destroy the current session, and reset $user to the anonymous user.
    session_destroy();
    // redirect to gatewayf; pass returnUrl for simplesaml to redirect



    header('Location:' . $gateway . '?returnUrl=' . DING_WAYF_BASE_URL . '&op=logout' . '&idp=' . $idp);
    exit;
  }

}
