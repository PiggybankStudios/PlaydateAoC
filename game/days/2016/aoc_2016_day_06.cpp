
struct Aoc2016_Day06_CharCount_t
{
	u64 counts[26];
};

struct Aoc2016_Day06_State_t
{
	bool initialized;
	u64 messageLength;
	Aoc2016_Day06_CharCount_t* charCounts;
	LineParser_t lineParser;
};

char FindMostCommonChar(const Aoc2016_Day06_CharCount_t* charCounts)
{
	u64 resultCount = 0;
	char resultChar = '0';
	for (u64 cIndex = 0; cIndex < ArrayCount(charCounts->counts); cIndex++)
	{
		if (charCounts->counts[cIndex] > resultCount) { resultCount = charCounts->counts[cIndex]; resultChar = (char)('a' + cIndex); }
	}
	return resultChar;
}
char FindLeastCommonChar(const Aoc2016_Day06_CharCount_t* charCounts)
{
	u64 resultCount = UINT64_MAX;
	char resultChar = '0';
	for (u64 cIndex = 0; cIndex < ArrayCount(charCounts->counts); cIndex++)
	{
		if (charCounts->counts[cIndex] > 0 && charCounts->counts[cIndex] < resultCount) { resultCount = charCounts->counts[cIndex]; resultChar = (char)('a' + cIndex); }
	}
	return resultChar;
}

// +==============================+
// |   Calculate_Aoc2016_Day06A   |
// +==============================+
// bool Calculate_Aoc2016_Day06A(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day06A)
{
	MemArena_t* scratch = GetScratchArena(resultArena);
	Aoc2016_Day06_State_t* state = (Aoc2016_Day06_State_t*)statePntr;
	if (!state->initialized)
	{
		ClearPointer(state);
		state->lineParser = NewLineParser(inputFile);
		state->initialized = true;
	}
	
	progressInfo->amountExpected = 650; //hardcoded :/
	
	bool finished = false;
	MyStr_t line = MyStr_Empty;
	if (LineParserGetLine(&state->lineParser, &line))
	{
		if (state->messageLength == 0)
		{
			Assert(line.length > 0);
			state->messageLength = line.length;
			state->charCounts = AllocArray(mainHeap, Aoc2016_Day06_CharCount_t, state->messageLength);
			MyMemSet(state->charCounts, 0x00, sizeof(Aoc2016_Day06_CharCount_t) * state->messageLength);
		}
		
		Assert(line.length == state->messageLength);
		for (u64 cIndex = 0; cIndex < state->messageLength; cIndex++)
		{
			char c = line.chars[cIndex];
			u64 charIndex = (u64)(c - 'a');
			Aoc2016_Day06_CharCount_t* counts = &state->charCounts[cIndex];
			counts->counts[charIndex] += 1;
		}
		
		progressInfo->amountCompleted++;
	}
	else
	{
		progressInfo->amountExpected = progressInfo->amountCompleted;
		MyStr_t resultStr = NewStr(state->messageLength, AllocArray(resultArena, char, state->messageLength+1));
		for (u64 cIndex = 0; cIndex < state->messageLength; cIndex++)
		{
			resultStr.chars[cIndex] = FindMostCommonChar(&state->charCounts[cIndex]);
		}
		resultStr.chars[state->messageLength] = '\0';
		SetOptionalOutPntr(result, resultStr);
		finished = true;
	}
	
	FreeScratchArena(scratch);
	return finished;
}

// +==============================+
// |   Calculate_Aoc2016_Day06B   |
// +==============================+
// bool Calculate_Aoc2016_Day06B(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day06B)
{
	MemArena_t* scratch = GetScratchArena(resultArena);
	Aoc2016_Day06_State_t* state = (Aoc2016_Day06_State_t*)statePntr;
	if (!state->initialized)
	{
		ClearPointer(state);
		state->lineParser = NewLineParser(inputFile);
		state->initialized = true;
	}
	
	progressInfo->amountExpected = 650; //hardcoded :/
	
	bool finished = false;
	MyStr_t line = MyStr_Empty;
	if (LineParserGetLine(&state->lineParser, &line))
	{
		if (state->messageLength == 0)
		{
			Assert(line.length > 0);
			state->messageLength = line.length;
			state->charCounts = AllocArray(mainHeap, Aoc2016_Day06_CharCount_t, state->messageLength);
			MyMemSet(state->charCounts, 0x00, sizeof(Aoc2016_Day06_CharCount_t) * state->messageLength);
		}
		
		Assert(line.length == state->messageLength);
		for (u64 cIndex = 0; cIndex < state->messageLength; cIndex++)
		{
			char c = line.chars[cIndex];
			u64 charIndex = (u64)(c - 'a');
			Aoc2016_Day06_CharCount_t* counts = &state->charCounts[cIndex];
			counts->counts[charIndex] += 1;
		}
		
		progressInfo->amountCompleted++;
	}
	else
	{
		progressInfo->amountExpected = progressInfo->amountCompleted;
		MyStr_t resultStr = NewStr(state->messageLength, AllocArray(resultArena, char, state->messageLength+1));
		for (u64 cIndex = 0; cIndex < state->messageLength; cIndex++)
		{
			resultStr.chars[cIndex] = FindLeastCommonChar(&state->charCounts[cIndex]);
		}
		resultStr.chars[state->messageLength] = '\0';
		SetOptionalOutPntr(result, resultStr);
		finished = true;
	}
	
	FreeScratchArena(scratch);
	return finished;
}