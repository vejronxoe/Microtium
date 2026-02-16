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
	unsigned int m_ID;
	char m_TypeOfEnemy;
	float m_Vertices[4];
	float m_Transform[2];
	float m_Velocity[2];
	float m_PlayerHitTimer;
	int m_HP;
	int m_Damage;
	bool m_IsBurning;
	float m_BurningTimer;
	int m_BurnDamageNextTime;
	FireParticle m_OnFire;

	void DDAndTexManager(unsigned int eob
		, std::vector<Enemy*> enemies
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

	virtual int EnemyEveryFrame(float deltaTime
		, std::vector<std::vector<Block>>& blocks
		, float* playerTransform) = 0;

	virtual void DrawEnemy(Shader& sh
		, float* transform
		, float* scale) = 0;
};
class Zombie: public Enemy
{
public:
	short int m_LookAt;
	unsigned int m_DD[2];
	unsigned int m_Tex[2];
	char m_AnimPhase;
	float m_AnimTimer;
	Zombie(std::vector<Enemy*> enemies
		, unsigned int* enemiesTex1
		, unsigned int* enemiesTex2
		, unsigned int* enemiesDD1
		, unsigned int* enemiesDD2
		, float x
		, float y
		, unsigned int eob);

	int EnemyEveryFrame(float deltaTime
		, std::vector<std::vector<Block>>& blocks
		, float* playerTransform) override;

	void DrawEnemy(Shader& sh
		, float* transform
		, float* scale) override;


};
class Slime : public Enemy
{
public:
	unsigned int m_DD;
	unsigned int m_Tex;
	float m_JumpTimer;
	float m_AnimTimer;
	char m_AnimPhase;
	Slime(std::vector<Enemy*> enemies
		, unsigned int* enemiesTex1
		, unsigned int* enemiesTex2
		, unsigned int* enemiesDD1
		, unsigned int* enemiesDD2
		, float x
		, float y
		, unsigned int eob);
	int EnemyEveryFrame(float deltaTime
		, std::vector<std::vector<Block>>& blocks
		, float* playerTransform) override;

	void DrawEnemy(Shader& sh
		, float* transform
		, float* scale) override;

};