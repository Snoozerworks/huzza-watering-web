<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
<title></title>

<script>
      var dojoConfig = {
        async: true,
      };
</script>

<script src="//ajax.googleapis.com/ajax/libs/dojo/1.12.1/dojo/dojo.js"></script>

</head>
<body>

	<h1>Fuktmätare</h1>

<?php
error_reporting ( E_ALL );
require_once ('../../server_access/htmlize_func.inc');
require_once ('HuzzaWatering.class.inc');
require_once 'constants.inc';

$chips = HuzzaWatering::listChips ();
$chip = HuzzaWatering::getChip ();
$from = HuzzaWatering::getHistoryLength ();
$n = HuzzaWatering::getSampleCount ();

echo "\n<p>Chips<br/>\n";
foreach ( $chips as $v ) {
	$sel = ($v == $chip) ? '&gt;' : '';
	printf ( "<a href=\"?chip=%s\">%s %s</a><br/>\n", $v, $sel, $v );
}
printf ( "\n<br/>Från: %s", $from->format ( DateTime::ATOM ) );

$Log = new HuzzaWatering ();
$Log->Load ( $chip, $from );
$log_arr = $Log->asArray ( $n );
printf ( "\n<br/>Punkter: %d</p>", count ( $log_arr ) );
$field_index = array_flip ( $Log->getFields () );

if (! $log_arr) {
	$log_arr = '{}';
	$field_index = '{}';
} else {
	$log_arr = json_encode ( $log_arr );
	$field_index = json_encode ( $field_index );
}

?>

Visa senaste
<a href="?history=P1W">månad</a>
	<a href="?period=P1W&samples=50">vecka</a>
	<a href="?period=P1D&samples=50">dag</a>
	<a href="?period=PT1H&samples=50">timme</a>
	<a href="?period=all&samples=50">allt</a>

	<script>
    require([
				 // Dojox chart resources
				"dojox/charting/Chart",
				//"dojox/charting/widget/Legend",
				"dojox/charting/widget/SelectableLegend",
				"dojox/charting/axis2d/Default",
				"dojox/charting/StoreSeries",
				"dojox/charting/plot2d/Lines",
				"dojox/charting/plot2d/Grid",
				"dojox/charting/action2d/MouseZoomAndPan",
		        "dojo/store/Memory",
				"dojox/charting/themes/Shrooms",
// 				"dojox/charting/themes/Wetland",
// 				"dojox/charting/themes/Julie",
// 				"dojox/charting/themes/Claro",
				// Wait until the DOM is ready
				"dojo/domReady!"
    ], function(Chart,
			Legend, 
    		DefaultAxis, 
    		StoreSeries,
			Lines,
			Grid,    	     
    	    MouseZoomAndPan,
    	    Memory,
    	    Theme){

		<?php
		echo <<<EOT

		var fields = $field_index;
		var json_data = $log_arr;
EOT;
		?>

		/*
		* Moisture level plots
		*/
		
        // Create dojox StoreSeries object
        var log1 = new StoreSeries(new Memory({data: json_data}), {}, {x: fields["time"], y: fields["s13"] });
        var log2 = new StoreSeries(new Memory({data: json_data}), {}, {x: fields["time"], y: fields["s14"] });
        var log3 = new StoreSeries(new Memory({data: json_data}), {}, {x: fields["time"], y: fields["s15"] });
        var log4 = new StoreSeries(new Memory({data: json_data}), {}, {x: fields["time"], y: fields["s16"] });

        // Create the chart
        var linesChart = new Chart("chartNode", {title: "Fuktighet"});

        // Set the theme
        linesChart.setTheme(Theme);

        // Add the only/default plot
        linesChart.addPlot("default",
        {
            type: Lines, // our plot2d/Lines module reference as type value
            labelOffset: -20,
        });

        // Add axis
        linesChart.addAxis("x", {
            labelFunc: function(text, value, precision) {
                var d = new Date();
                d.setTime(value*1000);                
                return d.toLocaleString().slice(2,-3);
            }
        });

        // Add axis
        //linesChart.addAxis("y", {vertical: true, includeZero: false});  
        linesChart.addAxis("y", {vertical: true, min:500, max:900});

        // Add the series of data
        linesChart.addSeries("Kruka 1", log1);
        linesChart.addSeries("Kruka 2", log2);
        linesChart.addSeries("Kruka 3", log3);
        linesChart.addSeries("Kruka 4", log4);

        // Add grid
        linesChart.addPlot("Grid", { 
            type: Grid,
            hAxis: "x",
            vAxis: "y",
            hMajorLines: true,
            hMinorLines: false,
            vMajorLines: true,
            vMinorLines: false,});

        // Add zoom and pan to chart
        new MouseZoomAndPan(linesChart, "default");
        new MouseZoomAndPan(linesChart, "default", {axis: "y"});

        // Render the chart!
        linesChart.render();

		// Add legend
		new Legend({chart: linesChart}, "legendNode");



		/*
		* Pumped volume plots
		*/

        var pump1 = new StoreSeries(new Memory({data: json_data}), {}, {x: fields["time"], y: fields["s7"] });
        var pump2 = new StoreSeries(new Memory({data: json_data}), {}, {x: fields["time"], y: fields["s8"] });
        var pump3 = new StoreSeries(new Memory({data: json_data}), {}, {x: fields["time"], y: fields["s9"] });
        

        // Create the chart
        var linesPumped = new Chart("chartPumped", {title: "Pumpad volym"});

        // Set the theme
        linesPumped.setTheme(Theme);

        // Add the only/default plot
        linesPumped.addPlot("chartPumped",
        {
            type: Lines, // our plot2d/Lines module reference as type value
            labelOffset: -20,
        });

        // Add axis
        linesPumped.addAxis("x", {
            labelFunc: function(text, value, precision) {
                var d = new Date();
                d.setTime(value*1000);                
                return d.toLocaleString().slice(2,-3);
            }
        });

        // Add axis
        linesPumped.addAxis("y", {vertical: true, includeZero: true});

        // Add the series of data
        linesPumped.addSeries("Pump 1", pump1);
        linesPumped.addSeries("Pump 2", pump2);
        linesPumped.addSeries("Pump 3", pump3);
        
        // Add grid
        linesPumped.addPlot("Grid", { 
            type: Grid,
            hAxis: "x",
            vAxis: "y",
            hMajorLines: true,
            hMinorLines: false,
            vMajorLines: true,
            vMinorLines: false,});

        // Add zoom and pan to chart
        new MouseZoomAndPan(linesPumped, "chartPumped");
        new MouseZoomAndPan(linesPumped, "chartPumped", {axis: "y"});
        
        // Render the chart!
        linesPumped.render();

		// Add legend
		new Legend({chart: linesPumped}, "legendPumped");

		

    });
</script>

	<!-- create the chart -->
	<div id="chartNode" style="width:100%; height: 350px;"></div>
	<div id="legendNode" style="width:100%; height: 50px;"></div>

	<div id="chartPumped" style="width: 100%; height: 350px;"></div>
	<div id="legendPumped" style="width: 100%; height: 50px;"></div>

</body>
</html>
