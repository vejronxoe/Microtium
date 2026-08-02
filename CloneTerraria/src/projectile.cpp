#include "projectile.h"

#include "Collision.h"
#include "itemList.h"
#include "math/matrix.h"
#include "structures.h"

Projectile::Projectile(unsigned char projectileType
	, float x
	, float y
	, float velocityX
	, float velocityY
	, int damage
	, unsigned int DD
	, unsigned int projectileTexture)
	:m_Transform{ x, y }, m_Velocity{ velocityX, velocityY}
{
	m_ProjectileType = projectileType;
	m_DD = DD; 
	m_Texture = projectileTexture;
	m_Damage = damage;
	switch (projectileType)
	{
	case p_BouncingBullet:
	case p_BouncingCannonBall:
	case p_BouncingArrow:
		m_Bouncing = 5;
		break;
	default:
		m_Bouncing = 0;
		break;
	}
}
bool Projectile::HitEnemy(float deltaTime
	, float* oldVelocity
	, float* proVertices
	, std::vector<BoomParticle>& particles
	, std::vector<Enemy>& enemies
	, bool burning)
{
	float vertices[4];
	if (m_Velocity[0] > 0)
	{
		vertices[0] = proVertices[0];
		vertices[2] = proVertices[2] + oldVelocity[0] * deltaTime + 0.5f * (oldVelocity[0] - m_Velocity[0]) * deltaTime;
	}
	else
	{
		vertices[0] = proVertices[0] + oldVelocity[0] * deltaTime + 0.5f * (oldVelocity[0] - m_Velocity[0]) * deltaTime;
		vertices[2] = proVertices[2];
	}
	if (m_Velocity[1] > 0)
	{
		vertices[3] = proVertices[3];
		vertices[1] = proVertices[1] + oldVelocity[1] * deltaTime + 0.5f * (oldVelocity[1] - m_Velocity[1]) * deltaTime;
	}
	else
	{
		vertices[3] = proVertices[3] + oldVelocity[1] * deltaTime + 0.5f * (oldVelocity[1] - m_Velocity[1]) * deltaTime;
		vertices[1] = proVertices[1];
	}
	for (int i = 0; i < enemies.size(); i++)
	{
	
		float enemyVertices[4];
		GetEnemyVerticesByType(enemies.at(i).m_TypeOfEnemy, enemyVertices);
		enemyVertices[0] = enemies.at(i).m_Transform[0] + enemyVertices[0];
		enemyVertices[1] = enemies.at(i).m_Transform[1] + enemyVertices[1];
		enemyVertices[2] = enemies.at(i).m_Transform[0] + enemyVertices[2];
		enemyVertices[3] = enemies.at(i).m_Transform[1] + enemyVertices[3];
		if (vertices[1] >= enemyVertices[3] && vertices[3] <= enemyVertices[1] && vertices[2] >= enemyVertices[0] && vertices[0] <= enemyVertices[2])
		{
			if (burning)
			{
				enemies.at(i).m_IsBurning = true;
			}
			if (enemies.at(i).DamageEnemy(m_Damage, m_Transform))
			{
				enemies.erase(enemies.begin() + i);
			}
			float c[4] = { 1,0,0,0.8f };
			particles.emplace_back(m_Transform, c,1,8);
			return true;

		}
	}
	return false;
}
bool Projectile::HitEnemies(float deltaTime
	, float* oldVelocity
	, float* proVertices
	, std::vector<BoomParticle>& particles
	, std::vector<Enemy>& enemies)
{
	float vertices[4];
	if (m_Velocity[0] > 0)
	{
		vertices[0] = proVertices[0];
		vertices[2] = proVertices[2] + oldVelocity[0] * deltaTime + 0.5f *(oldVelocity[0] - m_Velocity[0]) * deltaTime;
	}
	else
	{
		vertices[0] = proVertices[0] + oldVelocity[0] * deltaTime + 0.5f * (oldVelocity[0] - m_Velocity[0]) * deltaTime;;
		vertices[2] = proVertices[2];
	}
	if (m_Velocity[1] > 0)
	{
		vertices[3] = proVertices[3];
		vertices[1] = proVertices[1] + oldVelocity[1] * deltaTime + 0.5f * (oldVelocity[1] - m_Velocity[1]) * deltaTime;
	}
	else
	{
		vertices[3] = proVertices[3] + oldVelocity[1] * deltaTime + 0.5f * (oldVelocity[1] - m_Velocity[1]) * deltaTime;;
		vertices[1] = proVertices[1];
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		

		float enemyVertices[4];
		GetEnemyVerticesByType(enemies.at(i).m_TypeOfEnemy, enemyVertices);
		enemyVertices[0] = enemies.at(i).m_Transform[0] + enemyVertices[0];
		enemyVertices[1] = enemies.at(i).m_Transform[1] + enemyVertices[1];
		enemyVertices[2] = enemies.at(i).m_Transform[0] + enemyVertices[2];
		enemyVertices[3] = enemies.at(i).m_Transform[1] + enemyVertices[3];
		if (vertices[1] >= enemyVertices[3] && vertices[3] <= enemyVertices[1] && vertices[2] >= enemyVertices[0] && vertices[0] <= enemyVertices[2])
		{
			bool wasHit = false;
			for (int j =0; j < m_HitEnemies.size(); j++)
			{
				if (m_HitEnemies.at(j) == enemies.at(i).m_ID)
				{
					wasHit = true;
				}
			}
			if (!wasHit)
			{
				if (enemies.at(i).DamageEnemy(m_Damage, m_Transform))
				{
					enemies.erase(enemies.begin() + i);
					i--;
				}
				else
				{
					m_HitEnemies.push_back(enemies.at(i).m_ID);
				}
				float c[4] = { 1,0,0,0.8f };
				particles.emplace_back(m_Transform, c, 1, 8);
			}
		}
	}
	return false;
}

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
	,std::vector<int> & chunksToRebuild
	, unsigned int* blockTextures)
{
	float c[p_Size][4] = {};
	for (int i = 0; i < p_Size;i++)
	{
		switch (i)
		{
		case p_Sand:
			c[i][0] = 0.878;
			c[i][1] = 0.82;
			c[i][2] = 0.192;
			c[i][3] = 1;
			break;
		case p_FireArrow:
		case p_FireBullet:
		case p_FireCannonBall:
			c[i][0] = 0.922;
			c[i][1] = 0.141;
			c[i][2] = 0.141;
			c[i][3] = 1;
			break;
		case p_BasicArrow:
			
			c[i][0] = 0.639;
			c[i][1] = 0.325;
			c[i][2] = 0;
			c[i][3] = 1;
			break;
		case p_BasicCannonBall:
			c[i][0] = 0;
			c[i][1] = 0;
			c[i][2] = 0;
			c[i][3] = 1;
			break;
		case p_PierceBullet:
		case p_PierceCannonBall:
		case p_PierceArrow:
			c[i][0] = 0.612;
			c[i][1] = 0.612;
			c[i][2] = 0.612;
			c[i][3] = 1;
			break;
		case p_BouncingArrow:
		case p_BouncingCannonBall:
		case p_BouncingBullet:
			c[i][0] = 0.165;
			c[i][1] = 0.722;
			c[i][2] = 0.161;
			c[i][3] = 1;

			break;
		case p_BasicBullet:
			c[i][0] = 0.961;
			c[i][1] = 0.722;
			c[i][2] = 0;
			c[i][3] = 1;

			break;
		}
	}
	float blood[4] = { 1,0,0,1 };
	float halfSize[p_Size] = {};
	float gravity[p_Size] = {};
	for (int i = 0; i < p_Size;i++)
	{
		switch (i)
		{
		case p_Sand:
		case p_BouncingCannonBall:
		case p_BasicCannonBall:
		case p_FireCannonBall:
		case p_PierceCannonBall:
			halfSize[i] = 0.4f;
			gravity[i] = 18;
			break;
		case p_BasicArrow:
		case p_PierceArrow:
		case p_BouncingArrow:
		case p_FireArrow:
			halfSize[i] = 0.4f;
			gravity[i] = 8;
			break;
		case p_BouncingBullet:
		case p_BasicBullet:
		case p_FireBullet:
		case p_PierceBullet:
			gravity[i] = 0;
			halfSize[i] = 0.2f;
			break;
		}
	}

	float vertices[4];
	float velocity[2];
	float oldVelocity[2];
	for (int i = 0; i < projectiles.size(); i++)
	{
		velocity[0] = projectiles.at(i).m_Velocity[0];
		velocity[1] = projectiles.at(i).m_Velocity[1];
		oldVelocity[0] = projectiles.at(i).m_Velocity[0];
		oldVelocity[1] = projectiles.at(i).m_Velocity[1];
		bool blockHit[4] = {};
		bool wallHit[4] = {};
		bool destroy = false;

		projectiles.at(i).m_Velocity[1] -= gravity[projectiles.at(i).m_ProjectileType] * deltaTime;
		if (projectiles.at(i).m_Velocity[1] < -30)
		{
			projectiles.at(i).m_Velocity[1] = -30;
		}
		vertices[0] = projectiles.at(i).m_Transform[0] - halfSize[projectiles.at(i).m_ProjectileType]; vertices[1] = projectiles.at(i).m_Transform[1] + halfSize[projectiles.at(i).m_ProjectileType];
		vertices[2] = projectiles.at(i).m_Transform[0] + halfSize[projectiles.at(i).m_ProjectileType]; vertices[3] = projectiles.at(i).m_Transform[1] - halfSize[projectiles.at(i).m_ProjectileType];
		DynamicHitbox(deltaTime, projectiles.at(i).m_Transform, projectiles.at(i).m_Velocity, oldVelocity, vertices, true, true, blocks, blockHit[0], blockHit[1], blockHit[2], blockHit[3]);
		switch (projectiles.at(i).m_ProjectileType)
		{
		case p_BasicArrow:
		case p_BasicBullet:
		case p_BasicCannonBall:
		{
			if (projectiles.at(i).HitEnemy(deltaTime, oldVelocity, vertices, particles, enemies, false))
			{
				destroy = true;
				break;
			}
			if (blockHit[0] == true || blockHit[1] == true || blockHit[2] == true || blockHit[3] == true)
			{
				destroy = true;
			}
			break;
		}
		case p_BouncingArrow:
		case p_BouncingBullet:
		case p_BouncingCannonBall:
		{
			if (projectiles.at(i).HitEnemy(deltaTime, oldVelocity, vertices, particles, enemies, false))
			{
				destroy = true;
				break;
			}
			if (blockHit[2] || blockHit[3])
			{
				projectiles.at(i).m_Bouncing--;
				projectiles.at(i).m_Velocity[1] = -1 * velocity[1];
			}
			if (blockHit[0] || blockHit[1])
			{
				projectiles.at(i).m_Bouncing--;
				projectiles.at(i).m_Velocity[0] = -1 * velocity[0];
			}

			if (projectiles.at(i).m_Bouncing < 0)
			{
				destroy = true;
			}
			break;
		}
		case p_PierceArrow:
		case p_PierceBullet:
		case p_PierceCannonBall:
		{
			if (projectiles.at(i).HitEnemies(deltaTime, oldVelocity, vertices, particles, enemies))
			{
				particles.emplace_back(projectiles.at(i).m_Transform, c[projectiles.at(i).m_ProjectileType], 1, 15);
			}
			if (blockHit[0] == true || blockHit[1] == true || blockHit[2] == true || blockHit[3] == true)
			{
				destroy = true;
			}
			break;
		}
		case p_FireArrow:
		case p_FireBullet:
		case p_FireCannonBall:
		{
			if (projectiles.at(i).HitEnemy(deltaTime, oldVelocity, vertices, particles, enemies, true))
			{
				destroy = true;
			}
			
			if (blockHit[0] == true|| blockHit[1] == true|| blockHit[2] == true || blockHit[3] == true)
			{
				destroy = true;
			}
			break;
		}
		case p_Sand:
		{
			if (blockHit[2])
			{
				int x = roundf(projectiles.at(i).m_Transform[0]);
				int y = roundf(projectiles.at(i).m_Transform[1]);
				bool inBlock = isAnythingOnThisTransform(x, y, blocks, seedlings, Crowns, craftStations, doors, chests);
				if (!inBlock)
				{
					CreateBlock(x, y, t_Sand, chunksToRebuild, blocks, isSandOnX);
				}
				else
				{
					dropItems.emplace_back(x, y, 0, i_Sand, 1, true);
				}
				destroy = true;
				break;
			}
			else if (blockHit[0] || blockHit[1])
			{
				projectiles.at(i).m_Velocity[0] = velocity[0] / -2.0f;
			}
			break;
		}
		}
		AddVelocityToTransform(vertices, projectiles.at(i).m_Transform, projectiles.at(i).m_Velocity, oldVelocity, wallHit[2], wallHit[1], wallHit[0], wallHit[3], deltaTime);
		
		if (wallHit[0] == true || wallHit[1] == true || wallHit[2] == true || wallHit[3] == true)
		{
			destroy = true;
		}
		
		if (destroy)
		{
			particles.emplace_back(projectiles.at(i).m_Transform, c[projectiles.at(i).m_ProjectileType], 1, 15);
			projectiles.erase(projectiles.begin() + i);
			i--;
		}
	
	}
}
void Projectile::Draw(Shader& sh
	, float* transform
	, float* scale
	, float* rotation)
{

	double angle = atan2(m_Velocity[0], m_Velocity[1]) * 180.0 / PI;
	ChangeRotation(-abs(angle), rotation);
	sh.SetUniformMat4(advancedRotation, rotation);
	if (angle)
	{
		ChangeScale(angle/abs(angle),1, scale);
	}
	else
	{
		ChangeScale(1, 1, scale);
	}
	sh.SetUniformMat4(advancedScale, scale);
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(advancedTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Texture));
	ErrorGL(glBindVertexArray(m_DD));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
void SandEveryFrame(std::vector<int>& isSandOnX
	, std::vector<Projectile>& projectiles
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& Walls
	, std::vector<int>& chunksToRebuild
	, unsigned int projectileSand
	, unsigned int blockDD)
{
	for (int i = 0; i < isSandOnX.size(); i++)
	{
		int x = isSandOnX.at(i);
		for (int j = Blocks::yMin + 1; j < Blocks::yMax; j++)
		{
			if (blocks.at(x).at(j - Blocks::yMin).m_Behavior == b_Sand && blocks.at(x).at(j - 1 - Blocks::yMin).m_Behavior == b_Air)
			{
				int sizeBefore = isSandOnX.size();
				projectiles.emplace_back(p_Sand, x, j, 0, -5, 5, blockDD, projectileSand);
				DestroyBlock(chunksToRebuild, blocks, isSandOnX, x, j);
				if (sizeBefore == isSandOnX.size())
				{
					i--;
					break;
				}
			}
		}

	}
}

