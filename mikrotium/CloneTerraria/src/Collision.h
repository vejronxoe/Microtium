#pragma once
#include<vector>

#include"Blocks.h"


unsigned char DynamicSquereHitbox(float deltaTime, float* transform, float* velocity, float* objectVertices4f, std::vector<Block>& hitbox, bool& leftWallHit, bool& rightWallHit, bool& floorHit, bool& ceilHit);
