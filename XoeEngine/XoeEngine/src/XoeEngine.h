#pragma once

#include<GLFW/glfw3.h>
#include<GL/glew.h>
#include<iostream>

#include"Renderer.h"

bool SetupEngine(int wideOfWindow, int heightOfWindow,const char* nameOfGame);
void ClearScrean();
void EndProgram();
