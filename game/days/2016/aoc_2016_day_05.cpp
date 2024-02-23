/*
File:   aoc_2016_day_05.cpp
Author: Taylor Robbins
Date:   02\22\2024
*/

#include "aoc_2016_day_05_md5.cpp"

struct Aoc2016_Day05_State_t
{
	bool initialized;
	bool completed;
	MD5Context md5Context;
	u64 searchIndex;
	u64 numCharsFound;
	char password[8];
};

// +==============================+
// |   Calculate_Aoc2016_Day05A   |
// +==============================+
// bool Calculate_Aoc2016_Day05A(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day05A)
{
	NotNull(statePntr);
	MemArena_t* scratch = GetScratchArena(resultArena);
	Aoc2016_Day05_State_t* state = (Aoc2016_Day05_State_t*)statePntr;
	
	if (!state->initialized)
	{
		ClearArray(state->password);
		state->searchIndex = 0;
		state->numCharsFound = 0;
		state->initialized = true;
	}
	
	if (!state->completed)
	{
		if (state->numCharsFound < ArrayCount(state->password))
		{
			MyStr_t inputStr = PrintInArenaStr(scratch, "%.*s%llu", StrPrint(inputFile), state->searchIndex);
			md5Init(&state->md5Context);
			md5Update(&state->md5Context, inputStr.bytes, inputStr.length);
			md5Finalize(&state->md5Context);
			if (state->md5Context.digest[0] == 0x00 && state->md5Context.digest[1] == 0x00 && (state->md5Context.digest[2] & 0xF0) == 0x00)
			{
				state->password[state->numCharsFound] = GetHexChar(state->md5Context.digest[2] & 0x0F);
				state->numCharsFound++;
			}
			state->searchIndex++;
		}
		
		if (state->numCharsFound >= ArrayCount(state->password))
		{
			MyStr_t resultStr = NewStr(ArrayCount(state->password), &state->password[0]);
			resultStr = AllocString(resultArena, &resultStr);
			SetOptionalOutPntr(result, resultStr);
			state->completed = true;
		}
		
		// progressInfo->amountCompleted = state->searchIndex;
		// progressInfo->amountExpected = (state->completed ? state->searchIndex : state->searchIndex + 1);
		progressInfo->amountCompleted = state->numCharsFound;
		progressInfo->amountExpected = ArrayCount(state->password);
	}
	
	FreeScratchArena(scratch);
	return state->completed;
}

// +==============================+
// |   Calculate_Aoc2016_Day05B   |
// +==============================+
// bool Calculate_Aoc2016_Day05B(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day05B)
{
	NotNull(statePntr);
	MemArena_t* scratch = GetScratchArena(resultArena);
	Aoc2016_Day05_State_t* state = (Aoc2016_Day05_State_t*)statePntr;
	
	if (!state->initialized)
	{
		ClearArray(state->password);
		state->searchIndex = 0;
		state->numCharsFound = 0;
		state->initialized = true;
	}
	
	if (!state->completed)
	{
		if (state->numCharsFound < ArrayCount(state->password))
		{
			MyStr_t inputStr = PrintInArenaStr(scratch, "%.*s%llu", StrPrint(inputFile), state->searchIndex);
			md5Init(&state->md5Context);
			md5Update(&state->md5Context, inputStr.bytes, inputStr.length);
			md5Finalize(&state->md5Context);
			if (state->md5Context.digest[0] == 0x00 && state->md5Context.digest[1] == 0x00 && (state->md5Context.digest[2] & 0xF0) == 0x00)
			{
				u8 placeIndex = (u8)(state->md5Context.digest[2] & 0x0F);
				if (placeIndex < ArrayCount(state->password))
				{
					char newPasswordChar = GetHexChar((state->md5Context.digest[3] & 0xF0) >> 4);
					if (state->password[placeIndex] == '\0')
					{
						PrintLine_I("Found %llu %c", placeIndex, newPasswordChar);
						state->password[placeIndex] = newPasswordChar;
						state->numCharsFound++;
					}
					else
					{
						PrintLine_D("Ignored %llu %c", placeIndex, newPasswordChar);
					}
				}
			}
			state->searchIndex++;
		}
		
		if (state->numCharsFound >= ArrayCount(state->password))
		{
			MyStr_t resultStr = NewStr(ArrayCount(state->password), &state->password[0]);
			resultStr = AllocString(resultArena, &resultStr);
			SetOptionalOutPntr(result, resultStr);
			state->completed = true;
		}
		
		// progressInfo->amountCompleted = state->searchIndex;
		// progressInfo->amountExpected = (state->completed ? state->searchIndex : state->searchIndex + 1);
		progressInfo->amountCompleted = state->numCharsFound;
		progressInfo->amountExpected = ArrayCount(state->password);
	}
	
	FreeScratchArena(scratch);
	return state->completed;
}
