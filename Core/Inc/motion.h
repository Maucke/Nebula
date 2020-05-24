#ifndef __MOTIOH_H__ 	                                                                                                                                                                                                                                                                                                                                           
#define __MOTIOH_H__ 

#include "sys.h"

#define PITITEM 48
typedef struct
{
	float target;
	float current;
	u8 match;
	u8 disable;
} PIT;


extern PIT pit[PITITEM];

void DampAutoPos(void);
void DampCustormPos(u16 index,float step);

void SetCurrent(u8 index,float value);
void SetTarget(u8 index,float value);
void SetDisable(u8 index,u8 able);

#define POSNRCT 0
#define POSNRCD 1
#define POSNTOP 4
#define POSNBAR 3
#define POSNBAT 6
#define POSNBRO 2

#define CRTVANE 8
#define CRTVADS 5
#define DAMPTYP 9

#define PNTHOUR 10
#define PNTMIN 11
#define PNTSEC 12

#define RDRadius 13
#define RDRadiusC 14
#define RDRadiusB 15
#define RDHourRadius 16
#define RDMinRadius 17
#define RDSecRadius 18
#define RDRuleDelaut 19

#define RDHourColor 20
#define RDMinColor 21
#define RDSecColor 22

#define RDRadiusA 23
#define PNTTIME 24
#define PNTTIMEY 25
#endif
