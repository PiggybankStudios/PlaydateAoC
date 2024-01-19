/*
File:   app_state_list.h
Author: Taylor Robbins
Date:   12\19\2023
*/

#ifndef _APP_STATE_LIST_H
#define _APP_STATE_LIST_H

enum AppState_t
{
	AppState_None = 0,
	AppState_DaySelector,
	AppState_Calculator,
	AppState_NumStates,
};
const char* GetAppStateStr(AppState_t enumValue)
{
	switch (enumValue)
	{
		case AppState_None:        return "None";
		case AppState_DaySelector: return "DaySelector";
		case AppState_Calculator:  return "Calculator";
		default: return "Unknown";
	}
}

#endif //  _APP_STATE_LIST_H
