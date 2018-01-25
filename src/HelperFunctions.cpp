#include "HelperFunctions.h"

/*
Given an array of x y coordinates it will rotate the points about a specified origin
*/
void rotatePolygon(int num, double *xps, double *yps, double originX, double originY, double angle)
{
	for (int i = 0; i < num; i++)
	{
		double x1 = xps[i] - originX;
		double y1 = yps[i] - originY;
		double sinA = sin(angle);
		double cosA = cos(angle);
		xps[i] = (x1 * cosA) - (y1 * sinA) + originX;
		yps[i] = (x1 * sinA) + (y1 * cosA) + originY;
	}
}


void rotatePolygon(std::vector<double> &xps, std::vector<double> &yps, double originX, double originY, double angle)
{
	for (int i = 0; i < xps.size(); i++)
	{
		double x1 = xps[i] - originX;
		double y1 = yps[i] - originY;
		double sinA = sin(angle);
		double cosA = cos(angle);
		xps[i] = (x1 * cosA) - (y1 * sinA) + originX;
		yps[i] = (x1 * sinA) + (y1 * cosA) + originY;
	}
}

/*
Calculates the angle (in radians) between two coordinates
*/
double getAngle(double x1, double y1, double x2, double y2)
{
	double result = atan2(y1 - y2, x1 - x2);
	result += 3.5 * M_PI;
	return result;
}

/*
Calculates the distance between two points
*/
double getDistanceBetweenPoints(double x1, double y1, double x2, double y2)
{
	return sqrt(pow(abs(x2 - x1), 2) + pow(abs(y2 - y1), 2));
}

/*
Determines if a given point is within a polygon
*/
bool isPointInsidePolygon(int num, double xps[], double yps[], double x, double y)
{
	double minX = xps[0];
	double minY = yps[0];
	double maxX = xps[0];
	double maxY = yps[0];
	for (int i = 0; i < num; i++)
	{
		minX = std::min(minX, xps[i]);
		minY = std::min(minY, yps[i]);
		maxX = std::max(maxX, xps[i]);
		maxY = std::max(maxY, yps[i]);
	}
	// Eliminate points well outside the area of the polygon
	if (x < minX || x > maxX || y < minY || y > maxY)
		return false;

	bool in = false;
	for (int i = 0, j = num - 1; i < num; j = i++)
	{
		if ((yps[i] > y) != (yps[j] > y) &&
			x < (xps[j] - xps[i]) * (y - yps[i]) / (yps[j] - yps[i]) + xps[i])
		{
			in = !in;
		}
	}
	return in;
}

/*
Determines if a polygon intersects another polygon
*/
bool isPolygonInsidePolygon(int num1, double xps1[], double yps1[], int num2, double xps2[], double yps2[])
{
	for (int i = 0; i < num2; i++)
	{
		if (isPointInsidePolygon(num1, xps1, yps1, xps2[i], yps2[i]))
			return true;
	}
	return false;
}

/*
Splits a string into a vector of strings by a specified delimiter
*/
std::vector<std::string> splitString(std::string input, char delimiter)
{
	std::vector<std::string> output;
	std::string currentWord;
	for (int i = 0; i < input.size(); i++)
	{
		char c = input[i];
		if (c == delimiter)
		{
			if (currentWord.size() > 0)
			{
				output.push_back(currentWord);
				currentWord.clear();
			}
		}
		else
		{
			currentWord += input[i];
		}
	}
	if (currentWord.size() > 0)
	{
		output.push_back(currentWord);
	}
	return output;
}