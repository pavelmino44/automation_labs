
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
 * »ндикаци€ положени€ двери (рис. 2.11).
 * state - состо€ние двери (DoorStates), led1..led4 -> индикаторы S0..S3.
 * ¬ ST_UNKNOWN все индикаторы мигают с частотой 1 √ц (500 мс гор€т / 500 мс нет).
 * cycleTime - врем€ цикла задачи в мс (передаЄтс€ из программы, см. Main.c).
 * timer - текущее врем€ внутри полупериода мигани€, мс.
 */
static void SetLeds(struct LedStateMashine* inst, BOOL l1, BOOL l2, BOOL l3, BOOL l4)
{
	inst->led1 = l1;
	inst->led2 = l2;
	inst->led3 = l3;
	inst->led4 = l4;
}

void LedStateMashine(struct LedStateMashine* inst)
{
	UDINT dt = inst->cycleTime;
	if (dt == 0)
		dt = 100;   /* на случай, если врем€ цикла не задано */

	switch (inst->state)
	{
		case ST_UNKNOWN:
			inst->elapsed += dt;
			if (inst->elapsed >= BLINK_HALF_PERIOD_MS)
			{
				inst->elapsed = 0;
				inst->blink = !inst->blink;
			}
			SetLeds(inst, inst->blink, inst->blink, inst->blink, inst->blink);
			break;

		case ST_OPEN:     SetLeds(inst, 0, 0, 0, 0); break;
		case ST_ACC_POS:  SetLeds(inst, 1, 0, 0, 0); break;
		case ST_POS:      SetLeds(inst, 1, 1, 0, 0); break;
		case ST_DEC_POS:  SetLeds(inst, 1, 1, 1, 0); break;
		case ST_CLOSE:    SetLeds(inst, 1, 1, 1, 1); break;
		case ST_ACC_NEG:  SetLeds(inst, 1, 1, 1, 0); break;
		case ST_NEG:      SetLeds(inst, 1, 1, 0, 0); break;
		case ST_DEC_NEG:  SetLeds(inst, 1, 0, 0, 0); break;

		case ST_INIT:
		default:
			SetLeds(inst, 0, 0, 0, 0);
			break;
	}

	if (inst->state != ST_UNKNOWN)
	{
		inst->elapsed = 0;
		inst->blink = 0;
	}
	inst->timer = (INT)inst->elapsed;
}
