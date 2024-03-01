/*
File:   aoc_2016_day_08.cpp
Author: Taylor Robbins
Date:   02\29\2024
*/

struct Aoc2016_Day08_State_t
{
	bool initialized;
	bool completed;
	LineParser_t parser;
	v2i screenSize;
	u64 numPixels;
	u64 numTempPixels;
	bool* pixels;
	bool* tempPixels;
};

// +==============================+
// |   Calculate_Aoc2016_Day08A   |
// +==============================+
// bool Calculate_Aoc2016_Day08A(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day08A)
{
	NotNull(statePntr);
	MemArena_t* scratch = GetScratchArena(resultArena);
	Aoc2016_Day08_State_t* state = (Aoc2016_Day08_State_t*)statePntr;
	
	if (!state->initialized)
	{
		state->parser = NewLineParser(inputFile);
		MyStr_t firstLine;
		bool hasAnyLines = LineParserGetLine(&state->parser, &firstLine);
		Assert(hasAnyLines);
		StrReplaceInPlace(firstLine, "x", ",");
		bool parsedScreenSize = TryParseV2i(firstLine, &state->screenSize);
		Assert(parsedScreenSize);
		
		Assert(state->screenSize.width > 0 && state->screenSize.height > 0);
		state->numPixels = (u64)(state->screenSize.width * state->screenSize.height);
		state->numTempPixels = (u64)MaxI32(state->screenSize.width, state->screenSize.height);
		state->pixels = AllocArray(mainHeap, bool, state->numPixels);
		state->tempPixels = AllocArray(mainHeap, bool, state->numTempPixels);
		NotNull(state->pixels);
		NotNull(state->tempPixels);
		MyMemSet(state->pixels, 0x00, sizeof(bool) * state->numPixels);
		MyMemSet(state->tempPixels, 0x00, sizeof(bool) * state->numTempPixels);
		
		state->initialized = true;
	}
	
	if (!state->completed)
	{
		MyStr_t line;
		if (LineParserGetLine(&state->parser, &line))
		{
			u64 numPieces = 0;
			MyStr_t* pieces = SplitString(scratch, line, " ", &numPieces);
			Assert(numPieces > 0);
			MyStr_t cmd = pieces[0];
			
			// +==============================+
			// |         rect command         |
			// +==============================+
			if (StrEqualsIgnoreCase(cmd, "rect"))
			{
				Assert(numPieces == 2);
				MyStr_t rectSizeStr = pieces[1];
				StrReplaceInPlace(rectSizeStr, "x", ",");
				v2i rectSize = Vec2i_Zero;
				bool parsedRectSize = TryParseV2i(rectSizeStr, &rectSize);
				Assert(parsedRectSize);
				
				for (i32 yOffset = 0; yOffset < rectSize.height && yOffset < state->screenSize.height; yOffset++)
				{
					for (i32 xOffset = 0; xOffset < rectSize.width && xOffset < state->screenSize.width; xOffset++)
					{
						state->pixels[(yOffset * state->screenSize.width) + xOffset] = true;
					}
				}
			}
			// +==============================+
			// |        rotate command        |
			// +==============================+
			else if (StrEqualsIgnoreCase(cmd, "rotate"))
			{
				Assert(numPieces == 5);
				MyStr_t rotateTypeStr = pieces[1];
				MyStr_t coordinateStr = pieces[2];
				//pieces[3] is just "by" which holds no info
				MyStr_t amountStr = pieces[4];
				
				u64 equalsIndex = 0;
				bool foundEquals = FindSubstring(coordinateStr, "=", &equalsIndex);
				Assert(foundEquals);
				coordinateStr = StrSubstring(&coordinateStr, equalsIndex+1, coordinateStr.length);
				
				u64 columnOrRow = 0;
				bool parsedCoordinate = TryParseU64(coordinateStr, &columnOrRow);
				Assert(parsedCoordinate);
				
				u64 amount = 0;
				bool parsedAmount = TryParseU64(amountStr, &amount);
				Assert(parsedAmount);
				
				if (StrEqualsIgnoreCase(rotateTypeStr, "column"))
				{
					for (u64 yOffset = 0; yOffset < state->screenSize.height; yOffset++) { state->tempPixels[yOffset] = state->pixels[yOffset * state->screenSize.width + columnOrRow]; }
					for (u64 yOffset = 0; yOffset < state->screenSize.height; yOffset++)
					{
						u64 underflowPreventingOffset = 0;
						while (underflowPreventingOffset < amount) { underflowPreventingOffset += state->screenSize.height; }
						state->pixels[yOffset * state->screenSize.width + columnOrRow] = state->tempPixels[(yOffset + (underflowPreventingOffset - amount)) % state->screenSize.height];
					}
				}
				else if (StrEqualsIgnoreCase(rotateTypeStr, "row"))
				{
					for (u64 xOffset = 0; xOffset < state->screenSize.width; xOffset++) { state->tempPixels[xOffset] = state->pixels[columnOrRow * state->screenSize.width + xOffset]; }
					for (u64 xOffset = 0; xOffset < state->screenSize.width; xOffset++)
					{
						u64 underflowPreventingOffset = 0;
						while (underflowPreventingOffset < amount) { underflowPreventingOffset += state->screenSize.width; }
						state->pixels[columnOrRow * state->screenSize.width + xOffset] = state->tempPixels[(xOffset + (underflowPreventingOffset - amount)) % state->screenSize.width];
					}
				}
				else
				{
					PrintLine_E("Unknown rotate type: \"%.*s\" on line %llu", StrPrint(rotateTypeStr), state->parser.lineIndex);
				}
			}
			else
			{
				PrintLine_E("Unknown command on %.*s on line %llu", StrPrint(cmd), state->parser.lineIndex);
			}
		}
		else
		{
			state->completed = true;
			
			#if 1
			u64 numLitPixels = 0;
			for (u64 pIndex = 0; pIndex < state->numPixels; pIndex++)
			{
				if (state->pixels[pIndex] == true) { numLitPixels++; }
			}
			*result = PrintInArenaStr(resultArena, "%llu", numLitPixels);
			#else
			MyStr_t resultStr;
			resultStr.length = state->numPixels + state->screenSize.height;
			resultStr.chars = AllocArray(resultArena, char, resultStr.length+1);
			NotNull(resultStr.chars);
			for (u64 pIndex = 0; pIndex < state->numPixels; pIndex++)
			{
				u64 charIndex = pIndex + (pIndex / state->screenSize.width);
				if (pIndex > 0 && (pIndex % state->screenSize.width) == 0) { resultStr.chars[charIndex-1] = '\n'; }
				resultStr.chars[charIndex] = state->pixels[pIndex] ? '#' : '.';
			}
			resultStr.chars[resultStr.length] = '\0';
			*result = PrintInArenaStr(resultArena, "\n\n%.*s", StrPrint(resultStr));
			// PrintLine_D("Code: %.*s", StrPrint(resultStr));
			#endif
			
			FreeMem(mainHeap, state->pixels, state->numPixels * sizeof(bool));
			FreeMem(mainHeap, state->tempPixels, state->numTempPixels * sizeof(bool));
			state->pixels = nullptr;
			state->tempPixels = nullptr;
		}
	}
	
	progressInfo->amountCompleted = state->parser.lineIndex;
	progressInfo->amountExpected = (state->completed ? progressInfo->amountCompleted : 2000); //Hardcoded again, we should solve this problem somehow
	
	FreeScratchArena(scratch);
	return state->completed;
}

// +==============================+
// |   Calculate_Aoc2016_Day08A   |
// +==============================+
// bool Calculate_Aoc2016_Day08A(void* statePntr, MyStr_t inputFile, bool debug, MemArena_t* resultArena, MyStr_t* result, ProgressInfo_t* progressInfo)
CALCULATE_DAY_FUNC_DEFINITION(Calculate_Aoc2016_Day08B)
{
	NotNull(statePntr);
	MemArena_t* scratch = GetScratchArena(resultArena);
	Aoc2016_Day08_State_t* state = (Aoc2016_Day08_State_t*)statePntr;
	
	if (!state->initialized)
	{
		state->parser = NewLineParser(inputFile);
		state->initialized = true;
	}
	
	if (!state->completed)
	{
		MyStr_t line;
		if (LineParserGetLine(&state->parser, &line))
		{
			Unimplemented(); //TODO: Implement me!
		}
		else
		{
			state->completed = true;
			Unimplemented(); //TODO: Implement me!
		}
	}
	
	progressInfo->amountCompleted = state->parser.lineIndex;
	progressInfo->amountExpected = (state->completed ? progressInfo->amountCompleted : 2000); //Hardcoded again, we should solve this problem somehow
	
	FreeScratchArena(scratch);
	return state->completed;
}
