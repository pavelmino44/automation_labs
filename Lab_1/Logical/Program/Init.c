
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	fb_motor.Tm = 0.05;
	fb_motor.ke = 0.05;
	fb_motor.integrator.dt = 0.01;
	fb_motor.integrator.out = 0.0;
	fb_motor.phi = 0.0;
	
	fb_motor_2.Tm = 0.05;
	fb_motor_2.ke = 0.05;
	fb_motor_2.integrator.dt = 0.01;
	fb_motor_2.integrator.out = 0.0;
	fb_motor_2.phi = 0.0;
	
   Tzh = 0.02;
	
	fb_regulator.k_p = fb_motor.ke * fb_motor.Tm / Tzh;
	fb_regulator.k_i = fb_motor.ke / Tzh;
	fb_regulator.max_abs_value = 24.0;
	fb_regulator.max_abs_value = 24.0;
	fb_regulator.integrator.dt = 0.01;
	fb_regulator.integrator.out = 0.0;
	
	Counter = 0;
	Speed = 0;
	Enable = 0;
	
}
