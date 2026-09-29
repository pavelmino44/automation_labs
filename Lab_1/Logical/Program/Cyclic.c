
#include <bur/plctypes.h>
#include <Library.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _CYCLIC ProgramCyclic(void)
{
	Counter ++;
	if (Counter > 100)
	{
		Speed = 90.0;
		Enable = 1;
	}
	if (Counter > 250)
	{
		Speed = 0.0;
	}
	fb_motor_2.u = Enable ? (Speed * fb_motor_2.ke) : 0.0;
	FB_Motor(&fb_motor_2);
	
	if (Enable)
	{
		fb_regulator.e = Speed - fb_motor.w;
		FB_Regulator(&fb_regulator);
		
		fb_motor.u = fb_regulator.u;
		FB_Motor(&fb_motor);
	}
	else
	{
		fb_motor.u = 0.0;
		FB_Motor(&fb_motor);
	}
}
