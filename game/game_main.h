/*
File:   game_main.h
Author: Taylor Robbins
Date:   12\19\2023
*/

#ifndef _GAME_MAIN_H
#define _GAME_MAIN_H

#include "game_version.h"
#include "game_defines.h"

#include "day_selector_state.h"
#include "calculator_state.h"

#define CALCULATE_DAY_FUNC_DEFINITION(functionName) bool functionName(void* state, u64 numIter, MyStr_t* result)
typedef CALCULATE_DAY_FUNC_DEFINITION(CalculateDayFunc_f);

struct DayInfo_t
{
	u32 year;
	u32 day;
	u64 stateSize;
	CalculateDayFunc_f* calculateFunc;
};

struct GameGlobals_t
{
	bool initialized;
	
	SpriteSheet_t btnPromptsSheet;
};

#endif //  _GAME_MAIN_H
