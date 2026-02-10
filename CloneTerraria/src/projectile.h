#pragma once
#include "BlocksAndWalls.h"
#include "Enemy.h"
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
};
struct Projectile
{
	std::vector<unsigned int> m_HitEnemies;
	float m_Transform[2];
	float m_Velocity[2];
	unsigned char m_ProjectileType;
	int m_Damage;
	char m_Bouncing;
	unsigned int m_DD;
	unsigned int m_Texture;
	Projectile(unsigned char projectileType
		, float x
		, float y
		, float velocityX
		, float velocityY
		, int damage
		, unsigned int m_DD
		, unsigned int projectileTexture);
	int HitEnemy(float deltaTime
		, float* vertices
		, std::vector<Enemy*>& enemies);
	int HitEnemies(float deltaTime
		, float* proVertices
		, std::vector<Enemy*>& enemies);
	bool EveryFrame(float deltaTime
		, std::vector<Enemy*>& enemies
		, std::vector<std::vector<Block>>& blocks
		, std::vector<std::vector<wall>>& walls
		, std::vector<bool>& isSandOnX
		, unsigned int* blockTextures);
	void Draw(Shader& sh
		, float* transform
		, float* scale
		, float* rotation);
};

void SandEveryFrame(std::vector<bool>& isSandOnX
	, std::vector<Projectile>& projectiles
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	, unsigned int projectileTextures
	, unsigned int blockDD);