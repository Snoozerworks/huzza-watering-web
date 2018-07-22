<?php
$_SERVER ['DOCUMENT_ROOT'] || ($_SERVER ['DOCUMENT_ROOT'] = str_replace ( getenv ( "SCRIPT_NAME" ), "", str_replace ( "\\", "/", getenv ( "SCRIPT_FILENAME" ) ) ));
require_once 'constants.inc';

/**
 *
 * @param int[] $params
 */
function sendParams($params) {
	$keep_ids = [ 
			PRM\LAST_ERR,
			PRM\ONTIME,
			PRM\TANK_VOL,
			PRM\REFRESH_RATE,
			PRM\P1_FLOW,
			PRM\P1_PUMPED_VOL,
			PRM\P1_RQST_VOL,
			PRM\P2_FLOW,
			PRM\P2_PUMPED_VOL,
			PRM\P2_RQST_VOL,
			PRM\P3_FLOW,
			PRM\P3_PUMPED_VOL,
			PRM\P3_RQST_VOL 
	];
	
	ksort ( $params );
	$payload = '';
	foreach ( $params as $id => $val ) {
		// Filter out read only parameters
		if (! in_array ( $id, $keep_ids )) {
			continue;
		}
		$payload .= pack ( 'CN', ( int ) $id, ( int ) $val );
	}
	
	if ($payload) {
		$prepend = pack ( 'C', ( int ) CMD\SET );
		$append = pack ( 'C', ( int ) PRM\NONE );
		$payload = $prepend . $payload . $append;
	}
	
	return $payload;
}

/**
 *
 * @param int[] $params
 */
function requestParams($params) {
	$keep_ids = [ 
			PRM\LAST_ERR,
			PRM\ONTIME,
			PRM\TANK_VOL,
			PRM\REFRESH_RATE,
			PRM\P1_FLOW,
			PRM\P1_PUMPED_VOL,
			PRM\P1_RQST_VOL,
			PRM\P2_FLOW,
			PRM\P2_PUMPED_VOL,
			PRM\P2_RQST_VOL,
			PRM\P3_FLOW,
			PRM\P3_PUMPED_VOL,
			PRM\P3_RQST_VOL,
			PRM\ADC1,
			PRM\ADC2,
			PRM\ADC3,
			PRM\ADC4 
	];
	
	$params = array_unique ( $params );	
	sort ( $params );
	
	$payload = '';
	foreach ( $params as $prm_id ) {
		// Filter out read only parameters
		if (! in_array ( $prm_id, $keep_ids )) {
			continue;
		}
		$payload .= pack ( 'C', ( int ) $prm_id );
	}
	
	if ($payload) {
		$prepend = pack ( 'C', ( int ) CMD\GET );
		$append = pack ( 'C', ( int ) PRM\NONE );
		$payload = $prepend . $payload . $append;
	}
	
	return $payload;
}

/**
 * Echo raw data as octet-stream.
 *
 * @param int[] $send_params
 *        	Array with key/value to push to device.
 * @param int[] $request_params
 *        	Array of indices to fetch from device.
 */
function respond($send_params, $request_params) {
	// Start output
	header ( 'Content-Type: application/octet-stream' );
	echo sendParams ( $send_params );
	echo requestParams ( $request_params );
}

