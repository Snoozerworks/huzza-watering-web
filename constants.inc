<?php

// These constants much match those on the thing
namespace CMD {

	// Identifiers for commands which can be interpreted
	const NONE = 0x00;
	const GET = 0x01;
	const SET = 0x02;
	const _END = 0x03;
}

namespace PRM {

	// Indentifiers for parameters which can be set or get
	const NONE = 0x00;
	const P1_FLOW = 0x01;
	const P2_FLOW = 0x02;
	const P3_FLOW = 0x03;
	const P1_RQST_VOL = 0x04;
	const P2_RQST_VOL = 0x05;
	const P3_RQST_VOL = 0x06;
	const P1_PUMPED_VOL = 0x07;
	const P2_PUMPED_VOL = 0x08;
	const P3_PUMPED_VOL = 0x09;
	const TANK_VOL = 0x0A;
	const ONTIME = 0x0B;
	const REFRESH_RATE = 0x0C;
	const ADC1 = 0x0D;
	const ADC2 = 0x0E;
	const ADC3 = 0x0F;
	const ADC4 = 0x10;
	const LAST_ERR = 0x11;
	const _END = 0x12;
	
	// Translation to strings
	const TR_PRM = array (
			NONE => 'NONE',
			P1_FLOW => 'Pumpkapacitet 1',
			P2_FLOW => 'Pumpkapacitet 2',
			P3_FLOW => 'Pumpkapacitet 3',
			P1_RQST_VOL => 'Önskad volym 1',
			P2_RQST_VOL => 'Önskad volym 2',
			P3_RQST_VOL => 'Önskad volym 3',
			P1_PUMPED_VOL => 'Levererad volym 1',
			P2_PUMPED_VOL => 'Levererad volym 2',
			P3_PUMPED_VOL => 'Levererad volym 3',
			TANK_VOL => 'Tankvolym',
			ONTIME => 'Pumptid per runda',
			REFRESH_RATE => 'Uppdateringshastighet',
			ADC1 => 'Givare 1',
			ADC2 => 'Givare 2',
			ADC3 => 'Givare 3',
			ADC4 => 'Givare 4',
			LAST_ERR => 'Last error',
			_END => 'END' 
	);
}

namespace ERR {

	const NOERR = 0x00;
	const CONN_ERR = 0x01;
	const RESP_HTTP_ERR = 0x03;
	const RESP_TIMEOUT_ERR = 0x04;
	const RESP_MALFORMED_ERR = 0x05;
	const BAD_COMMAND_ERR = 0x06;
	const PARAMID_SET_ERR = 0x07;
	const PARAMVAL_SET_ERR = 0x08;
	const PARAMID_GET_ERR = 0x09;
	const NULLPTR_ERR = 0x0A;
	const BUFFER_OVERRUN = 0x0B;
	const _END = 0x0C;
}
