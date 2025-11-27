#pragma once
#include<vector>

#include"Blocks.h"

unsigned char DynamicSquereHitbox(float deltaTime, float* transform, float* velocity, float* objectVertices4f, std::vector<std::vector<Block>>& hitbox, bool& leftWallHit, bool& rightWallHit, bool& floorHit, bool& ceilHit);
float CameraHitboxY(float y);
float CameraHitboxX(float x);
bool AddVelocityToTransform(float* objectVertices4f, float* transform, float* velocity, bool& floorHit, float deltaTime);