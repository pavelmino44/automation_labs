
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	stateMachine.enable   = 0;
	stateMachine.speedSet = 0;
	stateMachine.command  = CMD_SHUTDOWN;
	stateMachine.speed    = 0;

	doorSM.doorState = ST_INIT;
	doorSM.speed     = 0;

	ledSM.state = ST_INIT;
	ledSM.led1  = 0;
	ledSM.led2  = 0;
	ledSM.led3  = 0;
	ledSM.led4  = 0;
}


void _CYCLIC ProgramCyclic(void)
{
	/* 1. Логика двери: датчики и тумблер приходят через мэпинг*/
	doorSM.state = stateMachine.state;
	DoorStateMashine(&doorSM);

	/* 2. Управление частотным преобразователем */
	stateMachine.enable   = 1;
	stateMachine.speedSet = doorSM.speed;
	DriveStateMashine(&stateMachine);   

	/* 3. Индикация положения двери */
	ledSM.state = (UINT)doorSM.doorState;
	LedStateMashine(&ledSM);            
}


void _EXIT ProgramExit(void)
{

}

