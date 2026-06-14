#pragma once
#include<vector>

#include"BlocksAndWalls.h"

unsigned char DynamicSquereHitbox(float deltaTime
	, float* transform
	, float* velocity
	, float* oldVelocity
	, float* objectVertices4f
	, bool platformControl
	, bool platformIgnore
	, std::vector<std::vector<Block>>& hitbox
	, bool& leftWallHit
	, bool& rightWallHit
	, bool& floorHit
	, bool& ceilHit);
float CameraHitboxY(float y);
float CameraHitboxX(float x);
bool AddVelocityToTransform(float* objectVertices4f
	, float* transform
	, float* velocity
	, float* oldVelocity
	, bool& floorHit
	, bool& rightHit
	, bool& leftHit
	, bool& ceilHit
	, float deltaTime);
int RoundFiveDown(float x);
int RoundFiveUp(float x);
void memoryDefender(int* vertices
	, int lenghtOfArray);