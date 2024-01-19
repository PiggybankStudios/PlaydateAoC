/*
File:   day_selector_state.cpp
Author: Taylor Robbins
Date:   12\19\2023
Description: 
	** Holds the AppState that lets the user choose which day to calculate
*/

//TODO: Fix this, it's broken, only one button shows on screen
//TODO: Add scrolling behavior when the selected button changes

DaySelectorState_t* dsel = nullptr;

// +--------------------------------------------------------------+
// |                            Start                             |
// +--------------------------------------------------------------+
void StartAppState_DaySelector(bool initialize, AppState_t prevState, MyStr_t transitionStr)
{
	if (initialize)
	{
		CreateVarArray(&dsel->buttons, mainHeap, sizeof(DayBtn_t), gl->dayInfos.length);
		VarArrayLoop(&gl->dayInfos, dIndex)
		{
			VarArrayLoopGet(DayInfo_t, dayInfo, &gl->dayInfos, dIndex);
			DayBtn_t* newBtn = VarArrayAdd(&dsel->buttons, DayBtn_t);
			NotNull(newBtn);
			ClearPointer(newBtn);
			newBtn->dayInfo = dayInfo;
			newBtn->mainRec.size = DSEL_BTN_SIZE;
		}
		
		dsel->initialized = true;
	}
}

// +--------------------------------------------------------------+
// |                             Stop                             |
// +--------------------------------------------------------------+
void StopAppState_DaySelector(bool deinitialize, AppState_t nextState)
{
	if (deinitialize)
	{
		FreeVarArray(&dsel->buttons);
		ClearPointer(dsel);
	}
}

// +--------------------------------------------------------------+
// |                            Layout                            |
// +--------------------------------------------------------------+
void LayoutAppState_DaySelector()
{
	MemArena_t* scratch = GetScratchArena();
	
	dsel->buttonsListRec.size = Vec2i_Zero;
	VarArrayLoop(&dsel->buttons, bIndex)
	{
		VarArrayLoopGet(DayBtn_t, btn, &dsel->buttons, bIndex);
		dsel->buttonsListRec.width = MaxI32(dsel->buttonsListRec.width, btn->mainRec.width);
		btn->mainRec.x = 0;
		btn->mainRec.y = dsel->buttonsListRec.height;
		if (bIndex > 0) { dsel->buttonsListRec.height += DSEL_BTN_PADDING; }
		dsel->buttonsListRec.height += btn->mainRec.width;
	}
	
	dsel->buttonsListRec.x = ScreenSize.width/2 - dsel->buttonsListRec.width/2;
	dsel->buttonsListRec.y = ScreenSize.height/2 - dsel->buttonsListRec.height/2;
	if (dsel->buttonsListRec.y < DSEL_BTN_PADDING) { dsel->buttonsListRec.y = DSEL_BTN_PADDING; }
	
	dsel->scrollMax = MaxR32(0, (r32)(dsel->buttonsListRec.height + DSEL_BTN_PADDING*2 - ScreenSize.height));
	dsel->scroll = ClampR32(dsel->scroll, 0, dsel->scrollMax);
	dsel->scrollGoto = ClampR32(dsel->scrollGoto, 0, dsel->scrollMax);
	
	FreeScratchArena(scratch);
}

// +--------------------------------------------------------------+
// |                            Update                            |
// +--------------------------------------------------------------+
void UpdateAppState_DaySelector()
{
	MemArena_t* scratch = GetScratchArena();
	LayoutAppState_DaySelector();
	
	// +==============================+
	// |         Handle Crank         |
	// +==============================+
	if (CrankMoved())
	{
		HandleCrankDelta();
		r32 crankDelta = (input->crankAngle - dsel->prevCrankAngle);
		i32 numSteps = FloorR32i(AbsR32(crankDelta) / DSEL_CRANK_SENSITIVITY);
		if (numSteps > 0)
		{
			for (i32 sIndex = 0; sIndex < numSteps; sIndex++)
			{
				if (dsel->selectionIndex < 0)
				{
					if (dsel->buttons.length > 0)
					{
						dsel->selectionIndex = 0;
					}
				}
				else if (crankDelta > 0)
				{
					dsel->selectionIndex++;
					if ((u64)dsel->selectionIndex >= dsel->buttons.length) { dsel->selectionIndex = 0; }
				}
				else
				{
					dsel->selectionIndex--;
					if (dsel->selectionIndex < 0) { dsel->selectionIndex = (i32)(dsel->buttons.length-1); }
				}
			}
		}
	}
	
	// +==============================+
	// |       Handle Btn_Down        |
	// +==============================+
	if (BtnPressed(Btn_Down))
	{
		HandleBtnExtended(Btn_Down);
		if (dsel->selectionIndex < 0)
		{
			if (dsel->buttons.length > 0)
			{
				dsel->selectionIndex = 0;
			}
		}
		else
		{
			dsel->selectionIndex++;
			if ((u64)dsel->selectionIndex >= dsel->buttons.length) { dsel->selectionIndex = 0; }
		}
	}
	
	// +==============================+
	// |        Handle Btn_Up         |
	// +==============================+
	if (BtnPressed(Btn_Up))
	{
		HandleBtnExtended(Btn_Up);
		if (dsel->selectionIndex < 0)
		{
			if (dsel->buttons.length > 0)
			{
				dsel->selectionIndex = 0;
			}
		}
		else
		{
			dsel->selectionIndex--;
			if (dsel->selectionIndex < 0) { dsel->selectionIndex = (i32)(dsel->buttons.length-1); }
		}
	}
	
	// +==============================+
	// |    Btn_A Selects a Button    |
	// +==============================+
	if (BtnPressed(Btn_A))
	{
		HandleBtnExtended(Btn_A);
		if (dsel->selectionIndex >= 0)
		{
			DayBtn_t* selectedBtn = VarArrayGet(&dsel->buttons, (u64)dsel->selectionIndex, DayBtn_t);
			PrintLine_I("Selected Year %u Day %u...", (u32)selectedBtn->dayInfo->year, (u32)selectedBtn->dayInfo->day);
			gl->selectedDayInfo = selectedBtn->dayInfo;
			PushAppState(AppState_Calculator);
		}
	}
	
	FreeScratchArena(scratch);
}

// +--------------------------------------------------------------+
// |                            Render                            |
// +--------------------------------------------------------------+
void RenderAppState_DaySelector(bool isOnTop)
{
	MemArena_t* scratch = GetScratchArena();
	LayoutAppState_DaySelector();
	
	pd->graphics->clear(kColorWhite);
	PdSetDrawMode(kDrawModeCopy);
	
	// +==============================+
	// |        Render Buttons        |
	// +==============================+
	VarArrayLoop(&dsel->buttons, bIndex)
	{
		VarArrayLoopGet(DayBtn_t, btn, &dsel->buttons, bIndex);
		reci mainRec = btn->mainRec + dsel->buttonsListRec.topLeft + NewVec2i(0, RoundR32i(dsel->scroll));
		// mainRec.y += (i32)bIndex; //TODO: Remove me!
		bool isSelected = (dsel->selectionIndex >= 0 && (u64)dsel->selectionIndex == bIndex);
		
		if (RecisIntersect(mainRec, ScreenRec))
		{
			PdDrawRecOutline(mainRec, isSelected ? DSEL_BTN_OUTLINE_SELECTED_THICKNESS : DSEL_BTN_OUTLINE_THICKNESS);
			
			MyStr_t btnText = PrintInArenaStr(scratch, "%u Day %u", (u32)btn->dayInfo->year, (u32)btn->dayInfo->day);
			v2i btnTextSize = MeasureText(pig->debugFont.font, btnText);
			v2i btnTextPos = mainRec.topLeft + NewVec2i(mainRec.width/2, mainRec.height/2) - NewVec2i(btnTextSize.width/2, btnTextSize.height/2);
			PdBindFont(&pig->debugFont);
			PdDrawText(btnText, btnTextPos);
		}
	}
	
	// +==============================+
	// |      Render Debug Info       |
	// +==============================+
	if (pig->debugEnabled)
	{
		LCDBitmapDrawMode oldDrawMode = PdSetDrawMode(kDrawModeNXOR);
		
		v2i textPos = NewVec2i(1, 1);
		if (pig->perfGraph.enabled) { textPos.y += pig->perfGraph.mainRec.y + pig->perfGraph.mainRec.height + 1; }
		PdBindFont(&pig->debugFont);
		i32 stepY = pig->debugFont.lineHeight + 1;
		
		PdDrawTextPrint(textPos, "Game: v%u.%u(%03u)", GAME_VERSION_MAJOR, GAME_VERSION_MINOR, GAME_VERSION_BUILD);
		textPos.y += stepY;
		PdDrawTextPrint(textPos, "Engine: v%u.%u(%03u)", PIG_VERSION_MAJOR, PIG_VERSION_MINOR, PIG_VERSION_BUILD);
		textPos.y += stepY;
		PdDrawTextPrint(textPos, "Memory: %.2lf%%", ((r64)mainHeap->used / (r64)MAIN_HEAP_MAX_SIZE) * 100.0);
		textPos.y += stepY;
		
		#if 0
		{
			textPos.y += stepY;
			
			PdDrawTextPrint(textPos, "Static: ~%p", &pd);
			textPos.y += stepY;
			PdDrawTextPrint(textPos, "Stack: ~%p", &scratch);
			textPos.y += stepY;
			PdDrawTextPrint(textPos, "Std: %llu", pig->stdHeap.used);
			textPos.y += stepY;
			PdDrawTextPrint(textPos, "PigEngineState_t: %p %u", pig, sizeof(PigEngineState_t));
			textPos.y += stepY;
			PdDrawTextPrint(textPos, "Scratch[0]: %p %llu/%llu", pig->scratchArenas[0].mainPntr, pig->scratchArenas[0].highUsedMark, pig->scratchArenas[0].size);
			textPos.y += stepY;
			PdDrawTextPrint(textPos, "Scratch[1]: %p %llu/%llu", pig->scratchArenas[1].mainPntr, pig->scratchArenas[1].highUsedMark, pig->scratchArenas[1].size);
			textPos.y += stepY;
			PdDrawTextPrint(textPos, "Scratch[2]: %p %llu/%llu", pig->scratchArenas[2].mainPntr, pig->scratchArenas[2].highUsedMark, pig->scratchArenas[2].size);
			textPos.y += stepY;
			PdDrawTextPrint(textPos, "Fixed: %p %llu/%llu", fixedHeap->mainPntr, fixedHeap->highUsedMark, fixedHeap->size);
			textPos.y += stepY;
			
			textPos.y += stepY;
			
			PdDrawTextPrint(textPos, "Main: %llu/%llu %.2lf%% (%llu/%llu pages)", mainHeap->used, mainHeap->size, ((r64)mainHeap->used / (r64)mainHeap->size) * 100.0, mainHeap->numPages, mainHeap->maxNumPages);
			textPos.y += stepY;
			HeapPageHeader_t* pageHeader = (HeapPageHeader_t*)mainHeap->headerPntr;
			u64 pageIndex = 0;
			while (pageHeader != nullptr)
			{
				PdDrawTextPrint(textPos, "  Page[%llu]: %p %llu/%llu %.2lf%%", pageIndex, pageHeader, pageHeader->used, pageHeader->size, ((r64)pageHeader->used / (r64)pageHeader->size) * 100.0);
				textPos.y += stepY;
				pageHeader = pageHeader->next;
				pageIndex++;
			}
		}
		#endif
		
		PdSetDrawMode(oldDrawMode);
	}
	
	FreeScratchArena(scratch);
}

// +--------------------------------------------------------------+
// |                           Register                           |
// +--------------------------------------------------------------+
void RegisterAppState_DaySelector()
{
	dsel = (DaySelectorState_t*)RegisterAppState(
		AppState_DaySelector,
		sizeof(DaySelectorState_t),
		StartAppState_DaySelector,
		StopAppState_DaySelector,
		UpdateAppState_DaySelector,
		RenderAppState_DaySelector
	);
}
