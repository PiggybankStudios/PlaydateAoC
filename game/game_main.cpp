/*
File:   game_main.cpp
Author: Taylor Robbins
Date:   12\19\2023
Description: 
	** None 
*/

// +--------------------------------------------------------------+
// |                           Globals                            |
// +--------------------------------------------------------------+
GameGlobals_t* gl = nullptr;

// +--------------------------------------------------------------+
// |                         Source Files                         |
// +--------------------------------------------------------------+
#include "game_helpers.cpp"
#include "day_selector_state.cpp"
#include "calculator_state.cpp"

#include "day_registration.h"

// +--------------------------------------------------------------+
// |                        Main Functions                        |
// +--------------------------------------------------------------+
AppState_t InitGame()
{
	WriteLine_O("+==============================+");
	PrintLine_O("|     %s v%u.%u(%0u)     |", PROJECT_NAME, GAME_VERSION_MAJOR, GAME_VERSION_MINOR, GAME_VERSION_BUILD);
	WriteLine_O("+==============================+");
	
	RegisterAppState_DaySelector();
	RegisterAppState_Calculator();
	
	gl = AllocStruct(fixedHeap, GameGlobals_t);
	ClearPointer(gl);
	gl->initialized = true;
	
	gl->btnPromptsSheet = LoadSpriteSheet(NewStr("Resources/Sheets/btn_prompts"), 8);
	Assert(gl->btnPromptsSheet.isValid);
	
	gl->mainFont = LoadFont(NewStr("Resources/Fonts/border_basic"));
	Assert(gl->mainFont.isValid);
	
	pd->display->setRefreshRate((r32)REFRESH_RATE);
	
	u64 numRegisteredDays = 0;
	#define REGISTER_AOC_DAY(year, day, stateStructType, calculateFunction, inputPath1, inputPath2) numRegisteredDays++;
	#include "day_registration.h"
	
	CreateVarArray(&gl->dayInfos, fixedHeap, sizeof(DayInfo_t), numRegisteredDays);
	
	#define REGISTER_AOC_DAY(yearValue, dayValue, stateStructType, calculateFunction, inputPath1, inputPath2) do \
	{                                                                                                            \
		DayInfo_t* newDayInfo = VarArrayAdd(&gl->dayInfos, DayInfo_t);                                           \
		NotNull(newDayInfo);                                                                                     \
		ClearPointer(newDayInfo);                                                                                \
		newDayInfo->year = (yearValue);                                                                          \
		newDayInfo->day = (dayValue);                                                                            \
		newDayInfo->stateSize = sizeof(stateStructType);                                                         \
		newDayInfo->calculateFunc = (calculateFunction);                                                         \
	} while(0)
	#include "day_registration.h"
	
	return FIRST_APP_STATE;
}

void PreUpdateGame()
{
	
}
void PostUpdateGame()
{
	
}

void PreRenderGame()
{
	
}
void PostRenderGame()
{
	
}
