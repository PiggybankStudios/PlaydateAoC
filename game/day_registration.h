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
REGISTER_AOC_DAY(2016, 5, Aoc2016_Day05_State_t, "input_2016_05_test.txt", "input_2016_05_real.txt", Calculate_Aoc2016_Day05A, "18f47a30", Calculate_Aoc2016_Day05B, "-");
REGISTER_AOC_DAY(2016, 6, Aoc2016_Day06_State_t, "input_2016_06_test.txt", "input_2016_06_real.txt", Calculate_Aoc2016_Day06A, "easter", Calculate_Aoc2016_Day06B, "advent");
REGISTER_AOC_DAY(2016, 7, Aoc2016_Day07_State_t, "input_2016_07_test.txt", "input_2016_07_real.txt", Calculate_Aoc2016_Day07A, "2", Calculate_Aoc2016_Day07B, "3");
REGISTER_AOC_DAY(2016, 8, Aoc2016_Day08_State_t, "input_2016_08_test.txt", "input_2016_08_real.txt", Calculate_Aoc2016_Day08A, "6", Calculate_Aoc2016_Day08B, "-");

#undef REGISTER_AOC_DAY

#else //!defined(REGISTER_AOC_DAY)

#include "days/2016/aoc_2016_day_02.cpp"
#include "days/2016/aoc_2016_day_05.cpp"
#include "days/2016/aoc_2016_day_06.cpp"
#include "days/2016/aoc_2016_day_07.cpp"
#include "days/2016/aoc_2016_day_08.cpp"

#endif
