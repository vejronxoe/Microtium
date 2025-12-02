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
	canPutBin,
};

unsigned int CreateCursorDrawData
(unsigned int* CursorTextures
, unsigned int eob);
void DrawCursor
(unsigned int* CursorTextures
, unsigned int cursorDrawData
, unsigned int blockDrawData
, Shader& basicSh
, Shader& fontSh
, unsigned int shadowLocation
, unsigned int transformLocation
, float* transform
, float* camera
, float* scale
, unsigned int cameraLocation
, unsigned int fontDrawData
, unsigned int numberLocation
, unsigned int fontTransformLocation
, unsigned int fontscaleLocation
, unsigned int numberTexture
, Player& player
, float* cameraCoordinates);
