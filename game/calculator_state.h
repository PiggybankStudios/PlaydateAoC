/*
File:   calculator_state.h
Author: Taylor Robbins
Date:   12\19\2023
*/

#ifndef _CALCULATOR_STATE_H
#define _CALCULATOR_STATE_H

struct CalculatorState_t
{
	bool initialized;
	
	PDMenuItem* backToSelectorItem;
	bool backToSelectorRequested;
};

#endif //  _CALCULATOR_STATE_H
