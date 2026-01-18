#pragma once
#include "BlocksAndWalls.h"
enum ProjectilesTypes
{
	p_Sand = 0
	, 
};
struct Projectile
{
	float m_Transform[2];
	float m_Velocity[2];
	unsigned char m_ProjectileType;
	unsigned int m_DD;
	unsigned int m_Texture;
	Projectile(unsigned char projectileType
		, float x
		, float y
		, float velocityX
		, float velocityY
		, unsigned int m_DD
		, unsigned int projectileTexture);
	bool EveryFrame(float deltaTime
		, std::vector<std::vector<Block>>& blocks
		, std::vector<std::vector<wall>>& walls
		, std::vector<bool>& isSandOnX
		, unsigned int* blockTextures);
	void Draw(Shader& sh
		, float* transform);
};

void SandEveryFrame(std::vector<bool>& isSandOnX
	, std::vector<Projectile>& projectiles
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	, unsigned int projectileTextures
	, unsigned int blockDD);