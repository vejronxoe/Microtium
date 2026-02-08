#pragma once
#include"Collision.h"
#include"BlocksAndWalls.h"
enum enemyTypes
{
	enemyZombie = 0
	, enemySlime
	, enemySize
};
class Enemy
{
public:
	char m_TypeOfEnemy;
	float m_Transform[2];
	float m_Velocity[2];
	float m_PlayerHitTimer;
	int m_Damage;
		
	void DDAndTexManager(unsigned int eob
		, std::vector<Enemy*> enemis
		, unsigned int* EnemisTex1
		, unsigned int* EnemisTex2
		, unsigned int* EnemisDD1
		, unsigned int* EnemisDD2);

	void WhereIsPlayer(float* playerTransform
		, float* distance
		, int* direction);

	bool PlayerInWay(float deltatime
		, float* playerTransform
		, float* enemyVertices);

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
	Zombie(std::vector<Enemy*> enemis
		, unsigned int* EnemisTex1
		, unsigned int* EnemisTex2
		, unsigned int* EnemisDD1
		, unsigned int* EnemisDD2
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
	Slime(std::vector<Enemy*> enemis
		, unsigned int* EnemisTex1
		, unsigned int* EnemisTex2
		, unsigned int* EnemisDD1
		, unsigned int* EnemisDD2
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