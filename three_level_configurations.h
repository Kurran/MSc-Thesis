#ifndef _THREE_LEVEL_CONFIGURATIONS_

#define _THREE_LEVEL_CONFIGURATIONS_

#include <stdio.h>
#include <vector>

namespace three_level_system
{
  constexpr int kCapacity = 6;

  const std::vector<std::vector<std::vector<int>>> configurations {
                                    {{0,0,0}},                                                 // 0 particles
                                    {{1,0,0},{0,1,0},{0,0,1}},                                 // 1 particle
                                    {{2,0,0},{1,1,0},{0,2,0},{0,1,1},{1,0,1},{0,0,2}},         // 2 particles
                                    {{2,1,0},{1,2,0},{1,1,1},{0,2,1},{2,0,1},{1,0,2},{0,1,2}}, // 3 particles
                                    {{2,2,0},{2,1,1},{2,0,2},{1,1,2},{1,2,1},{0,2,2}},         // 4 particles
                                    {{2,2,1},{2,1,2},{1,2,2}},                                 // 5 particles
                                    {{2,2,2}}};                                                // 6 particles

  const std::vector<std::vector<int>> degerancies {
                                    {1},
                                    {2, 2, 2},
                                    {1, 4, 1, 4, 4, 1},
                                    {2, 2, 8, 2, 2, 2, 2},
                                    {1, 4, 1, 4, 1, 4},
                                    {2, 2, 2},
                                    {1}};

  const std::vector<int> num_configurations {1, 6, 15, 20, 15, 6,1};

  const std::vector<int> microstates {1, 3, 6, 7, 6, 3,1};
}

#endif
