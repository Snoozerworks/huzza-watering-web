<?php
/**
 * Some functions to get log file contents as php array.
 */
require_once 'constants.inc';
class HuzzaWatering {
	private $pids = [ ]; // Parameter names
	private $log = [ ]; // Log
	                    // public $from_time = null; // DateTime object for first data
	                    // public $interval = null; // DateInterval object for time interval of data to inclued
	function __construct() {
		$this->from_time = new DateTime ( 'now', new DateTimeZone ( 'Europe/Stockholm' ) );
		$this->interval = new DateInterval ( 'P2D' );
	}
	
	/**
	 * Get log file field names (from first row only).
	 */
	function getFields() {
		return $this->pids;
	}
	
	/**
	 * Get log file field names (from first row only).
	 */
	function getLog() {
		return $this->log;
	}
	
	/**
	 * Load log for given chip and returns it as an array.
	 * Only SET commands preserved.
	 *
	 * @param string $chip
	 *        	Optional. Use first available chip by default.
	 * @param DateTime $from
	 *        	Optional. Include data for last 2 days by default.
	 */
	function Load($chip = '', DateTime $from = null) {
		if (! $chip) {
			$chip = HuzzaWatering::getChip ();
		}
		$filename = 'logs/' . $chip . '.txt';
		if (! file_exists ( $filename )) {
			return;
		}
		$from = is_null ( $from ) ? $this::getHistoryLength () : $from;
		
		$log_arr = [ ];
		
		// Open file and read line by line.
		$handle = @fopen ( $filename, 'r' );
		if ($handle) {
			while ( ($buffer = fgets ( $handle, 2048 )) !== false ) {
				$row = explode ( ',', $buffer );
				if (! isset ( $row [1] )) {
					// No timestamp. Skip
					continue;
				}
				
				$date = DateTime::createFromFormat ( DateTime::ATOM, $row [1] );
				if ($date < $from) {
					continue;
				}
				if ($row [4] != 'SET') {
					continue;
				}
				
				$vals ['time'] = $date->getTimestamp ();
				$vals ['timestr'] = $row [1];
				$vals ['id'] = $row [3];
				$vals ['CMD'] = CMD\SET;
				
				$row = array_slice ( $row, 5 );
				
				$len = count ( $row );
				$pid = '';
				for($k = 0; $k < $len; ++ $k) {
					if ($k % 2) {
						$vals [$pid] = $row [$k];
					} else {
						$pid = sprintf("s%d",$row [$k]);
						$this->pids [$pid] = null;
						$vals [$pid] = null;
					}
				}
				array_push ( $log_arr, $vals );
			}
			if (! feof ( $handle )) {
				echo "Error: unexpected fgets() fail\n";
			}
			fclose ( $handle );
		}
		
		$this->log = $log_arr;
		
		if (! empty ( $this->log )) {
			$this->pids = array_keys ( $this->pids );
			array_unshift ( $this->pids, 'time', 'timestr', 'id', 'CMD' );
		}
	}
	
	/**
	 * Filter log values
	 *
	 * Splits data into at most $n number of chunks equally distributed from first
	 * to last timestamp. For each chunk values of field $f is averaged. Returns an
	 * array of arrays with average timestamp and field values.
	 * A time period that does not contain sampels will be skipped.
	 *
	 * @param integer $n
	 * @return array
	 */
	function filterLog($n) {
		if (count ( $this->log ) <= 1) {
			return [ ];
		}
		
		$log = $this->log;
		
		$n = max ( 1, min ( $n, count ( $log ) ) );
		$tlast = end ( $log ) ['time'];
		$t = reset ( $log ) ['time'];
		
		$dt = ($tlast - $t) / $n;
		
		$v_agg = [ ]; // Value sum
		$v_cnt = [ ]; // Value count
		$filtered = [ ];
		
		$k = 0;
		$kl = count ( $log );
		while ( $k < $kl ) {
			
			foreach ( $log [$k] as $pid => $val ) {
				if (in_array ( $pid, [ 
						'time',
						'timestr',
						'id',
						'CMD' 
				] )) {
					continue;
				}
				if (array_key_exists ( $pid, $v_agg )) {
					$v_agg [$pid] += $val;
					++ $v_cnt [$pid];
				} else {
					$v_agg [$pid] = $val;
					$v_cnt [$pid] = 1;
				}
			}
			
			if ($log [$k] ['time'] - $t - $dt >= - 1e-3) {
				$filtered [] = $this->sumAvg ( ( int ) ($t + $dt / 2), $v_agg, $v_cnt );
				$t += $dt;
				$v_agg = [ ];
				$v_cnt = [ ];
			}
			
			++ $k;
		}
		
		if (count ( $filtered ) != $n) {
			printf ( "\n<br/>Filter count = %d should be %d", count ( $filtered ), $n );
		}
		
		/*
		 * {
		 * // Shall not happen....
		 * printf("\n<br/>!! Parts %d", count($filtered));
		 * printf("\n...%f %f", $log[$k-1]['time'], $t+$dt);
		 * $filtered[] = $this->sumAvg((int)($t+$dt/2), $v_agg, $v_cnt);
		 * }
		 */
		return $filtered;
	}
	
	/**
	 *
	 * @param integer $t
	 * @param number $v_agg
	 * @param integer[] $v_cnt
	 * @return number[]
	 */
	private function sumAvg($t, $v_agg, $v_cnt) {
		$t = ( int ) $t;
		$avg = array (
				'time' => $t,
				'timestr' => date ( 'c', $t ) 
		);
		foreach ( $v_cnt as $pid => $c ) {
			$avg [$pid] = ( int ) ($v_agg [$pid] / $c);
		}
		return $avg;
	}
	
	/**
	 * Return log as an array with columns sorted as in getFields().
	 * Optionally, if $n>0, splot the log in $n parts equally distibuted over time.
	 *
	 * @param integer $n
	 * @return array
	 */
	function asArray($n = -1) {
		$log = ($n > 1) ? $this->filterLog ( $n ) : $this->getLog ();
		$log = $this->addNullFields ( $log, $this->getFields () );
		array_walk ( $log, function (&$v, $k) {
			$v = array_values ( $v );
		} );
		return $log;
	}
	
	/**
	 * Add missing fields with value null if they do not exists.
	 *
	 * @param array $log
	 * @param array $fields
	 * @return array
	 */
	function addNullFields($log, $fields) {
		$f = array_combine ( $fields, array_fill ( 1, count ( $fields ), null ) );
		array_walk ( $log, function (&$v, $k, $x) {
			$v = array_replace ( $x, $v );
		}, $f );
		return $log;
	}
	
	/**
	 * Uses $_REQUEST['period'] to return a DateTime object for when to include data from.
	 *
	 * @return DateTime
	 */
	public static function getHistoryLength() {
		$tz = new DateTimeZone ( 'Europe/Stockholm' );
		$from_time = new DateTime ( 'now', $tz );
		$interval = new DateInterval ( 'P2D' );
		
		if (isset ( $_REQUEST ['from'] )) {
			if ($ft = DateTime::createFromFormat ( "%y-%m%-%d %h:%m", $_REQUEST ['from'], $tz )) {
				return $ft;
			}
		}
		
		if (isset ( $_REQUEST ['period'] )) {
			if (strtolower ( $_REQUEST ['period'] ) == 'all') {
				$interval = new DateInterval ( 'P10Y' );
			} else {
				try {
					$interval = new DateInterval ( $_REQUEST ['period'] );
				} catch ( Exception $e ) {
					// Default remains
				}
			}
		}
		
		$from_time->sub ( $interval );
		return $from_time;
	}
	
	/**
	 * Uses $_REQUEST['sample'] to return max number of samples to include in data.
	 *
	 * @return number
	 */
	public static function getSampleCount() {
		if (isset ( $_REQUEST ['samples'] ) && is_numeric ( $_REQUEST ['samples'] )) {
			return ( int ) $_REQUEST ['samples'];
		}
		return 100;
	}
	
	/**
	 * Uses $_REQUEST['chip'] to select chip.
	 *
	 * @return number
	 */
	public static function getChip() {
		$chips = HuzzaWatering::listChips ();
		
		if (empty ( $chips )) {
			return '';
		}
		
		if (! isset ( $_REQUEST ['chip'] )) {
			// Use first chip item by default
			return $chips [0];
		}
		
		if (in_array ( $_REQUEST ['chip'], $chips )) {
			return $_REQUEST ['chip'];
		}
		
		return '';
	}
	
	/**
	 * Return log files in an string array.
	 *
	 * @return string[]
	 */
	public static function listChips() {
		$chips = [ ];
		foreach ( glob ( "logs/*.txt" ) as $filename ) {
			$chips [] = substr ( substr ( $filename, 5 ), 0, - 4 );
		}
		return $chips;
	}
}

