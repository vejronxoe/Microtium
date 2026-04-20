#pragma once

#include<GL/glew.h>
#include<iostream>
#ifdef _DEBUG
#define Assert(x) if((x)){ __debugbreak();}
#define ErrorGL(x) GLClearError();\
x;\
Assert(GLCheakError(__FILE__,__LINE__))
#else
#define Assert(x) x
#define ErrorGL(x) x
#endif




void GLClearError();
bool GLCheakError(const char* filePath, int line);
