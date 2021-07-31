<?
  header("Access-Control-Allow-Origin: *");   
  header('Access-Control-Allow-Methods: POST, GET, OPTIONS');
  header('Access-Control-Allow-Headers: X-Requested-With, content-type');

  if(!isset($_FILES['file']))
  {
    die();
  }
  $url = 'http://cloud.valky.eu:8382/compile';
  $post = array('file'=>curl_file_create($_FILES['file']['tmp_name']));
  $ch = curl_init();
  curl_setopt($ch, CURLOPT_URL,$url);
  curl_setopt($ch, CURLOPT_POST, 1);
  curl_setopt($ch, CURLOPT_POSTFIELDS, $post);
  curl_exec ($ch);
  curl_close ($ch);
?>