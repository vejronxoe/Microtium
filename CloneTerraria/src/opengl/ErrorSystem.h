#pragma once
#include<cassert>
#include<glad/glad.h>
#include<iostream>
#define ErrorGL(x) GLClearError();\
x;\
assert(!GLCheakError(__FILE__,__LINE__))




void GLClearError();
bool GLCheakError(const char* filePath, int line);
