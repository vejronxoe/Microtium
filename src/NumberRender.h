#pragma once
#include"opengl/Shader.h"

unsigned int CreateDrawDataNumbers(unsigned int eob
	, unsigned int& numberTexture
	, unsigned int& dotTex
	, unsigned int& dotDD);
void drawTwoNumbersWithZero(float bottom
	, float right
	, float left
	, unsigned short int value
	, float* scale
	, float* transform
	, Shader Sh);
void drawFloat(float bottom
	, float left
	, float value
	, unsigned int numberTex
	, unsigned int numberDD
	, unsigned int dotTex
	, unsigned int dotDD
	, float* scale
	, float* transform
	, Shader Sh);
void drawNumber(float bottom
	, float right
	, float left
	, unsigned short int value
	, float* scale
	, float* transform
	, Shader Sh);