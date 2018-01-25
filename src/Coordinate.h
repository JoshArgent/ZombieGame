#pragma once
#ifndef COORDINATE
#define COORDINATE

struct Coordinate
{
	Coordinate(int x, int y) : x(x), y(y) {}
	int x;
	int y;
};

#endif