#pragma once
#include"../Opengl/Shader.h"
#include"../player.h"

enum cursor
{
	canNotDoIt = 0,
	canSlideIt,
	canClickOnIt,
	canPickaxeIt,
	canAxeIt,
	canHammerIt,
	canPutBin,
	canOpenChest,
	canOpenDoors

};

unsigned int CreateCursorDrawData(unsigned int* CursorTextures
, unsigned int eob);
void DrawCursor(unsigned int* CursorTextures
	, unsigned int* structurteTextures
	, unsigned int* structurteDD
	, unsigned int cursorDrawData
	, unsigned int blockDrawData
	, Shader& basicSh
	, Shader& structuresSh
	, Shader& fontSh
	, float* transform
	, float* camera
	, float* scale
	, unsigned int fontDrawData
	, unsigned int numberTexture
	, Player& player
	, float* cameraCoordinates);
void DrawCursor(unsigned int* CursorTextures
	, int aimingAtSomething
	, unsigned int cursorDrawData
	, Shader& basicSh
	, float* transform
	, float* camera);