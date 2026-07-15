#include"Enemy.h"

#include"ItemList.h"
#include"glfw/input.h"
#include"Opengl/Texture.h"
#include"Opengl/DrawData.h"
#include"math/matrix.h"
#include"glfw/Window.h"
#include"Collision.h"

#define SPAWNCOLDDOWN 10
#define DESPAWNTIME 10
#define ZOMBIEMOVEMENT 5
#define ZOMBIEWALK 0.5f
#define SLIMEJUMPCOOLDOWN 4
#define COOLDOWNHIT 3

void GetEnemyVerticesByType(unsigned int typeOfEnemy, float* vertices)
{
	switch (typeOfEnemy)
	{
	case enemySlime:
		vertices[0] = -0.9f;
		vertices[1] = 0.8f;
		vertices[2] = 0.9f;
		vertices[3] = -1;
		break;
	case enemyZombie:
		vertices[0] = -0.9f;
		vertices[1] = 1.3f;
		vertices[2] = 0.9f;
		vertices[3] = -1.5f;
		break;
	default:
		Assert(true);
		break;
	}
}
void Enemy::WhereIsPlayer(float* playerTransform
	, float* distance
	, int* direction)
{
	distance[0] = playerTransform[0] - m_Transform[0];
	distance[1] = playerTransform[1] - m_Transform[1];
	if (distance[0])
	{
		direction[0] = distance[0] / abs(distance[0]);
	}
	else
	{
		direction[0] = -1;
	}
	if (distance[1])
	{
		direction[1] = distance[1] / abs(distance[1]);
	}
	else
	{
		direction[1] = -1;
	}
}

bool Enemy::DamageEnemy(int Damage
	, float* transfromAttacker)
{		
	if (transfromAttacker)
	{
		int x = transfromAttacker[0] - m_Transform[0];
		int y = transfromAttacker[1] - m_Transform[1];
		if (x)
		{
			x = abs(x) / x;
		}
		else
		{
			x = 0;
		}

		if (y)
		{
			y = abs(y) / y;
		}
		else
		{
			y = 0;
		}
		m_Velocity[0] = 10 * -x;
		m_Velocity[1] = 5;
	}

	m_HP -= Damage;

	if (m_HP <= 0)
	{
		m_HP = 0;
		return true;
	}
	return false;

}
bool Enemy::PlayerInWay(float deltatime
	, float* playerTransform
	, float* oldVelocity
	, float* enemyVertices)
{
	float vertices[4];
	if (m_Velocity[0] > 0)
	{
		vertices[0] = enemyVertices[0];
		vertices[2] = enemyVertices[2] + oldVelocity[0] * deltatime + 0.5f * (m_Velocity[0] - oldVelocity[0]) * deltatime;
	}
	else
	{
		vertices[0] = enemyVertices[0] + oldVelocity[0] * deltatime + 0.5f * (m_Velocity[0] - oldVelocity[0]) * deltatime;
		vertices[2] = enemyVertices[2];
	}
	if (m_Velocity[1] > 0)
	{
		vertices[3] = enemyVertices[3];
		vertices[1] = enemyVertices[1] + oldVelocity[1] * deltatime + 0.5f * (m_Velocity[1] - oldVelocity[1]) * deltatime;
	}
	else
	{
		vertices[3] = enemyVertices[3] + oldVelocity[1] * deltatime + 0.5f * (m_Velocity[1] - oldVelocity[1]) * deltatime;
		vertices[1] = enemyVertices[1];
	}
	float playerVertices[4] = { playerTransform[0] - 1, playerTransform[1] + 1.5f, playerTransform[0] + 1, playerTransform[1] - 1.5f };
	if (vertices[1] >= playerVertices[3] && vertices[3] <= playerVertices[1] && vertices[2] >= playerVertices[0] && vertices[0] <= playerVertices[2])
	{
		return true;
	}
	return false;
}

void Enemy::DDAndTexManager(unsigned int eob
	, std::vector<Enemy>& enemies
	, unsigned int* enemiesTex1
	, unsigned int* enemiesTex2
	, unsigned int* enemiesDD1
	, unsigned int* enemiesDD2)
{
	bool enemyExist = false;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies.at(i).m_TypeOfEnemy == m_TypeOfEnemy)
		{
			enemyExist = true;
			break;
		}
	}

	if (!enemyExist)
	{
		switch (m_TypeOfEnemy)
		{
		case enemyZombie:
			enemiesTex1[m_TypeOfEnemy] = CreateTextureRGBA("res/textures/zombieAnim.png");
			enemiesTex2[m_TypeOfEnemy] = CreateTextureRGBA("res/textures/zombieBody.png");
			enemiesDD1[m_TypeOfEnemy] = CreateDrawData(eob, -0.2f, -1.5f, -1, 1, 1, 0, 0, 1.0f / 5.0f);
			enemiesDD2[m_TypeOfEnemy] = CreateDrawData(eob, 1.5f, -0.3, -1, 1, 1, 0, 0, 1);
			break;
		case enemySlime:
			enemiesTex1[m_TypeOfEnemy] = CreateTextureRGBA("res/textures/slimeAnim.png");
			enemiesDD1[m_TypeOfEnemy] = CreateDrawData(eob, 1, -1, -1, 1, 1, 0, 0, 1.0f/2.0f);
			break;
		}
	}
}

Enemy::Enemy(std::vector<Enemy>& enemies
	, unsigned int type
	, unsigned int* enemiesTex1
	, unsigned int* enemiesTex2
	, unsigned int* enemiesDD1
	, unsigned int* enemiesDD2
	, float x
	, float y
	, unsigned int eob)
{
	m_TypeOfEnemy = type;

	if (enemies.size())
	{
		m_ID = enemies.at(enemies.size() - 1).m_ID + 1;
	}
	else
	{
		m_ID = 0;
	}
	
	m_PlayerHitTimer = 100;

	int hp[2] = 
	{
		25 
		, 15

	};
	int damage[2] = 
	{
		45
		, 45

	};

	m_HP = hp[m_TypeOfEnemy];
	m_Damage = damage[m_TypeOfEnemy];
	m_Transform[0] = x;
	m_Transform[1] = y;
	m_Velocity[0] = 0;
	m_Velocity[1] = 0;
	m_AnimPhase = 0;
	m_AnimTimer = 0;
	m_JumpTimer = 0;
	DDAndTexManager(eob, enemies, enemiesTex1, enemiesTex2, enemiesDD1, enemiesDD2);
	m_DD[0] = enemiesDD1[m_TypeOfEnemy];
	m_DD[1] = enemiesDD2[m_TypeOfEnemy];
	m_Tex[0] = enemiesTex1[m_TypeOfEnemy];
	m_Tex[1] = enemiesTex2[m_TypeOfEnemy];
	m_IsBurning = false;
	m_BurningTimer = 0;
	m_BurnDamageNextTime = 0;
	m_TimerOutOfCamera = 0;
	float vertices[4];
	GetEnemyVerticesByType(m_TypeOfEnemy, vertices);
	m_OnFire.constructorFire(vertices, 4, 0.2f);

}


int Enemy::EnemyEveryFrame(float deltaTime
	, std::vector<std::vector<Block>>& blocks
	, float* playerTransform)
{
	float oldVelocity[2] = {m_Velocity[0], m_Velocity[1]};
	int RE = 0;
	if (m_PlayerHitTimer < COOLDOWNHIT)
	{
		m_PlayerHitTimer += deltaTime;
	}
	float vertices[4];
	GetEnemyVerticesByType(m_TypeOfEnemy, vertices);
	vertices[0] = m_Transform[0] + vertices[0];
	vertices[1] = m_Transform[1] + vertices[1];
	vertices[2] = m_Transform[0] + vertices[2];
	vertices[3] = m_Transform[1] + vertices[3];
	switch (m_TypeOfEnemy)
	{
	case enemyZombie:
	{
	
		int direction[2];
		float distance[2];
		WhereIsPlayer(playerTransform, distance, direction);
		m_LookAt = direction[0];
		if ((abs(distance[0]) < 2 || ZOMBIEMOVEMENT < m_Velocity[0] * direction[0]) && m_Velocity[0])
		{
			int oldVelocity = abs(m_Velocity[0]) / m_Velocity[0];
			m_Velocity[0] -= direction[0] * ZOMBIEMOVEMENT * deltaTime;
			if (m_Velocity[0])
			{
				if (m_Velocity[0] / abs(m_Velocity[0]) != oldVelocity)
				{
					m_Velocity[0] = 0;
				}
			}
		}
		else
		{
			int multi = 1;
			if (m_Velocity[0])
			{
				if (m_Velocity[0] / abs(m_Velocity[0]) != direction[0])
				{
					multi = 3;
				}
			}
			m_Velocity[0] += direction[0] * ZOMBIEMOVEMENT * deltaTime * multi;
		}
		m_Velocity[1] += deltaTime * GRAVITY;
		if (m_Velocity[1] < GRAVITY)
		{
			m_Velocity[1] = GRAVITY;
		}
		bool hit[4] = { false, false, false, false };
		DynamicHitbox(deltaTime, m_Transform, m_Velocity, oldVelocity, vertices, playerTransform[1] < m_Transform[1],false, blocks, hit[0], hit[2], hit[3], hit[1]);
		if (PlayerInWay(deltaTime, playerTransform, oldVelocity, vertices) && m_PlayerHitTimer >= COOLDOWNHIT)
		{
			RE = m_Damage;
			m_PlayerHitTimer = 0;
		}

		AddVelocityToTransform(vertices, m_Transform, m_Velocity, oldVelocity, hit[3], hit[2], hit[0], hit[1], deltaTime);
		if (hit[3])
		{
			m_AnimTimer += deltaTime;

			if (m_Velocity[0])
			{

				if (0.1f / abs(m_Velocity[0]) >= m_AnimTimer)
				{
					m_AnimPhase = 0;

				}
				else if (ZOMBIEWALK / abs(m_Velocity[0]) >= m_AnimTimer)
				{
					m_AnimPhase = 3;

				}
				else if (ZOMBIEWALK * 2 / abs(m_Velocity[0]) >= m_AnimTimer)
				{
					m_AnimPhase = 1;

				}
				else if (ZOMBIEWALK * 3 / abs(m_Velocity[0]) >= m_AnimTimer)
				{
					m_AnimPhase = 3;

				}
				else if (ZOMBIEWALK * 4 / abs(m_Velocity[0]) >= m_AnimTimer)
				{
					m_AnimPhase = 0;

				}
				else if (ZOMBIEWALK * 5 / abs(m_Velocity[0]) >= m_AnimTimer)
				{
					m_AnimPhase = 4;

				}
				else if (ZOMBIEWALK * 6 / abs(m_Velocity[0]) >= m_AnimTimer)
				{
					m_AnimPhase = 2;

				}
				else if (ZOMBIEWALK * 7 / abs(m_Velocity[0]) >= m_AnimTimer)
				{
					m_AnimPhase = 4;

				}
				else if (ZOMBIEWALK * 8 / abs(m_Velocity[0]) >= m_AnimTimer)
				{
					m_AnimPhase = 0;

				}
				else
				{
					m_AnimTimer = 0;
				}
			}
		}
		else
		{
			m_AnimPhase = 1;
		}
		if ((hit[0] || hit[2] || (Input::SpacePress && abs(distance[1]) < vertices[1] - m_Transform[1] + 1.5f)) && hit[3])
		{
			m_Velocity[1] = 15;
			m_AnimPhase = 1;
		}
		break;
	}
		
	case enemySlime:
	{
		m_Velocity[1] += deltaTime * GRAVITY;
		bool hit[4] = { false, false, false, false };
		int direction[2];
		float distance[2];
		WhereIsPlayer(playerTransform, distance, direction);
		if (m_AnimTimer > -0.5f * m_JumpTimer + 2)
		{
			m_AnimTimer = 0;
			if (m_AnimPhase)
			{
				m_AnimPhase = 0;
			}
			else
			{
				m_AnimPhase = 1;
			}
		}
		m_AnimTimer += deltaTime;
		if (m_JumpTimer < SLIMEJUMPCOOLDOWN)
		{
			m_JumpTimer += deltaTime;
		}
		else
		{
			
			m_Velocity[0] = 15 * direction[0];
			m_JumpTimer = 0;
			m_Velocity[1] = 15;
			m_AnimPhase = 0;
		}
		DynamicHitbox(deltaTime, m_Transform, m_Velocity, oldVelocity, vertices, playerTransform[1] < m_Transform[1],false, blocks, hit[0], hit[2], hit[3], hit[1]);
		if (PlayerInWay(deltaTime, playerTransform, oldVelocity, vertices) && m_PlayerHitTimer >= COOLDOWNHIT)
		{
			RE = m_Damage;
			m_PlayerHitTimer = 0;
		}
	
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, oldVelocity, hit[3], hit[2], hit[0], hit[1], deltaTime);
		m_Velocity[0] += 5 * direction[0] * deltaTime;
		if (hit[3])
		{
			m_Velocity[0] = 0;
		}
		
		break;
	}

	}
	return RE;
}
void Enemy::DrawEnemy(Shader& sh
	, float* transform
	, float* scale)
{
	switch (m_TypeOfEnemy)
	{
	case enemyZombie:
	{
		ChangeTransform(m_Transform[0], m_Transform[1], transform);
		sh.SetUniformMat4(basicTransform, transform);

		ChangeScale(m_LookAt, 1, scale);
		sh.SetUniformMat4(animScale, scale);


		ErrorGL(glBindVertexArray(m_DD[0]));
		sh.SetUniform1i(animNumber, m_AnimPhase);
		sh.SetUniform1i(animLeangth, 5);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex[0]));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

		sh.SetUniform1i(animNumber, 0);
		sh.SetUniform1i(animLeangth, 1);
		if (!m_AnimPhase)
		{
			ChangeTransform(m_Transform[0], m_Transform[1] + 0.1f, transform);
			sh.SetUniformMat4(basicTransform, transform);
		}
		ErrorGL(glBindVertexArray(m_DD[1]));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex[1]));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		break;
	}

	case enemySlime:
	{
		ChangeTransform(m_Transform[0], m_Transform[1], transform);
		sh.SetUniformMat4(basicTransform, transform);

		ChangeScale(1, 1, scale);
		sh.SetUniformMat4(animScale, scale);


		ErrorGL(glBindVertexArray(m_DD[0]));
		sh.SetUniform1i(animNumber, m_AnimPhase);
		sh.SetUniform1i(animLeangth, 2);
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex[0]));
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		break;
	}
	}
	

}
	


bool getLocationForEnemySpawn(unsigned int enemyType
	, float* cameraTransform
	, float* spawntransform
	, std::vector< std::vector<Block>>& blocks)
{
	int spawnVertices[4];
	spawnVertices[1] = RoundFiveDown(cameraTransform[1] + Window::halfHeightOfGameTransform) + 10;
	spawnVertices[3] = RoundFiveUp(cameraTransform[1] - Window::halfHeightOfGameTransform) - 10;

	if (rand() % 2)
	{
		spawnVertices[0] = RoundFiveUp(cameraTransform[0] - Window::halfWidthOfGameTransform) - 30;
		spawnVertices[2] = RoundFiveDown(cameraTransform[0] - Window::halfWidthOfGameTransform);

	}
	else
	{
		spawnVertices[0] = RoundFiveUp(cameraTransform[0] + Window::halfWidthOfGameTransform);
		spawnVertices[2] = RoundFiveDown(cameraTransform[0] + Window::halfWidthOfGameTransform) + 30;

	}
	memoryDefender(spawnVertices, 4);
	float enemyVertices[4] = {};
	GetEnemyVerticesByType(enemyType, enemyVertices);
	int dimensions[2] = { enemyVertices[2] - enemyVertices[0], enemyVertices[1] - enemyVertices[3] };
	spawnVertices[2] -= dimensions[0];
	spawnVertices[1] -= dimensions[1];
	for(int i  = spawnVertices[0]; i < spawnVertices[2]; i++)
	{
		for (int j = spawnVertices[3]; j < spawnVertices[1];j++)
		{
			if (blocks.at(i).at(j - Blocks::yMin).m_Behavior == b_Air)
			{
				bool spaceForEnemy = true;
				for (int k = i; k < i + dimensions[0]; k++)
				{
					for (int l = j; l < j + dimensions[1]; l++)
					{
						if (blocks.at(k).at(l- Blocks::yMin).m_Behavior != b_Air)
						{
							spaceForEnemy = false;
							break;
						}
					}
					if (!spaceForEnemy)
					{
						break;
					}
				}
				if (spaceForEnemy)
				{
					spawntransform[0] = i;
					spawntransform[1] = j;
					return true;
				}
			}
		}
	}
	return false;
}

void EnemySpawnManager(float deltaTime
	, float& spawnTimer
	, unsigned int eob
	, unsigned int* enemiesTex1
	, unsigned int* enemiesTex2
	, unsigned int* enemiesDD1
	, unsigned int* enemiesDD2
	, float* cameraTransform
	, std::vector< std::vector<Block>>& blocks
	, std::vector<Enemy>& enemies)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		Enemy e = enemies.at(i);
		float vertices[4];
		GetEnemyVerticesByType(e.m_TypeOfEnemy, vertices);
		vertices[0] += e.m_Transform[0];
		vertices[1] += e.m_Transform[1];
		vertices[2] += e.m_Transform[0];
		vertices[3] += e.m_Transform[1];
		if (!(vertices[2] < cameraTransform[0] + Window::halfWidthOfGameTransform &&
			vertices[0] > cameraTransform[0] - Window::halfWidthOfGameTransform &&
			vertices[1] < cameraTransform[1] + Window::halfHeightOfGameTransform &&
			vertices[3] > cameraTransform[1] - Window::halfHeightOfGameTransform))
		{
			enemies.at(i) .m_TimerOutOfCamera += deltaTime;
			if (e.m_TimerOutOfCamera > 10)
			{
			
				
				enemies.erase(enemies.begin() + i);
			}
		}
		else
		{
			enemies.at(i).m_TimerOutOfCamera = 0;
		}
	}
	if (spawnTimer > SPAWNCOLDDOWN)
	{
		if (Blocks::yMax / 12.0f > cameraTransform[1] && 0 <= cameraTransform[1])
		{
			if (enemies.size() < 4)
			{
				float transform[2];
				if (getLocationForEnemySpawn(enemySlime, cameraTransform, transform, blocks))
				{
					enemies.emplace_back(enemies, enemySlime, enemiesTex1, enemiesTex2, enemiesDD1, enemiesDD2, transform[0], transform[1], eob);
				}
			}
		}
		else if (0 > cameraTransform[1])
		{
			if (enemies.size() < 6)
			{
				float transform[2];
				if (getLocationForEnemySpawn(enemyZombie, cameraTransform, transform, blocks))
				{
					enemies.emplace_back(enemies, enemyZombie, enemiesTex1, enemiesTex2, enemiesDD1, enemiesDD2, transform[0], transform[1], eob);
				}
			}
		}
		spawnTimer = 0;
	}
	else
	{
		spawnTimer += deltaTime;
	}
}
