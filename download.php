<?php
require_once 'constants.inc';

// Parameter values
$pump1_vol = 0; // Pump flow cc/day
$pump2_vol = 0; // Pump flow cc/day
$pump3_vol = 0; // Pump flow cc/day

$pump_flow = 2000; // Pump flow capacity in cc/min.
$run_interval = 1 * 3600; // Interval in seconds between pump activations.
$tank_volume = 3000; // Tank volume in cc
                     
// Connection interval in milliseconds for the IoT thing
$refresh_rate = 2 * 60 * 1000;

//
// Send parameters to the Huzza
//

$end = pack ( 'C', ( int ) PRM\NONE );

// Start output
header ( 'Content-Type: application/octet-stream' );
echo pack ( 'C', ( int ) CMD\SET );

echo pack ( 'CN', ( int ) PRM\P1_RQST_VOL, ( int ) $pump1_vol );
echo pack ( 'CN', ( int ) PRM\P2_RQST_VOL, ( int ) $pump2_vol );
echo pack ( 'CN', ( int ) PRM\P3_RQST_VOL, ( int ) $pump3_vol );
echo pack ( 'CN', ( int ) PRM\P1_FLOW, ( int ) $pump_flow );
echo pack ( 'CN', ( int ) PRM\P2_FLOW, ( int ) $pump_flow );
echo pack ( 'CN', ( int ) PRM\P3_FLOW, ( int ) $pump_flow );
echo pack ( 'CN', ( int ) PRM\TANK_VOL, ( int ) $tank_volume );
echo pack ( 'CN', ( int ) PRM\RUN_INTERVAL, ( int ) $run_interval );
echo pack ( 'CN', ( int ) PRM\REFRESH_RATE, ( int ) $refresh_rate );

echo $end;

//
// Request parameters from the Huzza
//

echo pack ( 'C', ( int ) CMD\GET );

echo pack ( 'C', ( int ) PRM\REFRESH_RATE );
echo pack ( 'C', ( int ) PRM\RUN_INTERVAL );
echo pack ( 'C', ( int ) PRM\P1_RQST_VOL );
//echo pack ( 'C', ( int ) PRM\P2_RQST_VOL );
//echo pack ( 'C', ( int ) PRM\P3_RQST_VOL );
echo pack ( 'C', ( int ) PRM\P1_PUMPED_VOL );
//echo pack ( 'C', ( int ) PRM\P2_PUMPED_VOL );
//echo pack ( 'C', ( int ) PRM\P3_PUMPED_VOL );
echo pack ( 'C', ( int ) PRM\ADC1 );
//echo pack ( 'C', ( int ) PRM\ADC2 );
//echo pack ( 'C', ( int ) PRM\ADC3 );
//echo pack ( 'C', ( int ) PRM\ADC4 );

echo $end;
