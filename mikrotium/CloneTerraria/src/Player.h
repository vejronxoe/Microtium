#pragma once
#include"Opengl/Shader.h"

class Player
{
private:
	unsigned int m_VAO;
	unsigned int m_VB;
	unsigned int m_Tex;
public:
	float m_MaxMovementSpeed;
	float m_vertices[20];
	float m_Transform[2];
	float m_Velocity[2];
	float m_Camera[16];
	Player(unsigned int eob);
	void everyframe(float deltaTime);
	void DrawPlayer(Shader &Sh);
};