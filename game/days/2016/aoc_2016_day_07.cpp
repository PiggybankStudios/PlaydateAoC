/*
File:   aoc_2016_day_07.cpp
Author: Taylor Robbins
Date:   02\29\2024
Description: 
	** None 
*/

struct Aoc2016_Day07_State_t
{
	bool initialized;
	bool completed;
	LineParser_t parser;
	u64 numTLSSupportingIPs;
	u64 numSSLSupportingIPs;
};

bool IsSquareBracket(char c)
{
	return (c == '[' || c == ']');
}

// +==============================+
// |   Calculate_Aoc2016_Day07A   |
// +==============================+
// bool Calculate_Aoc2016_Day07A(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day07A)
{
	NotNull(statePntr);
	MemArena_t* scratch = GetScratchArena(resultArena);
	Aoc2016_Day07_State_t* state = (Aoc2016_Day07_State_t*)statePntr;
	
	if (!state->initialized)
	{
		state->parser = NewLineParser(inputFile);
		state->numTLSSupportingIPs = 0;
		state->initialized = true;
	}
	
	if (!state->completed)
	{
		MyStr_t line;
		if (LineParserGetLine(&state->parser, &line))
		{
			bool isTlsSupported = false;
			u64 squareBracketLevel = 0;
			for (u64 cIndex = 0; cIndex < line.length; cIndex++)
			{
				if (line.chars[cIndex] == '[') { IncrementU64(squareBracketLevel); }
				else if (line.chars[cIndex] == ']') { Decrement(squareBracketLevel); }
				else if (cIndex >= 3)
				{
					char c1 = line.chars[cIndex-3];
					char c2 = line.chars[cIndex-2];
					char c3 = line.chars[cIndex-1];
					char c4 = line.chars[cIndex-0];
					if (c1 == c4 && c2 == c3 && c1 != c2 && !IsSquareBracket(c1) && !IsSquareBracket(c2))
					{
						if (squareBracketLevel == 0)
						{
							if (!isTlsSupported)
							{
								PrintLine_D("Line %llu: %.*s supports TLS because of \"%c%c%c%c\" at index %llu", state->parser.lineIndex, StrPrint(line), c1, c2, c3, c4, cIndex-3);
								isTlsSupported = true;
							}
						}
						else
						{
							PrintLine_D("Line %llu: %.*s does NOT support TLS because of match inside square brackets: \"%c%c%c%c\" at index %llu", state->parser.lineIndex, StrPrint(line), c1, c2, c3, c4, cIndex-3);
							isTlsSupported = false;
							break;
						}
					}
				}
			}
			if (squareBracketLevel != 0)
			{
				PrintLine_D("Line %llu: %.*s has mismatching square brackets", state->parser.lineIndex, StrPrint(line));
			}
			if (isTlsSupported)
			{
				state->numTLSSupportingIPs++;
			}
		}
		else
		{
			state->completed = true;
			*result = PrintInArenaStr(resultArena, "%llu", state->numTLSSupportingIPs);
		}
	}
	
	progressInfo->amountCompleted = state->parser.lineIndex;
	progressInfo->amountExpected = (state->completed ? progressInfo->amountCompleted : 2000); //Hardcoded again, we should solve this problem somehow
	
	FreeScratchArena(scratch);
	return state->completed;
}

// +==============================+
// |   Calculate_Aoc2016_Day07A   |
// +==============================+
// bool Calculate_Aoc2016_Day07A(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day07B)
{
	NotNull(statePntr);
	MemArena_t* scratch = GetScratchArena(resultArena);
	Aoc2016_Day07_State_t* state = (Aoc2016_Day07_State_t*)statePntr;
	
	if (!state->initialized)
	{
		state->parser = NewLineParser(inputFile);
		state->numTLSSupportingIPs = 0;
		state->initialized = true;
	}
	
	if (!state->completed)
	{
		MyStr_t line;
		if (LineParserGetLine(&state->parser, &line))
		{
			bool isSslSupported = false;
			u64 squareBracketLevel1 = 0;
			for (u64 cIndex1 = 0; cIndex1 < line.length; cIndex1++)
			{
				if (line.chars[cIndex1] == '[') { IncrementU64(squareBracketLevel1); }
				else if (line.chars[cIndex1] == ']') { Decrement(squareBracketLevel1); }
				else if (cIndex1 >= 2)
				{
					char c1 = line.chars[cIndex1-2];
					char c2 = line.chars[cIndex1-1];
					char c3 = line.chars[cIndex1-0];
					if (c1 == c3 && c2 != c1 && !IsSquareBracket(c1) && !IsSquareBracket(c2))
					{
						PrintLine_D("Line %llu: %.*s Searching for paired %c%c%c for %c%c%c at index %llu",
							state->parser.lineIndex,
							StrPrint(line),
							c2, c1, c2,
							c1, c2, c1,
							cIndex1-2
						);
						u64 squareBracketLevel2 = squareBracketLevel1;
						for (u64 cIndex2 = cIndex1+1; cIndex2 < line.length; cIndex2++)
						{
							if (line.chars[cIndex2] == '[') { IncrementU64(squareBracketLevel2); }
							else if (line.chars[cIndex2] == ']') { Decrement(squareBracketLevel2); }
							else if (cIndex2 >= cIndex1+3)
							{
								char c4 = line.chars[cIndex2-2];
								char c5 = line.chars[cIndex2-1];
								char c6 = line.chars[cIndex2-0];
								if (c4 == c6 && c5 == c1 && c4 == c2 && !IsSquareBracket(c4) && !IsSquareBracket(c5))
								{
									if ((squareBracketLevel1 > 0) != (squareBracketLevel2 > 0))
									{
										PrintLine_I("Found %c%c%c at index %llu!", c4, c5, c6, cIndex2-2);
										isSslSupported = true;
										break;
									}
									else { PrintLine_D("Paired %c%c%c at index %llu is %s square brackets. Ignoring.", c4, c5, c6, cIndex2-2, (squareBracketLevel2 > 0) ? "inside" : "NOT inside"); }
								}
							}
						}
						if (isSslSupported) { break; }
						else { PrintLine_D("Couldn't find paired %c%c%c", c2, c1, c2); }
					}
				}
			}
			if (squareBracketLevel1 != 0 && !isSslSupported)
			{
				PrintLine_D("Line %llu: %.*s has mismatching square brackets", state->parser.lineIndex, StrPrint(line));
			}
			if (isSslSupported)
			{
				state->numSSLSupportingIPs++;
			}
		}
		else
		{
			state->completed = true;
			*result = PrintInArenaStr(resultArena, "%llu", state->numSSLSupportingIPs);
		}
	}
	
	progressInfo->amountCompleted = state->parser.lineIndex;
	progressInfo->amountExpected = (state->completed ? progressInfo->amountCompleted : 2000); //Hardcoded again, we should solve this problem somehow
	
	FreeScratchArena(scratch);
	return state->completed;
}
