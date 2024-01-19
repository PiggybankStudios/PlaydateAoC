/*
File:   calculator_state.h
Author: Taylor Robbins
Date:   12\19\2023
*/

#ifndef _CALCULATOR_STATE_H
#define _CALCULATOR_STATE_H

#define CALC_CRANK_ITER_ANGLE 5 //degrees

struct CalculatorState_t
{
	bool initialized;
	
	PDMenuItem* backToSelectorItem;
	bool backToSelectorRequested;
	
	r32 prevCrankAngle; //degrees
	u64 dayStateSize;
	void* dayState;
	
	u64 numIterationsPerformed;
	bool doneCalculating;
	MyStr_t answerStr;
};

#endif //  _CALCULATOR_STATE_H
