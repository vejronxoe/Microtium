#pragma once
#include<vector>

#include"BlocksAndWalls.h"


float CameraHitboxY(float y);
float CameraHitboxX(float x);
bool AddVelocityToTransform(float* objectVertices4f
	, float* transform
	, float* velocity
	, bool* hit
	, float deltaTime);
int RoundFiveDown(float x);
int RoundFiveUp(float x);

unsigned char DynamicHitbox(float deltaTime
	, float* transform
	, float* velocity
	, float* relVertices
	, bool platformControl
	, bool platformIgnore
	, std::vector<std::vector<Block>>& blocks
	, bool* hit);
	unsigned char CharacterHitbox(float deltaTime
	, float* transform
	, float* velocity
	, float* relVertices
	, bool platformControl
	, bool platformIgnore
	, std::vector<std::vector<Block>>& hitbox
	, bool* hit);