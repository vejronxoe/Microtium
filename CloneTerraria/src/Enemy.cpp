#include"Enemy.h"
#include"ItemList.h"
#include"glfw/input.h"
#include"Opengl/Texture.h"
#include"Opengl/DrawData.h"
#include"math/matrix.h"

#define ZOMBIEMOVEMENT 5
#define ZOMBIEWALK 0.5f
#define SLIMEJUMPCOOLDOWN 4
#define COOLDOWNHIT 3


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
	, float* enemyVertices)
{
	float vertices[4];
	if (m_Velocity[0] > 0)
	{
		vertices[0] = enemyVertices[0];
		vertices[2] = enemyVertices[2] + m_Velocity[0] * deltatime;
	}
	else
	{
		vertices[0] = enemyVertices[0] + m_Velocity[0] * deltatime;
		vertices[2] = enemyVertices[2];
	}
	if (m_Velocity[1] > 0)
	{
		vertices[3] = enemyVertices[3];
		vertices[1] = enemyVertices[1] + m_Velocity[0] * deltatime;
	}
	else
	{
		vertices[3] = enemyVertices[3] + m_Velocity[0] * deltatime;
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
	, std::vector<Enemy*> enemies
	, unsigned int* enemiesTex1
	, unsigned int* enemiesTex2
	, unsigned int* enemiesDD1
	, unsigned int* enemiesDD2)
{
	bool enemyExist = false;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies.at(i)->m_TypeOfEnemy == m_TypeOfEnemy)
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

Zombie::Zombie(std::vector<Enemy*> enemies
	, unsigned int* enemiesTex1
	, unsigned int* enemiesTex2
	, unsigned int* enemiesDD1
	, unsigned int* enemiesDD2
	, float x
	, float y
	, unsigned int eob)
{
	if (enemies.size())
	{
		m_ID = enemies.at(enemies.size() - 1)->m_ID + 1;
	}
	else
	{
		m_ID = 0;
	}
	m_Vertices[0] = -0.9f; 
	m_Vertices[1] = 1.3f; 
	m_Vertices[2] = 0.9f; 
	m_Vertices[3] = -1.5f;
	m_PlayerHitTimer = 0;
	m_TypeOfEnemy = enemyZombie;
	m_HP = 25;
	m_Damage = 45;
	m_Transform[0] = x;
	m_Transform[1] = y;
	m_Velocity[0] = 0;
	m_Velocity[1] = 0;
	m_AnimPhase = 0;
	m_AnimTimer = 0;
	DDAndTexManager(eob, enemies, enemiesTex1, enemiesTex2, enemiesDD1, enemiesDD2);
	m_DD[0] = enemiesDD1[m_TypeOfEnemy];
	m_DD[1] = enemiesDD2[m_TypeOfEnemy];
	m_Tex[0] = enemiesTex1[m_TypeOfEnemy];
	m_Tex[1] = enemiesTex2[m_TypeOfEnemy];
	m_IsBurning = false;
	m_BurningTimer = 0;
	m_BurnDamageNextTime = 0;
	m_OnFire.constructorFire(m_Vertices, 4, 0.2f);
}
int Zombie::EnemyEveryFrame(float deltaTime
	, std::vector<std::vector<Block>>& blocks
	, float* playerTransform)
{
	int RE = 0;
	if (m_PlayerHitTimer < COOLDOWNHIT)
	{
		m_PlayerHitTimer += deltaTime;
	}
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
	float vertices[4];
	vertices[0] = m_Transform[0] + m_Vertices[0];
	vertices[1] = m_Transform[1] + m_Vertices[1];
	vertices[2] = m_Transform[0] + m_Vertices[2];
	vertices[3] = m_Transform[1] + m_Vertices[3];
	DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[2], hit[3], hit[1]);
	if (PlayerInWay(deltaTime, playerTransform, vertices) && m_PlayerHitTimer >= COOLDOWNHIT)
	{
		RE = m_Damage;
		m_PlayerHitTimer = 0;
	}

	AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[3], hit[2], hit[0], hit[1], deltaTime);
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
	return RE;
}
void Zombie::DrawEnemy(Shader& sh
	, float* transform
	, float* scale)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(basicTransform, transform);
	 
	ChangeScale(m_LookAt,1, scale);
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

}
	

Slime::Slime(std::vector<Enemy*> enemies
	, unsigned int* enemiesTex1
	, unsigned int* enemiesTex2
	, unsigned int* enemiesDD1
	, unsigned int* enemiesDD2
	, float x
	, float y
	, unsigned int eob)
{
	m_PlayerHitTimer = 0;
	m_TypeOfEnemy = enemySlime;
	m_HP = 15;
	m_Damage = 45;
	m_Transform[0] = x;
	m_Transform[1] = y;
	m_Velocity[0] = 0;
	m_Velocity[1] = 0;
	m_JumpTimer = 0;
	m_AnimTimer = 0;
	m_AnimPhase = 0;
	m_Vertices[0] = -0.9f;
	m_Vertices[1] = 0.8f;
	m_Vertices[2] = 0.9f;
	m_Vertices[3] = -1;
	DDAndTexManager(eob, enemies, enemiesTex1, enemiesTex2, enemiesDD1, enemiesDD2);
	m_DD = enemiesDD1[m_TypeOfEnemy];
	m_Tex = enemiesTex1[m_TypeOfEnemy];
	m_IsBurning = false;
	m_BurnDamageNextTime = 0;
	m_BurningTimer = 0;
	m_OnFire.constructorFire(m_Vertices, 3, 0.2f);
}
int Slime::EnemyEveryFrame(float deltaTime
	, std::vector<std::vector<Block>>& blocks
	, float* playerTransform)
{
	int RE = 0;
	if (m_PlayerHitTimer < COOLDOWNHIT)
	{
		m_PlayerHitTimer += deltaTime;
	}

	m_Velocity[1] += deltaTime * GRAVITY;
	bool hit[4] = { false, false, false, false };
	float vertices[4];
	vertices[0] = m_Transform[0] + m_Vertices[0];
	vertices[1] = m_Transform[1] + m_Vertices[1];
	vertices[2] = m_Transform[0] + m_Vertices[2];
	vertices[3] = m_Transform[1] + m_Vertices[3];
	DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[2], hit[3], hit[1]);
	if (PlayerInWay(deltaTime, playerTransform, vertices) && m_PlayerHitTimer >= COOLDOWNHIT)
	{
		RE = m_Damage;
		m_PlayerHitTimer = 0;
	}
	AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[3], hit[2], hit[0], hit[1], deltaTime);
	if (hit[3])
	{
		m_Velocity[0] = 0;
	}
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
		int direction[2];
		float distance[2];
		WhereIsPlayer(playerTransform, distance, direction);
		m_JumpTimer = 0;
		m_Velocity[0] = 15 * direction[0];
		m_Velocity[1] = 15;
		m_AnimPhase = 0;
	}
	
	return RE;
}
void Slime::DrawEnemy(Shader& sh
	, float* transform
	, float* scale)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(basicTransform, transform);

	ChangeScale(1, 1, scale);
	sh.SetUniformMat4(animScale, scale);


	ErrorGL(glBindVertexArray(m_DD));
	sh.SetUniform1i(animNumber, m_AnimPhase);
	sh.SetUniform1i(animLeangth, 2);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

}