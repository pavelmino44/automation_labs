
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

/*
 * Лабораторная работа №2. Управление защитной дверью.
 *
 * Экземпляры ФБ объявлены в Logical/Global.var и замаплены в IoMap.iom:
 *   stateMachine (DriveStateMashine) - частотный преобразователь (ETAD/CMDD/LFRD)
 *   doorSM       (DoorStateMashine)  - логика двери, датчики S0..S3 и тумблер
 *   ledSM        (LedStateMashine)   - индикаторы
 * Поэтому здесь их НЕ объявляем повторно - они доступны через AsDefault.h.
 */

/* Информация о задаче (время цикла) - библиотека brsystem */
static RTInfo_typ rtInfo;

void _INIT ProgramInit(void)
{
	rtInfo.enable = 1;
	RTInfo(&rtInfo);
	ledSM.cycleTime = rtInfo.cycle_time / 1000;   /* мкс -> мс */
	if (ledSM.cycleTime == 0)
		ledSM.cycleTime = 100;

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
	/* 1. Логика двери: датчики и тумблер приходят через мэпинг
	      (doorSM.direction, doorSM.sw1..sw4), состояние ПЧ - из stateMachine */
	doorSM.state = stateMachine.state;
	DoorStateMashine(&doorSM);

	/* 2. Управление частотным преобразователем */
	stateMachine.enable   = 1;
	stateMachine.speedSet = doorSM.speed;
	DriveStateMashine(&stateMachine);   /* speed -> LFRD_Output, command -> CMDD_Output */

	/* 3. Индикация положения двери */
	ledSM.state = (UINT)doorSM.doorState;
	LedStateMashine(&ledSM);            /* led1..led4 -> DigitalOutput02..05 */
}
