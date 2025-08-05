#pragma once

#include<vector>

#include"Blocks.h"
#include"Opengl/Shader.h"
#include"Collision.h"


class Player
{
private:
	unsigned int m_VAO;
	unsigned int m_VB;
	unsigned int m_Tex;
	bool m_FloorHit;
	bool m_CeilHit;
	bool m_WallHit;
	float m_CoyoteTimer;
	float m_JumpTimer;
	bool m_CanJump;
public:
	int m_JumpPower;
	float m_Gravity;
	int m_Acceleration;
	int m_Friction;
	float m_MaxMovementSpeed;
	float m_vertices[20];
	float m_Transform[2];
	float m_Velocity[2];
	float m_Camera[16];
	Player(unsigned int eob);
	void EveryFrame(float deltaTime, std::vector<StaticSquereHitbox>& hitbox);
	void DrawPlayer(Shader &Sh);
};

