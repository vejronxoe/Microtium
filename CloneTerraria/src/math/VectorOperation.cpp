#include"math/VectorOperation.h"

#include<iostream>
#include<cmath>

#include"glfw/Window.h"
#include"ItemList.h"

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
	float lenght = sqrt(vector2D[1] * vector2D[1] + vector2D[0] * vector2D[0]);
	vector2D[0] = vector2D[0] / lenght;
	vector2D[1] = vector2D[1] / lenght;
}

bool DoTheyIntersect(float* vec4fa, float* vec4fb)
{
	return	(vec4fa[0] <= vec4fb[2] && vec4fa[2] >= vec4fb[0] && vec4fa[3] <= vec4fb[1] && vec4fa[1] >= vec4fb[3]);
}
bool DoTheyIntersect(int* vec4fa, int* vec4fb)
{
	return	(vec4fa[0] <= vec4fb[2] && vec4fa[2] >= vec4fb[0] && vec4fa[3] <= vec4fb[1] && vec4fa[1] >= vec4fb[3]);
}
bool IsInArea(float* vec4f, float x, float y)
{
	return	(x <= vec4f[2] && x >= vec4f[0] && y <= vec4f[1] && y >= vec4f[3]);
}
bool IsInArea(float* vec4f, float* trasform)
{
	return	(trasform[0] <= vec4f[2] && trasform[0] >= vec4f[0] && trasform[1] <= vec4f[1] && trasform[1] >= vec4f[3]);
}

float Clamp(float value
	, float min
	, float max)
{
	if (value < min)
	{
		return min;
	}
	else if (value > max)
	{
		return max;
	}
	else
	{
		return value;
	}
}