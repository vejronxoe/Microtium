#include"Enemy.h"
#include"ItemList.h"
#include"glfw/input.h"
#include"Opengl/Texture.h"
#include"Opengl/DrawData.h"
#include"math/matrix.h"

#define zombieMovement 5
#define COLDDOWNHIT 5

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

Enemy::Enemy(unsigned int typeOfEnemy
	, std::vector<Enemy> enemis
	, unsigned int* EnemisTex
	, unsigned int* EnemisDD1
	, unsigned int* EnemisDD2
	, float x
	, float y
	, unsigned int eob)
{
	int damage[2] = { 
		45
		, 35};
	m_PlayerHitTimer = 0;
	m_TypeOfEnemy = typeOfEnemy;
	Assert(typeOfEnemy < 0 || typeOfEnemy > 1);
	m_Damage = damage[typeOfEnemy];
	m_Transform[0] = x;
	m_Transform[1] = y;
	m_Velocity[0] = 0;
	m_Velocity[1] = 0;
	bool enemyExist = false;
	for(int i = 0; i < enemis.size();i++)
	{
		if (enemis.at(i).m_TypeOfEnemy == typeOfEnemy)
		{
			enemyExist = true;
			break;
		}
	}

	if (!enemyExist)
	{
		switch (typeOfEnemy)
		{
		case enemyZombie:
			EnemisTex[typeOfEnemy] = CreateTextureRGBA("res/textures/blue.png");
			EnemisDD1[typeOfEnemy] = CreateDrawData(eob, -1.5f, 1.5f, 1, -1);
			break;
		}
	}
	m_tex = EnemisTex[typeOfEnemy];
	m_DD[0] = EnemisDD1[typeOfEnemy];
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
	float playerVertices[4] = {playerTransform[0] - 1, playerTransform[1] + 1.5f, playerTransform[0] + 1, playerTransform[1] - 1.5f};
	if (vertices[1] >= playerVertices[3] && vertices[3] <= playerVertices[1] && vertices[2] >= playerVertices[0] && vertices[0] <= playerVertices[2])
	{
		return true;
	}
	return false;
}
int Enemy::EnemyEveryFrame(float deltaTime
	, std::vector<std::vector<Block>> blocks
	, float* playerTransform)
{
	int RE = 0;
	switch (m_TypeOfEnemy)
	{
	case enemyZombie:
	{
		if (m_PlayerHitTimer < COLDDOWNHIT)
		{
			m_PlayerHitTimer += deltaTime;
		}
		int direction[2];
		float distance[2];

		WhereIsPlayer(playerTransform, distance, direction);
		if (abs(distance[0]) < 2 || zombieMovement < m_Velocity[0] * direction[0])
		{
			m_Velocity[0] -= direction[0] * zombieMovement * deltaTime;
				
		}
		else
		{
			m_Velocity[0] += direction[0] * zombieMovement * deltaTime;
		}
		m_Velocity[1] += deltaTime * GRAVITY;
		if (m_Velocity[1] < GRAVITY)
		{
			m_Velocity[1] = GRAVITY;
		}
		bool hit[4] = {false, false, false, false};
		float vertices[4] = {-1.0f, 1.5f, 1.0f, -1.5f};
		vertices[0] += m_Transform[0];
		vertices[1] += m_Transform[1];
		vertices[2] += m_Transform[0];
		vertices[3] += m_Transform[1];
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[2], hit[3], hit[1]);
		if (PlayerInWay(deltaTime, playerTransform,vertices) && m_PlayerHitTimer >= COLDDOWNHIT)
		{
			RE = m_Damage;
		}
		
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[3], hit[2], hit[0], hit[1], deltaTime);

		if ((hit[0] || hit[2] || (Input::SpacePress && abs(distance[1]) < vertices[1] - m_Transform[1] + 1.5f)) && hit[3])
		{
			m_Velocity[1] = 15;
		}
		break;
	}
	case enemySlime:

		break;
	}
	return RE;
}
void Enemy::DrawEnemy(Shader sh
 , float* transform)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(basicTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_tex));
	ErrorGL(glBindVertexArray(m_DD[0]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}