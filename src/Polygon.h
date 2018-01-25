#pragma once
#include <vector>

struct Polygon
{
public:
	Polygon(std::vector<double> xps, std::vector<double> yps);
	~Polygon();
	int num;
	std::vector<double> xps;
	std::vector<double> yps;
};

