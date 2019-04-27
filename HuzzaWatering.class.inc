<?php
$_SERVER ['DOCUMENT_ROOT'] || ($_SERVER ['DOCUMENT_ROOT'] = str_replace ( getenv ( "SCRIPT_NAME" ), "", str_replace ( "\\", "/", getenv ( "SCRIPT_FILENAME" ) ) ));

require_once 'constants.inc';

/**
 * Some functions to get log file contents as php array.
 */
class HuzzaWatering {
	private $send_params;
	private $request_params;
	private $pump_no;
	private $dev_id; // Chip id
	private $settings = [ ]; // Chip settings
	private $pids = [ ]; // Parameter names
	private $log = [ ]; // Log
	                    // public $from_time = null; // DateTime object for first data
	                    // public $interval = null; // DateInterval object for time interval of data to inclued
	function __construct() {
		$this->from_time = new DateTime ( 'now', new DateTimeZone ( 'Europe/Stockholm' ) );
		$this->interval = new DateInterval ( 'P2D' );
		$this->pump_no = 1;
		$this->send_params = self::getParamsSelection ();
		$this->send_params = array_fill_keys ( array_keys ( $this->send_params ), FALSE );
	}
	
	/**
	 * Load send and request parameters from file.
	 * A valid device id must be set before.
	 */
	private function LoadParams() {
		$this->send_params = [ ];
		$this->request_params = [ ];
		
		if (! self::deviceExists ( $this->dev_id )) {
			return;
		}
		$filename = sprintf ( '%1$s/hw/dev/%2$s/%2$s.conf', $_SERVER ['DOCUMENT_ROOT'], $this->dev_id );
		if (! file_exists ( $filename )) {
			return;
		}
		$a = json_decode ( fgets ( $filename ) );
		$this->send_params = $a [0];
		$this->request_params = $a [1];
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
	public static function getParamsSelection() {
		$p = PRM\TR_PRM;
		unset ( $p [PRM\NONE] );
		unset ( $p [PRM\_END] );
		return $p;
	}
	function getSendParams() {
		return $this->send_params;
	}
	function setSendParams($params) {
		$this->send_params = $params;
	}
	function getRequestParams() {
		return $this->request_params;
	}
	function setRequestParams($params) {
		if (! $this->dev_id) {
			// No device selected
			$this->request_params = [ ];
		}
		$this->request_params = $params;
	}
	function save() {
		if ($this->dev_id) {
			$a = [ ];
			$a [0] = $this->send_params;
			$a [1] = $this->request_params;
			$filename = sprintf ( '%1$s/hw/dev/%2$s/%2$s.conf', $_SERVER ['DOCUMENT_ROOT'], $this->dev_id );
			file_put_contents ( $filename, json_encode ( $a ) );
			return FALSE;
		}
		return TRUE;
	}
	
	/**
	 * Load parameters for device with given id.
	 *
	 * @param string $dev_id
	 *        	Device id as 6 characters HEX.
	 * @return boolean True on failure.
	 */
	function loadDevice($dev_id) {
		$this->dev_id = 0;
		$dev_id = trim ( $dev_id );
		if (self::isValidDeviceId ( $dev_id )) {
			$this->dev_id = $dev_id;
		}
		$this->LoadParams ();
		return $this->dev_id ? FALSE : TRUE;
	}
	
	/**
	 * Validate a device id.
	 * Device id shall be a 6 characters of HEX-code.
	 *
	 * @param string $dev_id
	 * @return boolean
	 */
	static function isValidDeviceId($dev_id) {
		$dev_id = trim ( $dev_id );
		return (strlen ( $dev_id ) == 6 && ctype_xdigit ( $dev_id ));
	}
	
	/**
	 * Assign the chip id.
	 *
	 * @param string $chip
	 *        	HEX code chip id.
	 */
	function getDeviceId() {
		return $this->dev_id;
	}
	
	/**
	 * Load parameters for chip.
	 *
	 * @param string $filename
	 * @return bool True if there is no .conf file.
	 */
	public function readSettings() {
		$this->settings = [ ];
		if (! $this->dev_id) {
			return;
		}
		$filename = sprintf ( '%1$s/hw/dev/%2$s/%2$s.conf', $_SERVER ['DOCUMENT_ROOT'], $this->dev_id );
		if (file_exists ( $filename )) {
			$this->settings = json_decode ( fgets ( $filename ) );
		} else {
			
			self::getParamsSelection ();
		}
	}
	
	/**
	 * Save parameters for chip.
	 *
	 * @param array $config
	 *        	Configuration
	 * @return bool True on failure.
	 */
	public function writeSettings() {
		if ($this->dev_id) {
			$filename = sprintf ( '%1$s/hw/dev/%2$s/%2$s.conf', $_SERVER ['DOCUMENT_ROOT'], $this->dev_id );
			file_put_contents ( $filename, json_encode ( $this->settings ) );
			return FALSE;
		}
		return TRUE;
	}
	
	/**
	 * Select pump number $n.
	 */
	function selectPump(int $n) {
		$this->pump_no = $n;
	}
	
	/**
	 * Set pump capacity in cc/min for pump number $n.
	 *
	 * @param int $q
	 * @param int $n
	 */
	function setPumpCapacity($q, $n) {
		switch ($n) {
			case 1 :
				$this->settings [PRM\P1_FLOW] = $q;
				break;
			case 2 :
				$this->settings [PRM\P2_FLOW] = $q;
				break;
			case 3 :
				$this->settings [PRM\P3_FLOW] = $q;
				break;
		}
	}
	
	/**
	 * Get pump flow capacity for selected pump.
	 *
	 * @return int Capacity in cc/min.
	 */
	function getPumpCapacity() {
		switch ($this->pump_no) {
			case 1 :
				return $this->settings [PRM\P1_FLOW] = $q;
			case 2 :
				return $this->settings [PRM\P2_FLOW] = $q;
			case 3 :
				return $this->settings [PRM\P3_FLOW] = $q;
		}
	}
	
	/**
	 * Set requested flow for pump $n in cc/min.
	 *
	 * @param int $q
	 * @param int $n
	 */
	function setPumpSetpoint($q, $n) {
		switch ($n) {
			case 1 :
				$this->settings [PRM\P1_RQST_VOL] = $q;
				break;
			case 2 :
				$this->settings [PRM\P2_RQST_VOL] = $q;
				break;
			case 3 :
				$this->settings [PRM\P3_RQST_VOL] = $q;
				break;
		}
	}
	
	/**
	 * Get requested flow for selected pump.
	 *
	 * @return int Flow in cc/min.
	 */
	function getPumpSetpoint() {
		switch ($this->pump_no) {
			case 1 :
				return @$this->settings [PRM\P1_RQST_VOL] = $q;
			case 2 :
				return @$this->settings [PRM\P2_RQST_VOL] = $q;
			case 3 :
				return @$this->settings [PRM\P3_RQST_VOL] = $q;
		}
	}
	
	/**
	 * Set tank volume.
	 *
	 * @param int $volume
	 *        	Volume in cc.
	 */
	function setTankVolume(int $volume) {
		$this->settings [PRM\TANK_VOL] = $volume;
	}
	
	/**
	 * Get tank volume.
	 *
	 * @param int $volume
	 *        	Volume in cc.
	 */
	function getTankVolume() {
		return @$this->settings [PRM\TANK_VOL];
	}
	
	/**
	 * Set connection interval.
	 * Minimum value 1000 ms.
	 *
	 * @param int $t
	 *        	Interval in milliseconds.
	 */
	function setConnectionInterval(int $t) {
		$this->settings [PRM\REFRESH_RATE] = max ( 1000, $t );
	}
	
	/**
	 * Get connection interval.
	 * Minimum value 1000 ms.
	 *
	 * @return int Interval in milliseconds.
	 */
	function getConnectionInterval() {
		return @$this->settings [PRM\REFRESH_RATE];
	}
	
	/**
	 * Reset last active error to NOERR.
	 */
	function resetError() {
		$this->settings [PRM\LAST_ERR] = ERR\NOERR;
	}
	
	/**
	 * Reset pumped volume for pump 1, 2 and 3.
	 */
	function resetPumpedVolume() {
		$this->settings [PRM\P1_PUMPED_VOL] = 0;
		$this->settings [PRM\P2_PUMPED_VOL] = 0;
		$this->settings [PRM\P3_PUMPED_VOL] = 0;
	}
	
	/**
	 * Load log for given chip and returns it as an array.
	 * Only SET commands preserved.
	 *
	 * @param DateTime $from
	 *        	Optional. Include data for last 2 days by default.
	 */
	function LoadLog(DateTime $from = null) {
		$filename = $_SERVER ['DOCUMENT_ROOT'] . "/hw/dev/{$this->dev_id}/{$this->dev_id}.log";
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
				$vals ['id'] = ( int ) $row [3];
				$vals ['CMD'] = CMD\SET;
				
				$row = array_slice ( $row, 5 );
				
				$len = count ( $row );
				$pid = '';
				for($k = 0; $k < $len; ++ $k) {
					if ($k % 2) {
						$vals [$pid] = ( int ) $row [$k];
					} else {
						$pid = sprintf ( "s%d", $row [$k] );
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
	 * Add null to missing fields.
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
	 * Check if device exist on server.
	 *
	 * @param string $dev_id
	 *        	Device id as HEX.
	 * @return boolean True if it exists.
	 */
	public static function deviceExists($dev_id) {
		$dev_id = trim ( $dev_id );
		if (! self::isValidDeviceId ( $dev_id )) {
			return FALSE;
		}
		$devices = self::listDevices ();
		return in_array ( $dev_id, $devices );
	}
	
	/**
	 * Lists chip ids from available .conf files.
	 *
	 * @return string[]
	 */
	public static function listDevices() {
		$devices = [ ];
		$directories = glob ( $_SERVER ['DOCUMENT_ROOT'] . '/hw/dev/*', GLOB_ONLYDIR );
		foreach ( $directories as $d ) {
			$devices [] = substr ( $d, - 6 );
		}
		return $devices;
	}
}

