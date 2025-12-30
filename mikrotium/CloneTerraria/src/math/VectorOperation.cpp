#include"VectorOperation.h"

#include<iostream>

float Pyt2D(float x, float y)
{
	return sqrt(pow(y, 2) + pow(x, 2));
}

float Pyt2D(float* vector2D)
{
	return sqrt(pow(vector2D[0], 2) + pow(vector2D[1], 2));
}
void NormalizeVector(float* vector2D)
{
	float lenght = sqrt(pow(vector2D[1], 2) + pow(vector2D[0], 2));
	vector2D[0] = vector2D[0] / lenght;
	vector2D[1] = vector2D[1] / lenght;
}
