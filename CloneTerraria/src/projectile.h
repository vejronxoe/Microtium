#pragma once
#include "BlocksAndWalls.h"
#include "Enemy.h"
#include "flora.h"
#include "structures.h"
#include "DroppedItems.h"

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
	, p_Size
};
struct Projectile
{
	std::vector<uint8_t> m_HitEnemies;
	float m_Transform[2];
	float m_Velocity[2];
	unsigned int m_DD;
	unsigned int m_Texture;
	uint8_t m_Damage;
	int8_t m_ProjectileType;
	int8_t m_Bouncing;
	Projectile(unsigned char projectileType
		, float x
		, float y
		, float velocityX
		, float velocityY
		, int damage
		, unsigned int m_DD
		, unsigned int projectileTexture);
	bool HitEnemy(float deltaTime
		, float* oldVelocity
		, float* vertices
		, std::vector<BoomParticle>& particles
		, std::vector<Enemy>& enemies
		, bool Burning);

	bool HitEnemies(float deltaTime
		, float* oldVelocity
		, float* proVertices
		, std::vector<BoomParticle>& particles
		, std::vector<Enemy>& enemies);
	void Draw(Shader& sh
		, float* transform
		, float* scale
		, float* rotation);
};
void ProjectileUpdate(float deltaTime
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
	, unsigned int* blockTextures);

void SandEveryFrame(std::vector<int>& isSandOnX
	, std::vector<Projectile>& projectiles
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& Walls
	, std::vector<int>& chunksToRebuild
	, unsigned int projectileTextures
	, unsigned int blockDD);