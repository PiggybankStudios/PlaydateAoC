/*
File:   day_selector_state.h
Author: Taylor Robbins
Date:   01\18\2024
*/

#ifndef _DAY_SELECTOR_STATE_H
#define _DAY_SELECTOR_STATE_H

#define DSEL_BTN_SIZE                       NewVec2i(150, 35) //px
#define DSEL_BTN_PADDING                    5 //px
#define DSEL_BTN_OUTLINE_THICKNESS          4 //px
#define DSEL_BTN_OUTLINE_SELECTED_THICKNESS 10 //px
#define DSEL_CRANK_SENSITIVITY 10 //rot_degrees/btn

struct DayBtn_t
{
	DayInfo_t* dayInfo;
	
	reci mainRec;
};

struct DaySelectorState_t
{
	bool initialized;
	
	i64 selectionIndex;
	r32 prevCrankAngle; //degrees
	
	r32 scroll;
	r32 scrollGoto;
	r32 scrollMax;
	
	reci buttonsListRec;
	VarArray_t buttons; //DayBtn_t
};

#endif //  _DAY_SELECTOR_STATE_H
