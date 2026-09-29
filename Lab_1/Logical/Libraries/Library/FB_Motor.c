
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "Library.h"
#ifdef __cplusplus
	};
#endif
/* TODO: Add your comment here */
void FB_Motor(struct FB_Motor* inst)
{
	if (inst==0)
		return ;
	REAL u_norm = inst->u / inst->ke;
	REAL din = (u_norm - inst->integrator.out) / inst->Tm;
	
	inst->integrator.in = din;
	FB_Integrator(&inst->integrator);
	inst->w = inst->integrator.out;
	
	inst->phi += inst->w * inst->dt;
}
