#pragma once
#include"../Opengl/Shader.h"
#include"../player.h"

enum cursor
{
	canNotDoIt = 0,
	canClickOnIt,
	canPickaxeIt,
	canAxeIt,
	canHammerIt,
};

unsigned int CreateCursorDrawData(unsigned int* CursorTextures, unsigned int eob);
void DrawCursor(unsigned int* CursorTextures, unsigned int cursorDrawData, Shader& basicSh, unsigned int transformLocation, float* transform, unsigned int cameraLocation, Player& player);
