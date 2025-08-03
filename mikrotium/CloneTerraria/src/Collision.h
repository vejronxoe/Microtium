#pragma once

#include<vector>


class StaticSquereHitbox
{
public:
	float m_Transform[2];
	float m_Vertices[4];
	bool m_Active;
	StaticSquereHitbox(float x, float y);
};
void DynamicSquereHitbox(float deltaTime, float* transform, float* velocity, float* objectVertices4f, std::vector<StaticSquereHitbox>& hitbox, bool& wallHit, bool& floorHit);
