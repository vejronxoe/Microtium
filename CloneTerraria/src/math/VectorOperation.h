#pragma once

float Pyt2D(float x
	, float y);
float Pyt2D(float* vector2D);
void NormalizeVector(float* vector2D);
bool IsInArea(float* vec4f
	, float* trasform);
bool IsInArea(float* vec4f
	, float x
	, float y);
bool DoTheyIntersect(float* vec4fa
	, float* vec4fb);
float Clamp(float value
	, float min
	, float max);
bool DoTheyIntersect(int* vec4fa
	, int* vec4fb);
