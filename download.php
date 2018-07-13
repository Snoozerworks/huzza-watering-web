<?php
require_once 'constants.inc';

// Chip id to identify the chip making the request
$chipid = (isset ( $_REQUEST ['cid'] )) ? $_REQUEST ['cid'] : - 1;

// Parameter values
$pump1_vol = 0 * 2 * 7200; // Pump flow request cc/day
$pump2_vol = 0 * 2 * 7200; // Pump flow request cc/day
$pump3_vol = 0 * 2 * 7200; // Pump flow request cc/day

$pump_flow = 70; // Pump flow capacity in cc/min.
$ontime = 120; // Pump run time per round.
$tank_volume = 4000; // Tank volume in cc
                     
// Connection interval in milliseconds for the IoT thing
$refresh_rate = 2 * 60 * 1000;
$refresh_rate = 10 * 1000;

//
// Send parameters to the Huzza
//

$end = pack ( 'C', ( int ) PRM\NONE );

// Start output
header ( 'Content-Type: application/octet-stream' );
echo pack ( 'C', ( int ) CMD\SET );
// // Reset pumped volume
// echo pack ( 'CN', ( int ) PRM\P1_PUMPED_VOL, ( int ) 0 );
// echo pack ( 'CN', ( int ) PRM\P2_PUMPED_VOL, ( int ) 0 );
// echo pack ( 'CN', ( int ) PRM\P3_PUMPED_VOL, ( int ) 0 );
echo pack ( 'CN', ( int ) PRM\P1_RQST_VOL, ( int ) $pump1_vol );
echo pack ( 'CN', ( int ) PRM\P2_RQST_VOL, ( int ) $pump2_vol );
echo pack ( 'CN', ( int ) PRM\P3_RQST_VOL, ( int ) $pump3_vol );
echo pack ( 'CN', ( int ) PRM\P1_FLOW, ( int ) $pump_flow );
echo pack ( 'CN', ( int ) PRM\P2_FLOW, ( int ) $pump_flow );
echo pack ( 'CN', ( int ) PRM\P3_FLOW, ( int ) $pump_flow );
echo pack ( 'CN', ( int ) PRM\TANK_VOL, ( int ) $tank_volume );
echo pack ( 'CN', ( int ) PRM\ONTIME, ( int ) $ontime );
echo pack ( 'CN', ( int ) PRM\REFRESH_RATE, ( int ) $refresh_rate );
// echo pack ( 'CN', ( int ) PRM\LAST_ERR, ( int ) 0 );

echo $end;

//
// Request parameters from the Huzza
//

echo pack ( 'C', ( int ) CMD\GET );

echo pack ( 'C', ( int ) PRM\ONTIME );
echo pack ( 'C', ( int ) PRM\REFRESH_RATE );
echo pack ( 'C', ( int ) PRM\P1_RQST_VOL );
// echo pack ( 'C', ( int ) PRM\P2_RQST_VOL );
// echo pack ( 'C', ( int ) PRM\P3_RQST_VOL );
echo pack ( 'C', ( int ) PRM\TANK_VOL );
echo pack ( 'C', ( int ) PRM\P1_PUMPED_VOL );
echo pack ( 'C', ( int ) PRM\P2_PUMPED_VOL );
echo pack ( 'C', ( int ) PRM\P3_PUMPED_VOL );
echo pack ( 'C', ( int ) PRM\ADC1 );
echo pack ( 'C', ( int ) PRM\ADC2 );
echo pack ( 'C', ( int ) PRM\ADC3 );
echo pack ( 'C', ( int ) PRM\ADC4 );
echo pack ( 'C', ( int ) PRM\LAST_ERR );
echo $end;
