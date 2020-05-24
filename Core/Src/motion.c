#include "motion.h"

PIT pit[PITITEM];

int absl(int num)
{
	if(num>=0)
		return num;
	else
		return -num;
}

void DampAutoPos(void)
{
	int i;
	float step;
	for(i=0;i<PITITEM;i++)
	{
		if(!pit[i].disable)
		{
			step = absl(pit[i].target-pit[i].current)/10+1;
			
			if(pit[i].target > pit[i].current+step)
			{
				pit[i].current += step;
				pit[i].match = False;
			}
			else if(pit[i].target < pit[i].current-step)
			{
				pit[i].current -= step;
				pit[i].match = False;
			}
			else if(absl(pit[i].target-pit[i].current)<=step)
			{
				pit[i].current = pit[i].target;
				pit[i].match = True;
			}
		}
	}
}

void DampCustormPos(u16 index,float step)
{
	if(pit[index].target > pit[index].current+step)
	{
		pit[index].current += step;
		pit[index].match = False;
	}
	else if(pit[index].target < pit[index].current-step)
	{
		pit[index].current -= step;
		pit[index].match = False;
	}
	else if(absl(pit[index].target-pit[index].current)<=step)
	{
		pit[index].current = pit[index].target;
		pit[index].match = True;
	}
}

void SetCurrent(u8 index,float value)
{
	pit[index].current = value;
}

void SetTarget(u8 index,float value)
{
	pit[index].target = value;
}

void SetDisable(u8 index,u8 able)
{
	pit[index].disable = able;
}
