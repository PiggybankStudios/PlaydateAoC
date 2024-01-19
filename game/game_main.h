/*
File:   game_main.h
Author: Taylor Robbins
Date:   12\19\2023
*/

#ifndef _GAME_MAIN_H
#define _GAME_MAIN_H

#include "game_version.h"
#include "game_defines.h"

struct ProgressInfo_t
{
	u64 amountCompleted;
	u64 amountExpected;
};

#define CALCULATE_DAY_FUNC_DEFINITION(functionName) bool functionName(void* statePntr, u64 numIter, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
typedef CALCULATE_DAY_FUNC_DEFINITION(CalculateDayFunc_f);

struct DayInfo_t
{
	u16 year;
	u8 day;
	u64 stateSize;
	CalculateDayFunc_f* calculateFunc;
};

struct GameGlobals_t
{
	bool initialized;
	
	SpriteSheet_t btnPromptsSheet;
	VarArray_t dayInfos;
	
	DayInfo_t* selectedDayInfo;
};

#include "day_selector_state.h"
#include "calculator_state.h"

#endif //  _GAME_MAIN_H
