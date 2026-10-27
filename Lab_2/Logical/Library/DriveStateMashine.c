
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
#include "Library.h"
#ifdef __cplusplus
	};
#endif

/*
 * Управление частотным преобразователем (рис. 2.9, профиль CiA 402 / DRIVECOM).
 * state   - слово состояния ETAD, анализируется по маске 0x6F
 * command - слово управления CMDD
 * speed   - задание скорости LFRD (выдаётся только в состоянии "работа разрешена")
 */
void DriveStateMashine(struct DriveStateMashine* inst)
{
	UINT st = inst->state & DRV_STATE_MASK;

	/* Работа блока запрещена -> SHUTDOWN, скорость 0 */
	if (!inst->enable)
	{
		inst->command = CMD_SHUTDOWN;
		inst->speed   = 0;
		return;
	}

	/* Авария ПЧ -> сброс ошибки */
	if (inst->state & STATE_FAULT_BIT)
	{
		inst->command = CMD_FAULT_RESET;
		inst->speed   = 0;
		return;
	}

	if (st == STATE_ENABLED)
	{
		/* Работа разрешена - передаём задание скорости */
		inst->command = CMD_ENABLE;
		inst->speed   = inst->speedSet;
	}
	else if (st == STATE_READY)
	{
		/* SWITCHED ON -> разрешить работу */
		inst->command = CMD_ENABLE;
		inst->speed   = 0;
	}
	else if (st == STATE_SWITCHED_ON)
	{
		/* READY TO SWITCH ON -> включить */
		inst->command = CMD_SWITCH_ON;
		inst->speed   = 0;
	}
	else
	{
		/* DISABLED (switch on disabled) и прочие -> SHUTDOWN */
		inst->command = CMD_SHUTDOWN;
		inst->speed   = 0;
	}
}
