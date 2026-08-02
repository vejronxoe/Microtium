#pragma once
#include"Collision.h"
#include"BlocksAndWalls.h"
#include "particles.h"
#define TIMEONFIRE 10
enum enemyTypes : int8_t
{
	en_Slime = 0
	, en_FrostSlime
	, en_SandSlime
	, en_Zombie
	, en_Skeleton
	, en_Mummy
	, en_Worm
	, en_Imp
	, en_Ghost
	, en_Birds
	, en_ThunderBird
	, en_BigImp
	, en_Necromancer
	, enemySize
};
class Enemy
{

public:
	Enemy(std::vector<Enemy>& enemies
		, unsigned int type
		, float x
		, float y
		, unsigned int eob);
	FireParticle m_OnFire;
	float m_Transform[2] = {};
	float m_Velocity[2] = {};
	float m_PlayerHitTimer = 0;
	float m_TimerOutOfCamera = 0 ;
	float m_AnimTimer = 0;
	float m_BurningTimer = 0;
	float m_AbilityTimer =0;
	int m_BurnDamageNextTime =0;
	short int m_LookAt = 1;
	int16_t m_HP = 10;
	bool m_IsBurning = false;
	uint8_t m_ID = 0;
	int8_t m_Damage = 45;
	int8_t m_TypeOfEnemy = en_Slime;
	int8_t m_AnimPhase = 0;


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

	void DrawEnemy(Shader& animSh
		, unsigned int* textures
		, unsigned int* DDs
		, float* transform
		, float* scale);
};

void GetEnemyVerticesByType(unsigned int typeOfEnemy, float* vertices);

void EnemySpawnManager(float deltaTime
	, float& spawnTimer
	, unsigned int eob
	, float* cameraTransform
	, std::vector< std::vector<Block>>& blocks
	, std::vector<Enemy>& enemies);
uint8_t animDraw(Shader& animSh
	, float& timer
	, int* order
	, int orderSize
	, float delayBetweenFrames);