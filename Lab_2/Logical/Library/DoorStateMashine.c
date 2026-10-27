
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
 * Конечный автомат защитной двери (рис. 2.6, 2.11).
 *
 * Датчики:  sw1 = S0 (дверь открыта), sw2 = S1, sw3 = S2, sw4 = S3 (дверь закрыта)
 * Тумблер:  direction = 1 - закрыть (движение S0 -> S3, скорость > 0, состояния *_POS)
 *           direction = 0 - открыть (движение S3 -> S0, скорость < 0, состояния *_NEG)
 * Скорость: S0-S1 и S2-S3 - SPEED_SLOW, S1-S2 - SPEED_FAST.
 *
 * Переходы по датчикам выполняются по ИЗМЕНЕНИЮ логического уровня
 * (0 -> 1 или 1 -> 0), а не по самому значению. Для этого блок хранит
 * уровни датчиков с прошлого цикла (s0Prev..s3Prev).
 * Это относится ко всем датчикам, включая концевые S0 и S3.
 * Тумблер direction работает по уровню - он задаёт, куда ехать.
 *
 * Переходы только последовательные (рис. 2.11): каждый датчик проверяется
 * лишь в том состоянии, которое заканчивается на нём. Перескочить через
 * состояние нельзя. Реверс тумблером - только на тот же отрезок хода
 * (ACC_POS <-> DEC_NEG, POS <-> NEG, DEC_POS <-> ACC_NEG).
 */
void DoorStateMashine(struct DoorStateMashine* inst)
{
	/* Текущие уровни датчиков */
	BOOL s0 = inst->sw1;
	BOOL s1 = inst->sw2;
	BOOL s2 = inst->sw3;
	BOOL s3 = inst->sw4;

	/* Изменение уровня относительно прошлого цикла */
	BOOL s0Changed = (s0 != inst->s0Prev);
	BOOL s1Changed = (s1 != inst->s1Prev);
	BOOL s2Changed = (s2 != inst->s2Prev);
	BOOL s3Changed = (s3 != inst->s3Prev);

	BOOL driveOn = ((inst->state & DRV_STATE_MASK) == STATE_ENABLED);
	DoorStates stateAtStart;

	/* Пропал ПЧ (авария/выключение) - всё заново, через поиск положения */
	if (!driveOn && inst->doorState != ST_INIT)
		inst->doorState = ST_INIT;

	stateAtStart = inst->doorState;

	switch (inst->doorState)
	{
		case ST_INIT:          /* ожидание включения ПЧ */
			inst->speed = SPEED_STOP;
			if (driveOn)
				inst->doorState = ST_UNKNOWN;
			break;

		case ST_UNKNOWN:       /* поиск положения: медленно в сторону тумблера */
			inst->speed = inst->direction ? SPEED_SLOW : -SPEED_SLOW;
			if (!inst->direction && s0Changed)
				inst->doorState = ST_OPEN;
			else if (inst->direction && s3Changed)
				inst->doorState = ST_CLOSE;
			break;

		case ST_OPEN:          /* стоим в S0 */
			inst->speed = SPEED_STOP;
			if (inst->direction)
				inst->doorState = ST_ACC_POS;
			break;

		case ST_ACC_POS:       /* S0 -> S1, медленно */
			inst->speed = SPEED_SLOW;
			if (!inst->direction)
				inst->doorState = ST_DEC_NEG;      /* реверс: назад к S0 */
			else if (s1Changed)
				inst->doorState = ST_POS;
			break;

		case ST_POS:           /* S1 -> S2, быстро */
			inst->speed = SPEED_FAST;
			if (!inst->direction)
				inst->doorState = ST_NEG;
			else if (s2Changed)
				inst->doorState = ST_DEC_POS;
			break;

		case ST_DEC_POS:       /* S2 -> S3, медленно */
			inst->speed = SPEED_SLOW;
			if (!inst->direction)
				inst->doorState = ST_ACC_NEG;
			else if (s3Changed)
				inst->doorState = ST_CLOSE;
			break;

		case ST_CLOSE:         /* стоим в S3 */
			inst->speed = SPEED_STOP;
			if (!inst->direction)
				inst->doorState = ST_ACC_NEG;
			break;

		case ST_ACC_NEG:       /* S3 -> S2, медленно */
			inst->speed = -SPEED_SLOW;
			if (inst->direction)
				inst->doorState = ST_DEC_POS;      /* реверс: назад к S3 */
			else if (s2Changed)
				inst->doorState = ST_NEG;
			break;

		case ST_NEG:           /* S2 -> S1, быстро */
			inst->speed = -SPEED_FAST;
			if (inst->direction)
				inst->doorState = ST_POS;
			else if (s1Changed)
				inst->doorState = ST_DEC_NEG;
			break;

		case ST_DEC_NEG:       /* S1 -> S0, медленно */
			inst->speed = -SPEED_SLOW;
			if (inst->direction)
				inst->doorState = ST_ACC_POS;
			else if (s0Changed)
				inst->doorState = ST_OPEN;
			break;

		default:
			inst->speed = SPEED_STOP;
			inst->doorState = ST_INIT;
			break;
	}

	/* Запоминаем уровни для следующего цикла.
	   Пока ждём ПЧ (ST_INIT), держим "прошлые" уровни равными 0:
	   если дверь уже стоит на датчике, при входе в ST_UNKNOWN
	   это будет воспринято как изменение уровня 0 -> 1. */
	if (stateAtStart == ST_INIT || inst->doorState == ST_INIT)
	{
		inst->s0Prev = 0;
		inst->s1Prev = 0;
		inst->s2Prev = 0;
		inst->s3Prev = 0;
	}
	else
	{
		inst->s0Prev = s0;
		inst->s1Prev = s1;
		inst->s2Prev = s2;
		inst->s3Prev = s3;
	}
}
