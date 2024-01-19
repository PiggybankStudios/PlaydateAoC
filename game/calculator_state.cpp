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
		pig->debugOutputDisabledLevels[DbgLevel_Debug] = !calc->debugOutputOn;
		
		calc->fileChosen = false;
		calc->useTestFile = true;
		
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
		FreeString(mainHeap, &calc->fileContents);
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
	// |  Handle Choosing Input File  |
	// +==============================+
	if (!calc->fileChosen)
	{
		if (BtnPressed(Btn_Left) || BtnPressed(Btn_Right))
		{
			if (BtnPressed(Btn_Left)) { HandleBtnExtended(Btn_Left); }
			if (BtnPressed(Btn_Right)) { HandleBtnExtended(Btn_Right); }
			calc->useTestFile = !calc->useTestFile;
		}
		
		if (BtnPressed(Btn_A))
		{
			HandleBtnExtended(Btn_A);
			MyStr_t inputFilePath = (calc->useTestFile ? gl->selectedDayInfo->testInputPath : gl->selectedDayInfo->realInputPath);
			if (ReadEntireFile(false, inputFilePath, &calc->fileContents, mainHeap))
			{
				calc->fileChosen = true;
			}
			else
			{
				PrintLine_E("Failed to open input file at \"%.*s\"", StrPrint(inputFilePath));
			}
		}
	}
	
	// +==============================+
	// |   Handle Crank to Iterate    |
	// +==============================+
	u64 numItersToPerform = 0;
	if (CrankMoved() && !calc->doneCalculating && calc->fileChosen)
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
	if (BtnPressed(Btn_Right) && calc->fileChosen && !calc->doneCalculating)
	{
		HandleBtnExtended(Btn_Right);
		numItersToPerform += 1;
	}
	
	// +==============================+
	// |    Calculate Iteratively     |
	// +==============================+
	if (calc->fileChosen)
	{
		for (u64 iIndex = 0; iIndex < numItersToPerform; iIndex++)
		{
			MyStr_t resultStr = MyStr_Empty;
			calc->numIterationsPerformed++;
			if (gl->selectedDayInfo->calculateFunc(calc->dayState, calc->fileContents, calc->debugOutputOn, scratch, &resultStr, &calc->progress))
			{
				calc->answerStr = AllocString(mainHeap, &resultStr);
				calc->doneCalculating = true;
				break;
			}
		}
	}
	
	// +==========================================+
	// | Up/Down Changes Num Iters Per Crank Move |
	// +==========================================+
	if ((BtnPressed(Btn_Up) || BtnPressed(Btn_Down)) && calc->fileChosen)
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
	
	// +==========================================+
	// | Btn_B Resets or Returns to Day Selection |
	// +==========================================+
	if (BtnPressed(Btn_B))
	{
		HandleBtnExtended(Btn_B);
		if (calc->fileChosen || calc->doneCalculating)
		{
			if (calc->dayState != nullptr)
			{
				MyMemSet(calc->dayState, 0x00, calc->dayStateSize);
			}
			if (calc->doneCalculating) { FreeString(mainHeap, &calc->answerStr); }
			FreeString(mainHeap, &calc->fileContents);
			calc->doneCalculating = false;
			calc->fileChosen = false;
			calc->numIterationsPerformed = 0;
			ClearStruct(calc->progress);
		}
		else
		{
			PopAppState();
		}
	}
	
	// +===============================+
	// | Btn_Left Toggles Debug Output |
	// +===============================+
	if (BtnPressed(Btn_Left) && calc->fileChosen)
	{
		HandleBtnExtended(Btn_Left);
		PrintLine_D("Debug output turned %s", calc->debugOutputOn ? "ON" : "OFF");
		calc->debugOutputOn = !calc->debugOutputOn;
		pig->debugOutputDisabledLevels[DbgLevel_Debug] = !calc->debugOutputOn;
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
	PdSetDrawMode(kDrawModeInverted);
	
	// +==============================+
	// |      Render File Choice      |
	// +==============================+
	if (!calc->fileChosen)
	{
		PdBindFont(&gl->mainFont);
		reci leftRec = NewReci(0, 0, ScreenSize.width/2, ScreenSize.height);
		v2i leftCenter = NewVec2i(leftRec.x + leftRec.width/2, leftRec.y + leftRec.height/2 - boundFont->lineHeight/2);
		reci rightRec = NewReci(ScreenSize.width/2, 0, ScreenSize.width/2, ScreenSize.height);
		v2i rightCenter = NewVec2i(rightRec.x + rightRec.width/2, rightRec.y + rightRec.height/2 - boundFont->lineHeight/2);
		
		if (calc->useTestFile)
		{
			leftRec = ReciDeflate(leftRec, Vec2iFill(RoundR32i(Oscillate(0, 8, 1500))));
		}
		else
		{
			rightRec = ReciDeflate(rightRec, Vec2iFill(RoundR32i(Oscillate(0, 8, 1500))));
		}
		
		PdDrawRecOutline(leftRec, 10, false, kColorBlack);
		PdDrawRecOutline(leftRec, 5, false, kColorWhite);
		PdDrawText("Test", leftCenter, TextAlign_Center);
		if (calc->useTestFile)
		{
			LCDBitmapDrawMode oldDrawMode = PdSetDrawMode(kDrawModeNXOR);
			PdDrawRec(leftRec, kColorBlack);
			PdSetDrawMode(oldDrawMode);
		}
		
		PdDrawRecOutline(rightRec, 10, false, kColorBlack);
		PdDrawRecOutline(rightRec, 5, false, kColorWhite);
		PdDrawText("Real", rightCenter, TextAlign_Center);
		if (!calc->useTestFile)
		{
			LCDBitmapDrawMode oldDrawMode = PdSetDrawMode(kDrawModeNXOR);
			PdDrawRec(rightRec, kColorBlack);
			PdSetDrawMode(oldDrawMode);
		}
	}
	// +==============================+
	// |    Render Basic Info Text    |
	// +==============================+
	else
	{
		PdBindFont(&gl->mainFont);
		v2i screenCenter = NewVec2i(ScreenSize.width/2, ScreenSize.height/2);
		v2i bottomRight = NewVec2i(ScreenSize.width - 5, ScreenSize.height - 5 - boundFont->lineHeight);
		if (!calc->doneCalculating)
		{
			r32 completionAmount = 0.0f;
			if (calc->progress.amountExpected > 0)
			{
				completionAmount = (r32)calc->progress.amountCompleted / (r32)calc->progress.amountExpected;
			}
			PdDrawTextPrintEx(screenCenter, TextAlign_Center, "Calculating %llu %.02f%%...", calc->numIterationsPerformed, (double)(completionAmount * 100.0f));
			PdDrawTextPrint(NewVec2i(5, ScreenSize.height - 5 - gl->mainFont.lineHeight), "Iters: %llu", calc->numItersPerCrankMove);
			PdDrawTextPrintEx(bottomRight, TextAlign_Right, "Debug: %s", calc->debugOutputOn ? "ON" : "OFF");
		}
		else
		{
			PdDrawTextPrintEx(screenCenter, TextAlign_Center, "Answer: %.*s", StrPrint(calc->answerStr));
			PdDrawTextPrintEx(bottomRight, TextAlign_Right, "Iterations: %llu", calc->numIterationsPerformed);
		}
	}
	
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
