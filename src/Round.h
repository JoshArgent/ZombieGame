#include <math.h>
#include <algorithm>
#pragma once

class Round
{
public:
	Round(int round);
	~Round();

	int round;

	inline double GetZombieSpawnRate()
	{
		// Growth formua for given round no.: 0.3 * (1+0.05)^r
		// Limit the rate at 0.95
		return std::min(0.3 * pow(1 + 0.05, round), 0.95);
	}

	inline int GetZombieSpawnLimit()
	{
		// Growth formula for given round no.: 10 * (1+0.1)^r
		// Limit the max spawned to 30
		return std::min((int) std::round(10 * pow(1 + 0.1, round)), 30);
	}

	inline int GetNumZombies()
	{
		// How many zombies each round will feature until it is complete
		return 10 + (round * 10);
	}

	inline int GetZombieHealth()
	{
		// How many hits does a zombie need to die
		// Growth rate: 1 * (1+0.05)^r - 1
		return std::min((int) std::round(pow(1 + 0.05, 10)), 4) - 1;
	}	
};

