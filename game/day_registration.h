/*
File:   day_registration.h
Author: Taylor Robbins
Date:   01\18\2024
*/

#ifdef REGISTER_AOC_DAY

// +==============================+
// |       REGISTER_AOC_DAY       |
// +==============================+
// #define REGISTER_AOC_DAY(year, day, stateStructType, calculateFunction, inputPath1, inputPath2)

REGISTER_AOC_DAY(2016, 1, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_a.txt", "Resources/Text/input_2016_02_b.txt");
REGISTER_AOC_DAY(2016, 2, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_a.txt", "Resources/Text/input_2016_02_b.txt");
REGISTER_AOC_DAY(2016, 3, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_a.txt", "Resources/Text/input_2016_02_b.txt");
REGISTER_AOC_DAY(2016, 4, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_a.txt", "Resources/Text/input_2016_02_b.txt");
REGISTER_AOC_DAY(2016, 5, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_a.txt", "Resources/Text/input_2016_02_b.txt");
REGISTER_AOC_DAY(2016, 6, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_a.txt", "Resources/Text/input_2016_02_b.txt");
REGISTER_AOC_DAY(2016, 7, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_a.txt", "Resources/Text/input_2016_02_b.txt");
REGISTER_AOC_DAY(2016, 8, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_a.txt", "Resources/Text/input_2016_02_b.txt");
REGISTER_AOC_DAY(2016, 9, Aoc2016_Day02_State_t, Calculate_Aoc2016_Day02, "Resources/Text/input_2016_02_a.txt", "Resources/Text/input_2016_02_b.txt");

#undef REGISTER_AOC_DAY

#else //!defined(REGISTER_AOC_DAY)

#include "days/2016/aoc_2016_day_02.cpp"

#endif
