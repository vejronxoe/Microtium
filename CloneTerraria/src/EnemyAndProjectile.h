#pragma once
#include"Collision.h"
#include"BlocksAndWalls.h"
#include "particles.h"
#include "flora.h"
#include "structures.h"
#include "DroppedItems.h"

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
struct Projectile
{
	std::vector<uint8_t> m_HitEnemies;
	float m_Transform[2];
	float m_Velocity[2];
	uint8_t m_Damage;
	int8_t m_ProjectileType;
	int8_t m_Bouncing;
	Projectile(unsigned char projectileType
		, float x
		, float y
		, float velocityX
		, float velocityY
		, int damage);
	void Draw(Shader& sh
		, unsigned int* DDs
		, unsigned int* texs
		, float* transform
		, float* scale
		, float* rotation);
};


struct Enemy
{

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
		, std::vector<Projectile>& projectiles
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

enum ProjectilesTypes
{
	  p_Sand = 0
	, p_FireArrow
	, p_PierceArrow
	, p_BasicArrow
	, p_BouncingArrow
	, p_BasicCannonBall
	, p_PierceCannonBall
	, p_BouncingCannonBall
	, p_FireCannonBall
	, p_BasicBullet
	, p_PierceBullet
	, p_BouncingBullet
	, p_FireBullet
	, p_FrostSpike
	, p_Size
};





int ProjectileUpdate(float deltaTime
	, std::vector<Projectile>& projectiles
	, std::vector<Enemy>& enemies
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& Walls
	, std::vector<CraftStation>& craftStations
	, std::vector<seedling>& seedlings
	, std::vector<Crown>& Crowns
	, std::vector<DroppedItem>& dropItems
	, std::vector<BoomParticle>& particles
	, std::vector<Door>& doors
	, std::vector<Chest>& chests
	, std::vector<int>& isSandOnX
	, std::vector<int>& chunksToRebuild
	, float* playerPos
	, unsigned int* blockTextures);

void SandEveryFrame(std::vector<int>& isSandOnX
	, std::vector<Projectile>& projectiles
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& Walls
	, std::vector<int>& chunksToRebuild);