
{REDUND_ERROR} FUNCTION_BLOCK DriveStateMashine (*Frequency inverter control (CiA402)*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		state : UINT; (*Inverter status word (ETAD)*)
		enable : BOOL; (*Enable FB*)
		speedSet : INT; (*Speed setpoint from door logic*)
	END_VAR
	VAR_OUTPUT
		command : UINT; (*Inverter control word (CMDD)*)
		speed : INT; (*Speed to inverter (LFRD)*)
	END_VAR
END_FUNCTION_BLOCK

{REDUND_ERROR} FUNCTION_BLOCK DoorStateMashine (*Door state machine*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		state : UINT; (*Inverter status word*)
		sw1 : BOOL; (*S0 - door open*)
		sw2 : BOOL; (*S1*)
		sw3 : BOOL; (*S2*)
		sw4 : BOOL; (*S3 - door closed*)
		direction : BOOL; (*Switch: 1 - close, 0 - open*)
	END_VAR
	VAR_OUTPUT
		speed : INT; (*Speed setpoint*)
		doorState : DoorStates; (*Current door state*)
	END_VAR
	VAR
		s0Prev : BOOL; (*Sensor levels on previous cycle*)
		s1Prev : BOOL;
		s2Prev : BOOL;
		s3Prev : BOOL;
	END_VAR
END_FUNCTION_BLOCK

{REDUND_ERROR} FUNCTION_BLOCK LedStateMashine (*LED indication*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		state : UINT; (*Door state*)
		cycleTime : UDINT; (*Task cycle time, ms*)
	END_VAR
	VAR_OUTPUT
		led1 : BOOL; (*S0*)
		led2 : BOOL; (*S1*)
		led3 : BOOL; (*S2*)
		led4 : BOOL; (*S3*)
		timer : INT; (*Blink timer, ms*)
	END_VAR
	VAR
		elapsed : UDINT;
		blink : BOOL;
	END_VAR
END_FUNCTION_BLOCK
