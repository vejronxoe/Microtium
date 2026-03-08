#pragma once
#include"Collision.h"
#include"BlocksAndWalls.h"
#include "particles.h"
#define TIMEONFIRE 10
enum enemyTypes
{
	enemyZombie = 0
	, enemySlime
	, enemySize
};
class Enemy
{

public:
	Enemy(std::vector<Enemy> enemies
		, unsigned int type
		, unsigned int* enemiesTex1
		, unsigned int* enemiesTex2
		, unsigned int* enemiesDD1
		, unsigned int* enemiesDD2
		, float x
		, float y
		, unsigned int eob);
	unsigned int m_ID;
	char m_TypeOfEnemy;

	float m_Transform[2];
	float m_Velocity[2];
	float m_PlayerHitTimer;
	float m_TimerOutOfCamera;
	short int m_LookAt;
	unsigned int m_DD[2];
	unsigned int m_Tex[2];
	char m_AnimPhase;
	float m_AnimTimer;
	float m_JumpTimer;
	int m_HP;
	int m_Damage;
	bool m_IsBurning;
	float m_BurningTimer;
	int m_BurnDamageNextTime;
	FireParticle m_OnFire;

	void DDAndTexManager(unsigned int eob
		, std::vector<Enemy> enemies
		, unsigned int* enemiesTex1
		, unsigned int* enemiesTex2
		, unsigned int* enemiesDD1
		, unsigned int* enemiesDD2);

	void WhereIsPlayer(float* playerTransform
		, float* distance
		, int* direction);

	bool PlayerInWay(float deltatime
		, float* playerTransform
		, float* oldVelocity
		, float* enemyVertices);

	bool DamageEnemy(int Damage
		, float* transformAttacker);

	int EnemyEveryFrame(float deltaTime
		, std::vector<std::vector<Block>>& blocks
		, float* playerTransform);

	void DrawEnemy(Shader& sh
		, float* transform
		, float* scale);
};

void GetEnemyVerticesByType(unsigned int typeOfEnemy, float* vertices);

void EnemySpawnManager(float deltaTime
	, float& spawnTimer
	, unsigned int eob
	, unsigned int* enemiesTex1
	, unsigned int* enemiesTex2
	, unsigned int* enemiesDD1
	, unsigned int* enemiesDD2
	, float* cameraTransform
	, std::vector< std::vector<Block>>& blocks
	, std::vector<Enemy>& enemies);