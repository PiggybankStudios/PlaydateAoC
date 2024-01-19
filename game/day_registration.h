/*
File:   day_registration.h
Author: Taylor Robbins
Date:   01\18\2024
*/

#ifdef REGISTER_AOC_DAY

// +==============================+
// |       REGISTER_AOC_DAY       |
// +==============================+
// #define REGISTER_AOC_DAY(year, day, stateStructType, calculateFunction, testPath, realPath)

REGISTER_AOC_DAY(2016, 2, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_test.txt", "Resources/Text/input_2016_02.txt");
REGISTER_AOC_DAY(2016, 2, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02x, "Resources/Text/input_2016_02_test.txt", "Resources/Text/input_2016_02.txt");

#undef REGISTER_AOC_DAY

#else //!defined(REGISTER_AOC_DAY)

#include "days/2016/aoc_2016_day_02.cpp"

#endif
