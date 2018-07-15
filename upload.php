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
		
		$out [] = sprintf ( "%u", $p );
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
		$out [] = sprintf ( "%u", $p );
		$out [] = $val;
		// $out .= sprintf ( "%x,%d", $p, $val );
	}
	return false;
}

date_default_timezone_set ( 'Europe/Stockholm' );

$payload = file_get_contents ( "php://input" );
$payload = str_split ( $payload );
$payload = array_map ( 'ord', $payload );

// First 3 bytes are chip id
$chip_id = '';

if (count ( $payload ) > 3) {
	$val = array_splice ( $payload, 0, 3 );
	$val = ($val [0] << 16) + ($val [1] << 8) + $val [2];
	$chip_id = sprintf ( "%u", $val );
} else {
	// Ignore anything without a chip id
	return;
}

// Create directory and file if not exists.  
$dir = sprintf ("dev/%X", $chip_id);
if (!file_exists($dir)) {
	mkdir($dir, 0755, true);
}
$handle = @fopen ( sprintf ( "%s/%X.log", $dir, $chip_id  ), 'ab' );
if (! $handle) {
	return;
}

ob_start ();

// Next, command byte followed by data...
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

$out = sprintf ( "\r\nTime,%s,Id,%s,%s", date ( DateTime::ATOM ), $chip_id, $out );

fprintf ( $handle, $out );

$h = ob_get_flush ();
if (! empty ( $h )) {
	fprintf ( $handle, "\r\n\r\n*** ob_get_flush () ***\r\n%s", $h );
}

fclose ( $handle );
