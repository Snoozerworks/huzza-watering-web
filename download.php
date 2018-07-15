<?php
require_once 'download2.inc';
require_once 'constants.inc';

// Chip id to identify the chip making the request
$chipid = (isset ( $_REQUEST ['cid'] )) ? $_REQUEST ['cid'] : - 1;

$send_params = [ 
		// PRM\LAST_ERR=>0, // Error code
		PRM\ONTIME => 120, // Pump run time per round in seconds.
		PRM\TANK_VOL => 4000, // Tank volume in cc,
		PRM\REFRESH_RATE => 5 * 1000, // Connection interval in milliseconds for the IoT thing
		
		// P1 nutrition pump 
		PRM\P1_FLOW => 70, // Pump flow capacity in cc/min.
		PRM\P1_PUMPED_VOL=>0, // Accumulated pumped volume
		PRM\P1_RQST_VOL => 0, // Pump flow request cc/day
		
		// Water pump
		PRM\P2_FLOW => 450,
		PRM\P2_PUMPED_VOL => 0,
		PRM\P2_RQST_VOL => 0,
		
		// Pump 3 not used
		PRM\P3_FLOW => 70,
		PRM\P3_PUMPED_VOL => 0,
		PRM\P3_RQST_VOL => 0 
];

 $request_params = [
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

respond($send_params, $request_params);

