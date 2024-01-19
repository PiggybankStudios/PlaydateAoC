/*
File:   aoc_2016_day_02.cpp
Author: Taylor Robbins
Date:   01\18\2024
*/

struct Aoc2016_Day02_State_t
{
	bool initialized;
	bool completed;
	u64 cIndex;
	v2i position;
	bool movedSinceLastCode;
	VarArray_t codeChars; //char
};

// +==============================+
// |   Calculate_Aoc2016_Day02    |
// +==============================+
// bool Calculate_Aoc2016_Day02(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day02)
{
	NotNull(statePntr);
	Aoc2016_Day02_State_t* state = (Aoc2016_Day02_State_t*)statePntr;
	
	if (!state->initialized)
	{
		state->cIndex = 0;
		state->movedSinceLastCode = false;
		PrintLine_I("Input is %llu chars", inputFile.length);
		
		state->position = NewVec2i(1, 1);
		CreateVarArray(&state->codeChars, mainHeap, sizeof(char));
		
		state->initialized = true;
	}
	
	if (!state->completed)
	{
		char nextChar = (state->cIndex < inputFile.length ? inputFile.chars[state->cIndex] : '\0');
		state->cIndex++;
		
		progressInfo->amountCompleted = state->cIndex;
		progressInfo->amountExpected = inputFile.length+1;
		
		if (nextChar == '\n' || nextChar == '\0')
		{
			if (state->movedSinceLastCode)
			{
				char* newCodeChar = VarArrayAdd(&state->codeChars, char);
				NotNull(newCodeChar);
				*newCodeChar = '1' + (state->position.x + 3*state->position.y);
				PrintLine_D("Added %c to the code", *newCodeChar);
			}
			state->movedSinceLastCode = false;
		}
		else if (nextChar == 'U' || nextChar == 'u')
		{
			if (state->position.y > 0) { state->position.y--; }
			state->movedSinceLastCode = true;
			PrintLine_D("Stepped up to (%d, %d)", state->position.x, state->position.y);
		}
		else if (nextChar == 'D' || nextChar == 'd')
		{
			if (state->position.y < 2) { state->position.y++; }
			state->movedSinceLastCode = true;
			PrintLine_D("Stepped down to (%d, %d)", state->position.x, state->position.y);
		}
		else if (nextChar == 'L' || nextChar == 'l')
		{
			if (state->position.x > 0) { state->position.x--; }
			state->movedSinceLastCode = true;
			PrintLine_D("Stepped left to (%d, %d)", state->position.x, state->position.y);
		}
		else if (nextChar == 'R' || nextChar == 'r')
		{
			if (state->position.x < 2) { state->position.x++; }
			state->movedSinceLastCode = true;
			PrintLine_D("Stepped right to (%d, %d)", state->position.x, state->position.y);
		}
		
		PrintLine_D("Processed %llu/%llu chars", state->cIndex, inputFile.length);
		if (state->cIndex > inputFile.length)
		{
			WriteLine_I("The end!");
			state->completed = true;
		}
		
		if (state->completed)
		{
			MyStr_t resultStr = NewStr(state->codeChars.length, (char*)state->codeChars.items);
			SetOptionalOutPntr(result, AllocString(resultArena, &resultStr));
			FreeVarArray(&state->codeChars);
		}
	}
	
	return state->completed;
}

// +==============================+
// |   Calculate_Aoc2016_Day02x   |
// +==============================+
// bool Calculate_Aoc2016_Day02x(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day02x)
{
	NotNull(statePntr);
	Aoc2016_Day02_State_t* state = (Aoc2016_Day02_State_t*)statePntr;
	
	if (!state->initialized)
	{
		state->cIndex = 0;
		state->movedSinceLastCode = false;
		PrintLine_I("Input is %llu chars", inputFile.length);
		
		state->position = NewVec2i(0, 2);
		CreateVarArray(&state->codeChars, mainHeap, sizeof(char));
		
		state->initialized = true;
	}
	
	if (!state->completed)
	{
		char nextChar = (state->cIndex < inputFile.length ? inputFile.chars[state->cIndex] : '\0');
		state->cIndex++;
		
		progressInfo->amountCompleted = state->cIndex;
		progressInfo->amountExpected = inputFile.length+1;
		
		if (nextChar == '\n' || nextChar == '\0')
		{
			if (state->movedSinceLastCode)
			{
				char keyCharTable[] =
				{
					'X', 'X', '1', 'X', 'X',
					'X', '2', '3', '4', 'X',
					'5', '6', '7', '8', '9',
					'X', 'A', 'B', 'C', 'X',
					'X', 'X', 'D', 'X', 'X',
				};
				char* newCodeChar = VarArrayAdd(&state->codeChars, char);
				NotNull(newCodeChar);
				*newCodeChar = keyCharTable[(state->position.x + 5*state->position.y)];
				PrintLine_D("Added %c to the code", *newCodeChar);
			}
			state->movedSinceLastCode = false;
		}
		else if (nextChar == 'U' || nextChar == 'u')
		{
			if (state->position.y-1 >= 2 - state->position.x && state->position.y-1 >= -2 + state->position.x)
			{
				state->position.y--;
				PrintLine_D("Stepped up to (%d, %d)", state->position.x, state->position.y);
			}
			state->movedSinceLastCode = true;
		}
		else if (nextChar == 'D' || nextChar == 'd')
		{
			if (state->position.y+1 <= 2 + state->position.x && state->position.y+1 <= 6 - state->position.x)
			{
				state->position.y++;
				PrintLine_D("Stepped down to (%d, %d)", state->position.x, state->position.y);
			}
			state->movedSinceLastCode = true;
		}
		else if (nextChar == 'L' || nextChar == 'l')
		{
			if (state->position.x-1 >= 2 - state->position.y && state->position.x-1 >= -2 + state->position.y)
			{
				state->position.x--;
				PrintLine_D("Stepped left sto (%d, %d)", state->position.x, state->position.y);
			}
			state->movedSinceLastCode = true;
		}
		else if (nextChar == 'R' || nextChar == 'r')
		{
			if (state->position.x+1 <= 2 + state->position.y && state->position.x+1 <= 6 - state->position.y)
			{
				state->position.x++;
				PrintLine_D("Stepped right to (%d, %d)", state->position.x, state->position.y);
			}
			state->movedSinceLastCode = true;
		}
		
		PrintLine_D("Processed %llu/%llu chars", state->cIndex, inputFile.length);
		if (state->cIndex > inputFile.length)
		{
			WriteLine_I("The end!");
			state->completed = true;
		}
		
		if (state->completed)
		{
			MyStr_t resultStr = NewStr(state->codeChars.length, (char*)state->codeChars.items);
			SetOptionalOutPntr(result, AllocString(resultArena, &resultStr));
			FreeVarArray(&state->codeChars);
		}
	}
	
	return state->completed;
}
