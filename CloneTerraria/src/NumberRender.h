#pragma once
#include"Opengl/Shader.h"

unsigned int CreateDrawDataNumbers(unsigned int eob, unsigned int& numberLocation);
void drawNumber(float bottom, float right, float left, unsigned short int value, unsigned int NumberDrawData, unsigned int numberLocation, unsigned int transformLocation, unsigned int scaleLocation, float* scale, float* transform, Shader Sh);

