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
	if (CrankMoved() && !calc->doneCalculating)
	{
		HandleCrankDelta();
		r32 crankDelta = (input->crankAngle - calc->prevCrankAngle);
		i32 numSteps = FloorR32i(AbsR32(crankDelta) / CALC_CRANK_ITER_ANGLE);
		if (numSteps > 0)
		{
			calc->prevCrankAngle = ToDegrees32(AngleFixR32(ToRadians32(calc->prevCrankAngle + (CALC_CRANK_ITER_ANGLE * numSteps * SignOfR32(crankDelta)))));
			calc->numIterationsPerformed += (u64)numSteps;
			MyStr_t resultStr = MyStr_Empty;
			if (gl->selectedDayInfo->calculateFunc(calc->dayState, (u64)numSteps, scratch, &resultStr, &calc->progress))
			{
				calc->answerStr = AllocString(mainHeap, &resultStr);
				calc->doneCalculating = true;
			}
		}
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
	PdBindFont(&pig->debugFont);
	if (calc->numIterationsPerformed == 0)
	{
		PdDrawText("Crank to calculate...", NewVec2i(150, 115));
	}
	else if (!calc->doneCalculating)
	{
		r32 completionAmount = (r32)calc->progress.amountCompleted / (r32)calc->progress.amountExpected;
		PdDrawTextPrint(NewVec2i(150, 115), "Calculating %llu %.02f%%...", calc->numIterationsPerformed, completionAmount * 100.0f);
	}
	else
	{
		PdDrawTextPrint(NewVec2i(150, 115), "Answer: %.*s", StrPrint(calc->answerStr));
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
