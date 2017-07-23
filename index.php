<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
<title>Växtvattnarmackapär</title>
</head>
<body>
	<h1>Logg</h1>

<?php
// include_once 'constants.inc';
$filename = 'log.txt';
date_default_timezone_set ( 'Europe/Stockholm' );

$handle = @fopen ( $filename, 'r' );
if (! $handle) {
	echo "Logg $filename saknas";
	return;
} else {
	// echo "\n<br/>FILE\n<br/>";
	echo "\n<pre>";
	while ( ($buffer = fgets ( $handle, 4096 )) !== false ) {
		echo $buffer;
	}
	if (! feof ( $handle )) {
		echo "Error: unexpected fgets() fail\n";
	}
	echo "</pre>";
}
fclose ( $handle );

?>

</body>
</html>