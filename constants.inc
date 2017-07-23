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
	const RUN_INTERVAL = 0x0B;
	const REFRESH_RATE = 0x0C;
	const ADC1 = 0x0D;
	const ADC2 = 0x0E;
	const ADC3 = 0x0F;
	const ADC4 = 0xA0;
	const _END = 0xA1;
}
