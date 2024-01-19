/*
File:   calculator_state.h
Author: Taylor Robbins
Date:   12\19\2023
*/

#ifndef _CALCULATOR_STATE_H
#define _CALCULATOR_STATE_H

#define CALC_CRANK_ITER_ANGLE 20 //degrees
#define NUM_ITERS_PER_CRANK_MOVE_OPTIONS { 1, 3, 5, 10, 30, 50, 100, 300, 500, 1000, 3000, 5000, 10000, 30000, 50000, 100000, 300000, 500000, 1000000 }

struct CalculatorState_t
{
	bool initialized;
	
	PDMenuItem* backToSelectorItem;
	bool backToSelectorRequested;
	
	bool fileChosen;
	bool useTestFile;
	bool doSecondPart;
	
	MyStr_t fileContents;
	u64 dayStateSize;
	void* dayState;
	
	bool debugOutputOn;
	r32 prevCrankAngle; //degrees
	u64 numItersPerCrankMove;
	
	ProgressInfo_t progress;
	u64 numIterationsPerformed;
	bool doneCalculating;
	MyStr_t answerStr;
};

#endif //  _CALCULATOR_STATE_H
