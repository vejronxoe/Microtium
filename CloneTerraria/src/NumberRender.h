#pragma once
#include"Opengl/Shader.h"

unsigned int CreateDrawDataNumbers(unsigned int eob, unsigned int& numberLocation);
void drawNumber(float bottom, float right, float left, unsigned short int value, unsigned int NumberDrawData, float* scale, float* transform, Shader Sh);
void drawTwoNumberWithZero(float bottom, float right, float left, unsigned short int value, unsigned int NumberDrawData, float* scale, float* transform, Shader Sh);

