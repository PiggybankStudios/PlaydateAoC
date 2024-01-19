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
		
		calc->initialized = true;
		FreeScratchArena(scratch);
	}
	
	calc->backToSelectorItem = pd->system->addMenuItem("Back", BackToSelectorCallback, nullptr);
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
	
	if (calc->backToSelectorRequested)
	{
		calc->backToSelectorRequested = false;
		PopAppState();
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
	
	//TODO: Remove me!
	PdSetDrawMode(kDrawModeInverted);
	PdBindFont(&pig->debugFont);
	PdDrawText("The calculator...", NewVec2i(180, 115));
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
