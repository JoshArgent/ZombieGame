#pragma once
#include "header.h"
#include <math.h>
#include <algorithm>
#include <vector>
#include <string>

void rotatePolygon(int num, double xps[], double yps[], double originX, double originY, double angle);

void rotatePolygon(std::vector<double> &xps, std::vector<double> &yps, double originX, double originY, double angle);

double getAngle(double x1, double y1, double x2, double y2);

double getDistanceBetweenPoints(double x1, double y1, double x2, double y2);

bool isPointInsidePolygon(int num, double xps[], double yps[], double x, double y);

bool isPolygonInsidePolygon(int num1, double xps1[], double yps1[], int num2, double xps2[], double yps2[]);

std::vector<std::string> splitString(std::string input, char delimiter);

/*
Returns the smallest element from an array
*/
template <typename T>
T minElement(int num, T* arrayObj)
{
	T min = arrayObj[0];
	for (int i = 1; i < num; i++)
	{
		if (min > arrayObj[i])
			min = arrayObj[i];
	}
	return min;
}

/*
Returns the largest element from an array
*/
template <typename T>
T maxElement(int num, T* arrayObj)
{
	T max = arrayObj[0];
	for (int i = 1; i < num; i++)
	{
		if (max < arrayObj[i])
			max = arrayObj[i];
	}
	return max;
}