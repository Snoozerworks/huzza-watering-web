<?php
require_once 'constants.inc';

/**
 *
 * @param array $payload
 */
function parseGetParam(&$payload) {
	$out = [ ];
	while ( count ( $payload ) ) {
		$p = array_shift ( $payload );
		
		if ($p == PRM\NONE) {
			return implode ( ',', $out );
		}
		
		if ($p > PRM\_END || $p < PRM\NONE) {
			return false;
		}
		
		$out [] = sprintf ( "%X", $p );
	}
	return false;
}

/**
 *
 * @param array $payload
 */
function parseSetParam(&$payload) {
	$out = [ ];
	$k = 100;
	while ( count ( $payload ) > 0 && $k > 0 ) {
		-- $k;
		$p = array_shift ( $payload );
		if ($p == PRM\NONE) {
			return implode ( ',', $out );
		}
		
		if ($p > PRM\_END || $p < PRM\NONE) {
			return false;
		}
		
		$val = array_splice ( $payload, 0, 4 );
		$val = ($val [0] << 24) + ($val [1] << 16) + ($val [2] << 8) + $val [3];
		$out [] = sprintf ( "%X", $p );
		$out [] = $val;
		// $out .= sprintf ( "%x,%d", $p, $val );
	}
	return false;
}
ob_start ();

// $h = getallheaders ();
// echo "\r\nAll headers\r\n";
// var_dump ( $h );
// echo "\r\n\r\n";

$filename = 'log.txt';
date_default_timezone_set ( 'Europe/Stockholm' );

$handle = @fopen ( $filename, 'ab' );
if (! $handle) {
	return;
}

// Get raw data
// fprintf ( $handle, "\r\nOB start()" );

$payload = file_get_contents ( "php://input" );

// echo "\r\nRaw payload\r\n";
// var_dump ( $payload );

// echo "\r\nPayload bytes \r\n";
// $k = 0;
// for($k = 0; $k < strlen ( $payload ); $k ++) {
// printf ( "%X ", ord ( $payload [$k] ) );
// }

// echo "\r\n\r\n";
// printf ( "\r\n** Raw post data %s **", date ( 'Y-m-d H:i:s' ) );
// printf ( "\r\nLength %d", strlen ( $payload ) );

$payload = str_split ( $payload );
$payload = array_map ( 'ord', $payload );

$out = '';
while ( count ( $payload ) ) {
	
	$c = array_shift ( $payload );
	
	switch ($c) {
		case CMD\GET :
			// $out .= sprintf ( "\r\nGET\r\nParam" );
			$parsed = parseGetParam ( $payload );
			
			if ($parsed === false) {
				$out = sprintf ( "ERR,BAD PARAMETER REQUEST" );
				$payload = [ ];
			} else {
				$out .= "GET," . $parsed;
			}
			break;
		
		case CMD\SET :
			// $out .= sprintf ( "\r\nSET,\r\nParam\tValue" );
			$parsed = parseSetParam ( $payload );
			
			if ($parsed === false) {
				$out = sprintf ( "ERR,BAD PARAMETER DATA" );
				$payload = [ ];
			} else {
				$out .= "SET," . $parsed;
			}
			
			break;
		
		case CMD\NONE :
			$out .= sprintf ( "NONE," );
			break;
		
		default :
			$out = sprintf ( "ERR,BAD COMMAND" );
			$payload = [ ];
			break;
	}
}

$out = sprintf ( "\r\nTime,%s,%s", date ( 'Y-m-d H:i:s' ), $out );
fprintf ( $handle, $out );

$h = ob_get_flush ();
if (! empty ( $h )) {
	fprintf ( $handle, "\r\n\r\n*** ob_get_flush () ***\r\n%s", $h );
}

fclose ( $handle );
