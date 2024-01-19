/*
File:   day_selector_state.h
Author: Taylor Robbins
Date:   01\18\2024
*/

#ifndef _DAY_SELECTOR_STATE_H
#define _DAY_SELECTOR_STATE_H

struct DayBtn_t
{
	// DayInfo_t* dayInfo;
	
	reci mainRec;
};

struct DaySelectorState_t
{
	bool initialized;
	
	i64 selectionIndex;
	
	r32 scroll;
	r32 scrollGoto;
	
	reci buttonsListRec;
	VarArray_t buttons; //DayBtn_t
};

#endif //  _DAY_SELECTOR_STATE_H
