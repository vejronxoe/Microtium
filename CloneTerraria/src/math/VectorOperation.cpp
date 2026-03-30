#include"VectorOperation.h"

#include<iostream>

#include"../glfw/Window.h"
#include"../ItemList.h"

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
bool DoTheyIntersect(float* vec4fa, float* vec4fb)
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
void UITranslatorToPixels(float leftCoordinates
	, float downCoordinates
	, float rightCoordinates
	, float upCoordinates
	, float* vertices4fOutput
	, int stablePoint)
{
	float Scale = 0;
	float sizeDifference[2] = {};
	if (Window::height > Window::width)
	{
		Scale = Window::width;
		
	}
	else
	{
		Scale = Window::height;
	}

	vertices4fOutput[0] = leftCoordinates * Scale;
	vertices4fOutput[1] = upCoordinates * Scale;
	vertices4fOutput[2] = rightCoordinates * Scale;
	vertices4fOutput[3] = downCoordinates * Scale;

	switch (stablePoint)
	{
	case leftTop:
	case leftMiddle:
	case leftBottom:
		break;
	case middleTop:
	case middleMiddle:
	case middleBottom:
		vertices4fOutput[0] += sizeDifference[0];
		vertices4fOutput[2] += sizeDifference[0];
		break;
	case rightTop:
	case rightMiddle:
	case rightBottom:
		vertices4fOutput[0] += sizeDifference[0] * 2;
		vertices4fOutput[2] += sizeDifference[0] * 2;

		break;
	default:
		
		std::cout << "Error vectorOperation.cpp wrong stable point" << std::endl;
		break;
	}
	switch (stablePoint)
	{
	case leftTop:
	case middleTop:
	case rightTop:
		vertices4fOutput[1] += sizeDifference[1] * 2;
		vertices4fOutput[3] += sizeDifference[1] * 2;
		break;
	case leftMiddle:
	case middleMiddle:
	case rightMiddle:
		vertices4fOutput[1] += sizeDifference[1];
		vertices4fOutput[3] += sizeDifference[1];
		break;
	case leftBottom:
	case middleBottom:
	case rightBottom:
		break;
	default:
		std::cout << "Error vectorOperation.cpp wrong stable point" << std::endl;
		break;
	}
	
}
float DistanceOnUI(float distance)
{
	float Scale = 0;
	if (Window::height > Window::width)
	{
		Scale = Window::width;

	}
	else
	{
		Scale = Window::height;
	}
	return distance * Scale;
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