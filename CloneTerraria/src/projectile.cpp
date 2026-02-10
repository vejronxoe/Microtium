#include "projectile.h"

#include "Collision.h"
#include "itemList.h"
#include "math/matrix.h"

Projectile::Projectile(unsigned char projectileType
	, float x
	, float y
	, float velocityX
	, float velocityY
	, int damage
	, unsigned int DD
	, unsigned int projectileTexture)
	:m_Transform{ x, y }, m_Velocity{ velocityX, velocityY}, m_ProjectileType{projectileType},m_DD{DD}, m_Texture{projectileTexture}
{
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
int Projectile::HitEnemy(float deltaTime
	, float* proVertices
	, std::vector<Enemy*>& enemies)
{
	float vertices[4];
	if (m_Velocity[0] > 0)
	{
		vertices[0] = proVertices[0];
		vertices[2] = proVertices[2] + m_Velocity[0] * deltaTime;
	}
	else
	{
		vertices[0] = proVertices[0] + m_Velocity[0] * deltaTime;
		vertices[2] = proVertices[2];
	}
	if (m_Velocity[1] > 0)
	{
		vertices[3] = proVertices[3];
		vertices[1] = proVertices[1] + m_Velocity[0] * deltaTime;
	}
	else
	{
		vertices[3] = proVertices[3] + m_Velocity[0] * deltaTime;
		vertices[1] = proVertices[1];
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		float enemyVertices[4] = { enemies.at(i)->m_Transform[0] + enemies.at(i)->m_Vertices[0], enemies.at(i)->m_Transform[1] + enemies.at(i)->m_Vertices[1], enemies.at(i)->m_Transform[0] + enemies.at(i)->m_Vertices[2], enemies.at(i)->m_Transform[1] + enemies.at(i)->m_Vertices[3] };
		if (vertices[1] >= enemyVertices[3] && vertices[3] <= enemyVertices[1] && vertices[2] >= enemyVertices[0] && vertices[0] <= enemyVertices[2])
		{
			if (enemies.at(i)->DamageEnemy(m_Damage, m_Transform))
			{
				enemies.erase(enemies.begin() + i);
			}
			return true;

		}
	}
	return false;
}
int Projectile::HitEnemies(float deltaTime
	, float* proVertices
	, std::vector<Enemy*>& enemies)
{
	float vertices[4];
	if (m_Velocity[0] > 0)
	{
		vertices[0] = proVertices[0];
		vertices[2] = proVertices[2] + m_Velocity[0] * deltaTime;
	}
	else
	{
		vertices[0] = proVertices[0] + m_Velocity[0] * deltaTime;
		vertices[2] = proVertices[2];
	}
	if (m_Velocity[1] > 0)
	{
		vertices[3] = proVertices[3];
		vertices[1] = proVertices[1] + m_Velocity[0] * deltaTime;
	}
	else
	{
		vertices[3] = proVertices[3] + m_Velocity[0] * deltaTime;
		vertices[1] = proVertices[1];
	}
	for (int i = 0; i < enemies.size(); i++)
	{
		float enemyVertices[4] = { enemies.at(i)->m_Transform[0] + enemies.at(i)->m_Vertices[0], enemies.at(i)->m_Transform[1] + enemies.at(i)->m_Vertices[1], enemies.at(i)->m_Transform[0] + enemies.at(i)->m_Vertices[2], enemies.at(i)->m_Transform[1] + enemies.at(i)->m_Vertices[3] };
		if (vertices[1] >= enemyVertices[3] && vertices[3] <= enemyVertices[1] && vertices[2] >= enemyVertices[0] && vertices[0] <= enemyVertices[2])
		{
			bool wasHit = false;
			for (int i =0; i < m_HitEnemies.size(); i++)
			{
				if (m_HitEnemies.at(i) == enemies.at(i)->m_ID)
				{
					wasHit = true;
				}
			}
			if (!wasHit)
			{
				if (enemies.at(i)->DamageEnemy(m_Damage, m_Transform))
				{
					enemies.erase(enemies.begin() + i);
					i--;
				}
				else
				{
					m_HitEnemies.push_back(enemies.at(i)->m_ID);
				}
			}
		}
	}
	return false;
}

bool Projectile::EveryFrame(float deltaTime
	, std::vector<Enemy*>& enemies
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	, std::vector<bool>& isSandOnX
	, unsigned int* blockTextures)
{
	float vertices[4];
	float velocity[2];
	velocity[0] = m_Velocity[0];
	velocity[1] = m_Velocity[1];
	bool hit[4];
	hit[0] = false;
	hit[1] = false;
	hit[2] = false;
	hit[3] = false;
	switch (m_ProjectileType)
	{
	case p_Sand:
	{
		m_Velocity[1] -= 18 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.4f; vertices[1] = m_Transform[1] + 0.4f;
		vertices[2] = m_Transform[0] + 0.4f; vertices[3] = m_Transform[1] - 0.4f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemy(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);

		if (hit[2])
		{

			CreateBlock(roundf(m_Transform[0]), roundf(m_Transform[1]), i_Sand, walls, blocks, isSandOnX, blockTextures);
			return true;
		}
		else if (hit[0] || hit[1])
		{
			m_Velocity[0] = velocity[0] / -2.0f;
		}
		break;
	}
	case p_BouncingCannonBall:
	{
		m_Velocity[1] -= 18 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.4f; vertices[1] = m_Transform[1] + 0.4f;
		vertices[2] = m_Transform[0] + 0.4f; vertices[3] = m_Transform[1] - 0.4f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemy(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);
		if (hit[2] || hit[3])
		{
			m_Bouncing--;
			m_Velocity[1] = -1 * velocity[1];
		}
		else if (hit[0] || hit[1])
		{
			m_Bouncing--;
			m_Velocity[0] = -1 * velocity[0];
		}

		if (m_Bouncing < 0)
		{
			return true;
		}
		break;
	}
	case p_PierceCannonBall:
	{
		m_Velocity[1] -= 18 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.4f; vertices[1] = m_Transform[1] + 0.4f;
		vertices[2] = m_Transform[0] + 0.4f; vertices[3] = m_Transform[1] - 0.4f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemies(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);
		if (hit[2])
		{
			return true;
		}
		else if (hit[0] || hit[1])
		{
			m_Velocity[0] = velocity[0] / -2.0f;
		}
		break;
	}
	case p_BasicCannonBall:
	case p_FireCannonBall:
	{
		m_Velocity[1] -= 24 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.4f; vertices[1] = m_Transform[1] + 0.4f;
		vertices[2] = m_Transform[0] + 0.4f; vertices[3] = m_Transform[1] - 0.4f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemy(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);
		if (hit[2])
		{
			return true;
		}
		else if (hit[0] || hit[1])
		{
			m_Velocity[0] = velocity[0] / -2.0f;
		}
		break;
	}
	case p_PierceArrow:
	{
		m_Velocity[1] -= 8 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.4f; vertices[1] = m_Transform[1] + 0.4f;
		vertices[2] = m_Transform[0] + 0.4f; vertices[3] = m_Transform[1] - 0.4f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemies(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);
		if (hit[0] || hit[1] || hit[2] || hit[3])
		{
			return true;
		}

		break;
	}
	case p_BasicArrow:
	case p_FireArrow:
	{
		m_Velocity[1] -= 8 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.4f; vertices[1] = m_Transform[1] + 0.4f;
		vertices[2] = m_Transform[0] + 0.4f; vertices[3] = m_Transform[1] - 0.4f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemy(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);
		if (hit[0] || hit[1] || hit[2] || hit[3])
		{
			return true;
		}

		break;
	}
	case p_BouncingArrow:
	{
		m_Velocity[1] -= 8 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.4f; vertices[1] = m_Transform[1] + 0.4f;
		vertices[2] = m_Transform[0] + 0.4f; vertices[3] = m_Transform[1] - 0.4f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemy(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);

		if (hit[2] || hit[3])
		{
			m_Bouncing--;
			m_Velocity[1] = -1 * velocity[1];
		}
		else if (hit[0] || hit[1])
		{
			m_Bouncing--;
			m_Velocity[0] = -1 * velocity[0];
		}

		if (m_Bouncing < 0)
		{
			return true;
		}
		break;
	}
	case p_PierceBullet:
	{
		m_Velocity[1] -= 4 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.2f; vertices[1] = m_Transform[1] + 0.2f;
		vertices[2] = m_Transform[0] + 0.2f; vertices[3] = m_Transform[1] - 0.2f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemies(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);
		if (hit[0] || hit[1] || hit[2] || hit[3])
		{
			return true;
		}
		break;
	}
	case p_BasicBullet:
	case p_FireBullet:
	{
		m_Velocity[1] -= 4 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.2f; vertices[1] = m_Transform[1] + 0.2f;
		vertices[2] = m_Transform[0] + 0.2f; vertices[3] = m_Transform[1] - 0.2f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemy(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);
		if (hit[0] || hit[1] || hit[2] || hit[3])
		{
			return true;
		}
		break;
	}
	case p_BouncingBullet:
	{
		m_Velocity[1] -= 4 * deltaTime;
		if (m_Velocity[1] < -30)
		{
			m_Velocity[1] = -30;
		}
		vertices[0] = m_Transform[0] - 0.2f; vertices[1] = m_Transform[1] + 0.2f;
		vertices[2] = m_Transform[0] + 0.2f; vertices[3] = m_Transform[1] - 0.2f;
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[1], hit[2], hit[3]);
		if (HitEnemy(deltaTime, vertices, enemies))
		{
			return true;
		}
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[2], hit[1], hit[0], hit[3], deltaTime);
		if (hit[2] || hit[3])
		{
			m_Bouncing--;
			m_Velocity[1] = -1 * velocity[1];
		}
		else if (hit[0] || hit[1])
		{
			m_Bouncing--;
			m_Velocity[0] = -1 * velocity[0];
		}

		if (m_Bouncing < 0)
		{
			return true;
		}
		break;
	}
	default:
		std::cout << "Error unknow Projectil" << m_ProjectileType << std::endl;
		break;
	}
	return false;
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
void SandEveryFrame(std::vector<bool>& isSandOnX
	, std::vector<Projectile>& projectiles
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	,  unsigned int projectileSand
	, unsigned int blockDD)
{
	bool checkNext = false;
	int nextY;
	for (int i = 0; i < isSandOnX.size(); i++)
	{
		if (isSandOnX.at(i))
		{
			checkNext = false;
			for (int j = 0; j < blocks.at(i).size(); j++)
			{
				if (checkNext)
				{
					checkNext = false;
					if (blocks.at(i).at(j).m_Y != nextY)
					{
					
						projectiles.emplace_back(p_Sand, i, nextY + 1, 0, -5, 5,blockDD, projectileSand);
						DestroyBlock(blocks, walls, isSandOnX, i, nextY + 1);
						j--;
						if(isSandOnX.at(i) == false)
						{
							break;
						}
					}
				}
				
				if(blocks.at(i).at(j).m_BlockBehavior == b_Sand)
				{
					checkNext = true;
					nextY = blocks.at(i).at(j).m_Y - 1;
					
				}
			}
		}
	}
}