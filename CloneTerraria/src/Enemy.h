#pragma once
#include"Collision.h"
#include"BlocksAndWalls.h"
enum enemyTypes
{
	enemyZombie = 0
	, enemySlime
};
struct Enemy
{
	char m_TypeOfEnemy;
	float m_Transform[2];
	float m_Velocity[2];
	float m_PlayerHitTimer;
	int m_Damage;
	unsigned int m_tex;
	unsigned int m_DD;
	Enemy(unsigned int typeOfEnemy
		, float x
		, float y
		, unsigned int eob);
	int EnemyEveryFrame(float deltaTime
		, std::vector<std::vector<Block>> blocks
		, float* playerTransform);
	void WhereIsPlayer(float* playerTransform
		, float* distance
		, int* direction);
	bool PlayerInWay(float deltatime, float* playerTransform, float* enemyVertices);
	void DrawEnemy(Shader sh
		, float* transform);
};