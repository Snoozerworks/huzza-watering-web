<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
<title>Växtvattnarmackapär</title>
</head>
<body>

	<h1>Logg</h1>

	<a href="graphs.php">Grafer</a>

<?php
error_reporting ( E_ALL );
require_once ('../server_access/htmlize_func.inc');
require_once ('HuzzaWatering.class.inc');
require_once 'constants.inc';

date_default_timezone_set ( 'Europe/Stockholm' );

$filename = 'logs/D38833.txt';

$Log = new HuzzaWatering ();

$tz = new DateTimeZone ( 'Europe/Stockholm' );
$from_time = new DateTime ( 'now', $tz );
$from_time->sub ( new DateInterval ( 'P2Y' ) );

$chips = HuzzaWatering::listChips ();
$chip = HuzzaWatering::getChip ();

echo "\n<p>Chips<br/>\n";
foreach ( $chips as $v ) {
	$sel = ($v == $chip) ? '&gt;' : '';
	printf ( "<a href=\"?chip=%s\">%s %s</a><br/>\n", $v, $sel, $v );
}

$Log->Load ( $chip, $from_time );
$log_arr = $Log->getLog ();

echo "\n<h2>Parameters in uploaded data</h2>";
echo sprn_2d_array ( array (
		$Log->getFields () 
) );

$x = $Log->filterLog ( 50 );
echo "\n<h2>Filtered uploaded parameter values</h2>";
echo sprn_2d_array ( $x );

echo "\n<h2>Json filtered values</h2>";
$x = $Log->addNullFields ( $x, $Log->getFields () );
echo json_encode ( $x );

echo "\n<h2>As filtered array</h2>";
$x = $Log->asArray ( 50 );
echo sprn_2d_array ( $x );

echo "\n<h2>Uploaded parameter values</h2>";
echo sprn_2d_array ( $log_arr );

?>

</body>
</html>
