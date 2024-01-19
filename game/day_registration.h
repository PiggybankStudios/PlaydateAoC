/*
File:   day_registration.h
Author: Taylor Robbins
Date:   01\18\2024
*/

#ifdef REGISTER_AOC_DAY

// +==============================+
// |       REGISTER_AOC_DAY       |
// +==============================+
// #define REGISTER_AOC_DAY(year, day, stateStructType, testPath, realPath, firstFunc, firstTestStr, secondFunc, secondTestStr)

REGISTER_AOC_DAY(2016, 2, Aoc2016_Day02_State_t, "input_2016_02_test.txt", "input_2016_02_real.txt", Calculate_Aoc2016_Day02A, "1985", Calculate_Aoc2016_Day02B, "5DB3");

#undef REGISTER_AOC_DAY

#else //!defined(REGISTER_AOC_DAY)

#include "days/2016/aoc_2016_day_02.cpp"

#endif
