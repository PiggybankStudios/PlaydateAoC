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

#define CALCULATE_DAY_FUNC_DEFINITION(functionName) bool functionName(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
typedef CALCULATE_DAY_FUNC_DEFINITION(CalculateDayFunc_f);

struct DayInfo_t
{
	u16 year;
	u8 day;
	u64 stateSize;
	CalculateDayFunc_f* firstCalculateFunc;
	CalculateDayFunc_f* secondCalculateFunc;
	MyStr_t testInputPath;
	MyStr_t realInputPath;
	MyStr_t firstTestExpectedResult;
	MyStr_t secondTestExpectedResult;
};

struct GameGlobals_t
{
	bool initialized;
	
	SpriteSheet_t btnPromptsSheet;
	Font_t mainFont;
	
	VarArray_t dayInfos;
	
	DayInfo_t* selectedDayInfo;
};

#include "day_selector_state.h"
#include "calculator_state.h"

#endif //  _GAME_MAIN_H
