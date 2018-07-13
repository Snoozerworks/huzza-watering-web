<html>
<head></head>

<body>
	<h1>Raw HEX output of download</h1>

<?php
require_once 'constants.inc';
require_once '../../server_access/htmlize_func.inc';

/**
 *
 * @param int[] $payload
 * @return string
 */
function parseSetParam(&$payload, &$out) {
	$k = 100;
	while ( count ( $payload ) > 0 && $k > 0 ) {
		-- $k;
		$p = array_shift ( $payload );
		$descr = & addRow ( $out, $p, 'Parameter=' );
		
		if ($p > PRM\_END || $p < PRM\NONE) {
			$descr .= 'ERR, INVALID PARAMETER ID';
			return false;
		}
		
		$descr .= PRM\TR_PRM [$p];
		
		// prn_htmlize($row);
		
		if ($p == PRM\NONE) {
			return true;
		}
		
		$val = array_splice ( $payload, 0, 4 );
		$dec = ($val [0] << 24) + ($val [1] << 16) + ($val [2] << 8) + $val [3];
		foreach ( $val as $v ) {
			$descr = & addRow ( $out, $v );
		}
		$descr = "value=$dec";
	}
	return true;
}

/**
 *
 * @param int[] $payload
 * @return string
 */
function parseGetParam(&$payload, &$out) {
	while ( count ( $payload ) ) {
		$p = array_shift ( $payload );
		
		$descr = & addRow ( $out, $p, 'Parameter=' );
		if ($p > PRM\_END || $p < PRM\NONE) {
			$descr .= 'ERR, INVALID PARAMETER ID';
			return false;
		}
		
		$descr .= PRM\TR_PRM [$p];
		
		if ($p == PRM\NONE) {
			return true;
		}
	}
	return true;
}

/**
 *
 * @param int $b
 *        	Byte value
 * @param String $str
 *        	Optional. Description of byte.
 * @return String Description of byte by reference.
 */
function &addRow(&$arr, $b, $str = '') {
	$r = count ( $arr );
	$arr [$r] = array (
			sprintf ( "%02d", $r ),
			sprintf ( "0x%02X", $b ),
			$str 
	);
	return $arr [$r] [2];
}

/**
 * ***************************
 * END OF FUNCTIONS
 * ***************************
 */

$payload = file_get_contents ( 'http://skarmflyg.org/hw/download.php' );
$payload = str_split ( $payload );
$payload = array_map ( 'ord', $payload );

printf ( "\n<br/>%d bytes", count ( $payload ) );
printf ( "\n<br/>%s", date ( DateTime::ATOM ) );

// Next, command byte followed by data...
$out = [ ];

while ( count ( $payload ) ) {
	
	$c = array_shift ( $payload );
	
	switch ($c) {
		case CMD\GET :
			addRow ( $out, $c, 'Command=GET' );
			if (! parseGetParam ( $payload, $out )) {
				$payload = [ ];
			}
			break;
		
		case CMD\SET :
			addRow ( $out, $c, 'Command=SET' );
			if (! parseSetParam ( $payload, $out )) {
				$payload = [ ];
			}
			break;
		
		case CMD\NONE :
			addRow ( $out, $c, 'Command=NONE' );
			break;
		
		default :
			addRow ( $out, $c, 'Command=ERR, BAD COMMAND' );
			$payload = [ ];
			break;
	}
}

echo sprn_2d_array ( $out, [ 
		'Byte',
		'HEX',
		'Type',
		'Value' 
] );

?>

</body>

</html>