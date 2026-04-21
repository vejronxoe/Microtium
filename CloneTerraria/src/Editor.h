#pragma once
struct Editor
{
	float m_Transform[2] = {40, 0};
	float m_Velocity[2] = {0,0};
	void Update(float deltaTime);

};