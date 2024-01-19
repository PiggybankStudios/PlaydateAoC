/*
File:   calculator_state.cpp
Author: Taylor Robbins
Date:   12\19\2023
Description: 
	** Holds the AppState that runs the calculator for a particular day
*/

CalculatorState_t* calc = nullptr;

void BackToSelectorCallback(void* userPntr)
{
	UNUSED(userPntr);
	calc->backToSelectorRequested = true;
}

// +--------------------------------------------------------------+
// |                            Start                             |
// +--------------------------------------------------------------+
void StartAppState_Calculator(bool initialize, AppState_t prevState, MyStr_t transitionStr)
{
	if (initialize)
	{
		MemArena_t* scratch = GetScratchArena();
		
		NotNull(gl->selectedDayInfo);
		
		calc->dayStateSize = gl->selectedDayInfo->stateSize;
		if (calc->dayStateSize > 0)
		{
			calc->dayState = AllocMem(mainHeap, calc->dayStateSize);
			NotNull(calc->dayState);
			MyMemSet(calc->dayState, 0x00, calc->dayStateSize);
		}
		
		ClearStruct(calc->progress);
		calc->numItersPerCrankMove = 1;
		calc->debugOutputOn = true;
		
		calc->initialized = true;
		FreeScratchArena(scratch);
	}
	
	calc->backToSelectorItem = pd->system->addMenuItem("Back", BackToSelectorCallback, nullptr);
	calc->prevCrankAngle = input->crankAngle;
}

// +--------------------------------------------------------------+
// |                             Stop                             |
// +--------------------------------------------------------------+
void StopAppState_Calculator(bool deinitialize, AppState_t nextState)
{
	pd->system->removeMenuItem(calc->backToSelectorItem);
	calc->backToSelectorItem = nullptr;
	
	if (deinitialize)
	{
		if (calc->dayState != nullptr)
		{
			FreeMem(mainHeap, calc->dayState, calc->dayStateSize);
		}
		FreeString(mainHeap, &calc->answerStr);
		ClearPointer(calc);
	}
}

// +--------------------------------------------------------------+
// |                            Layout                            |
// +--------------------------------------------------------------+
void GameUiLayout()
{
	
}

// +--------------------------------------------------------------+
// |                            Update                            |
// +--------------------------------------------------------------+
void UpdateAppState_Calculator()
{
	MemArena_t* scratch = GetScratchArena();
	
	// +==============================+
	// |     Handle Back Selected     |
	// +==============================+
	if (calc->backToSelectorRequested)
	{
		calc->backToSelectorRequested = false;
		PopAppState();
	}
	
	// +==============================+
	// |   Handle Crank to Iterate    |
	// +==============================+
	u64 numItersToPerform = 0;
	if (CrankMoved() && !calc->doneCalculating)
	{
		HandleCrankDelta();
		r32 crankDelta = (input->crankAngle - calc->prevCrankAngle);
		i32 numSteps = FloorR32i(AbsR32(crankDelta) / CALC_CRANK_ITER_ANGLE);
		if (numSteps > 0)
		{
			calc->prevCrankAngle = ToDegrees32(AngleFixR32(ToRadians32(calc->prevCrankAngle + (CALC_CRANK_ITER_ANGLE * numSteps * SignOfR32(crankDelta)))));
			numItersToPerform = (u64)numSteps * calc->numItersPerCrankMove;
		}
	}
	
	// +==================================+
	// | Btn_Right Does Single Iterations |
	// +==================================+
	if (BtnPressed(Btn_Right))
	{
		HandleBtnExtended(Btn_Right);
		numItersToPerform += 1;
	}
	
	// +==============================+
	// |    Calculate Iteratively     |
	// +==============================+
	if (numItersToPerform > 0)
	{
		for (u64 iIndex = 0; iIndex < numItersToPerform; iIndex++)
		{
			MyStr_t resultStr = MyStr_Empty;
			if (gl->selectedDayInfo->calculateFunc(calc->dayState, calc->debugOutputOn, scratch, &resultStr, &calc->progress))
			{
				calc->answerStr = AllocString(mainHeap, &resultStr);
				calc->doneCalculating = true;
				break;
			}
		}
		calc->numIterationsPerformed += numItersToPerform;
	}
	
	// +==========================================+
	// | Up/Down Changes Num Iters Per Crank Move |
	// +==========================================+
	if (BtnPressed(Btn_Up) || BtnPressed(Btn_Down))
	{
		if (BtnPressed(Btn_Up)) { HandleBtnExtended(Btn_Up); }
		if (BtnPressed(Btn_Down)) { HandleBtnExtended(Btn_Down); }
		u64 options[] = NUM_ITERS_PER_CRANK_MOVE_OPTIONS;
		u64 currentIndex = 0;
		for (u64 oIndex = 0; oIndex < ArrayCount(options); oIndex++)
		{
			if (options[oIndex] == calc->numItersPerCrankMove) { currentIndex = oIndex; break; }
		}
		
		if (BtnPressedRaw(Btn_Up))
		{
			if (currentIndex < ArrayCount(options)-1)
			{
				currentIndex++;
			}
		}
		else
		{
			if (currentIndex > 0)
			{
				currentIndex--;
			}
		}
		
		calc->numItersPerCrankMove = options[currentIndex];
	}
	
	// +==============================+
	// |         Btn_B Resets         |
	// +==============================+
	if (BtnPressed(Btn_B) && calc->doneCalculating)
	{
		HandleBtnExtended(Btn_B);
		if (calc->dayState != nullptr)
		{
			MyMemSet(calc->dayState, 0x00, calc->dayStateSize);
		}
		FreeString(mainHeap, &calc->answerStr);
		calc->doneCalculating = false;
		calc->numIterationsPerformed = 0;
	}
	
	// +===============================+
	// | Btn_Left Toggles Debug Output |
	// +===============================+
	if (BtnPressed(Btn_Left))
	{
		HandleBtnExtended(Btn_Left);
		PrintLine_D("Debug output turned %s", calc->debugOutputOn ? "ON" : "OFF");
		calc->debugOutputOn = !calc->debugOutputOn;
	}
	
	FreeScratchArena(scratch);
}

// +--------------------------------------------------------------+
// |                            Render                            |
// +--------------------------------------------------------------+
void RenderAppState_Calculator(bool isOnTop)
{
	MemArena_t* scratch = GetScratchArena();
	GameUiLayout();
	
	pd->graphics->clear(kColorBlack);
	PdSetDrawMode(kDrawModeCopy);
	
	// +==============================+
	// |    Render Basic Info Text    |
	// +==============================+
	PdSetDrawMode(kDrawModeInverted);
	PdBindFont(&gl->mainFont);
	if (calc->numIterationsPerformed == 0)
	{
		MyStr_t displayStr = NewStr("Crank to calculate...");
		v2i displayStrSize = MeasureText(gl->mainFont.font, displayStr);
		v2i displayStrPos = NewVec2i(ScreenSize.width/2 - displayStrSize.width/2, ScreenSize.height/2 - displayStrSize.height/2);
		PdDrawText(displayStr, displayStrPos);
	}
	else if (!calc->doneCalculating)
	{
		r32 completionAmount = (r32)calc->progress.amountCompleted / (r32)calc->progress.amountExpected;
		MyStr_t displayStr = PrintInArenaStr(scratch, "Calculating %llu %.02f%%...", calc->numIterationsPerformed, completionAmount * 100.0f);
		v2i displayStrSize = MeasureText(gl->mainFont.font, displayStr);
		v2i displayStrPos = NewVec2i(ScreenSize.width/2 - displayStrSize.width/2, ScreenSize.height/2 - displayStrSize.height/2);
		PdDrawText(displayStr, displayStrPos);
		PdDrawTextPrint(NewVec2i(5, ScreenSize.height - 5 - gl->mainFont.lineHeight), "Iters: %llu", calc->numItersPerCrankMove);
		MyStr_t debugOutputStr = PrintInArenaStr(scratch, "Debug: %s", calc->debugOutputOn ? "ON" : "OFF");
		v2i debugOutputSize = MeasureText(gl->mainFont.font, debugOutputStr);
		v2i debugOutputPos = NewVec2i(ScreenSize.width - 5 - debugOutputSize.width, ScreenSize.height - 5 - debugOutputSize.height);
		PdDrawText(debugOutputStr, debugOutputPos);
	}
	else
	{
		MyStr_t displayStr = PrintInArenaStr(scratch, "Answer: %.*s", StrPrint(calc->answerStr));
		v2i displayStrSize = MeasureText(gl->mainFont.font, displayStr);
		v2i displayStrPos = NewVec2i(ScreenSize.width/2 - displayStrSize.width/2, ScreenSize.height/2 - displayStrSize.height/2);
		PdDrawText(displayStr, displayStrPos);
	}
	
	PdSetDrawMode(kDrawModeCopy);
	
	// +==============================+
	// |         Debug Render         |
	// +==============================+
	if (pig->debugEnabled)
	{
		LCDBitmapDrawMode oldDrawMode = PdSetDrawMode(kDrawModeNXOR);
		
		v2i textPos = NewVec2i(1, 1);
		if (pig->perfGraph.enabled) { textPos.y += pig->perfGraph.mainRec.y + pig->perfGraph.mainRec.height + 1; }
		PdBindFont(&pig->debugFont);
		i32 stepY = pig->debugFont.lineHeight + 1;
		
		PdDrawTextPrint(textPos, "Memory: %.2lf%%", ((r64)mainHeap->used / (r64)MAIN_HEAP_MAX_SIZE) * 100.0);
		textPos.y += stepY;
		PdDrawTextPrint(textPos, "ProgramTime: %u (%u)", ProgramTime, input->realProgramTime);
		textPos.y += stepY;
		
		PdSetDrawMode(oldDrawMode);
	}
	
	FreeScratchArena(scratch);
}

// +--------------------------------------------------------------+
// |                           Register                           |
// +--------------------------------------------------------------+
void RegisterAppState_Calculator()
{
	calc = (CalculatorState_t*)RegisterAppState(
		AppState_Calculator,
		sizeof(CalculatorState_t),
		StartAppState_Calculator,
		StopAppState_Calculator,
		UpdateAppState_Calculator,
		RenderAppState_Calculator
	);
}
