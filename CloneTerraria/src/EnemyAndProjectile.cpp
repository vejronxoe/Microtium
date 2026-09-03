#include"EnemyAndProjectile.h"

#include<cmath>

#include"ItemList.h"
#include"glfw/input.h"
#include"opengl/Texture.h"
#include"opengl/DrawData.h"
#include"math/matrix.h"
#include"math/VectorOperation.h"
#include"glfw/Window.h"
#include"Collision.h"


#define SPAWNCOOLDOWN 10
#define DESPAWNTIME 10
#define ENEMIESMOVEMENT 9
#define COOLDOWNHIT 3
#define JUMPSTRENGHT 17


#define SLIMEABILITIESCOOLDOWN 4
#define SKELETONCOOLDOWN 5
#define ZOMBIEDELAY 0.3f
#define GHOSTCOOLDOWN 5.0f
#define GHOSTFLIGHTDUR 1.5f
#define GHOSTLOOKDUR 0.75f
#define WORMBOOMDELAY 1.75f
#define BIRDCOOLDOWN 4.5f

void GetEnemyVerticesByType(unsigned int typeOfEnemy, float* vertices)
{
	switch (typeOfEnemy)
	{
	case en_Slime:
	case en_SandSlime:
	case en_FrostSlime:
		vertices[0] = -0.9f;
		vertices[1] = 0.8f;
		vertices[2] = 0.9f;
		vertices[3] = -1;
		break;
	case en_Skeleton:
	case en_Zombie:
	case en_Mummy:
		vertices[0] = -0.9f;
		vertices[1] = 1.3f;
		vertices[2] = 0.9f;
		vertices[3] = -1.5f;
		break;
	case en_Imp:
		vertices[0] = - 0.9;
		vertices[1] = 1.25;
		vertices[2] = 0.9;
		vertices[3] = -1.25;
		break;
	case en_Ghost:
	case en_Birds:
		vertices[0] = -1.3f;
		vertices[1] = 1.3f;
		vertices[2] = 1.3f;
		vertices[3] = -1.3f;
	break;
	case en_Worm:
		vertices[0] = -1.5f;
		vertices[1] = 1;
		vertices[2] = 1.5f;
		vertices[3] = -1;
		break;
	
	default:
		assert(false);
		break;
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
			x = std::abs(x) / x;
		}
		else
		{
			x = 0;
		}

		if (y)
		{
			y = std::abs(y) / y;
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

Enemy::Enemy(std::vector<Enemy>& enemies
	, unsigned int type
	, float x
	, float y
	, unsigned int eob)
{
	m_TypeOfEnemy = type;

	if (enemies.size())
	{
		m_ID = enemies.at(enemies.size() - 1).m_ID + 1;
	}
	
	m_PlayerHitTimer = 100;

	int hp[enemySize];
	int damage[enemySize];
	for (int i = 0; i < enemySize;i++)
	{
		hp[i] = 15;
		damage[i] = 45;
	}
	
	hp[en_Zombie] = 35;
	hp[en_Skeleton] = 25;
	hp[en_Mummy] = 35;
	hp[en_Worm] = 65;
	hp[en_Imp] = 55;
	hp[en_Birds] = 55;
	hp[en_ThunderBird] = 180;
	hp[en_BigImp] = 240;
	hp[en_Necromancer] = 60;
	damage[en_Zombie] = 55;
	damage[en_Skeleton] = 60;
	damage[en_Mummy] = 50;
	damage[en_Worm] = 120;
	damage[en_Imp] = 100;
	damage[en_Ghost] = 100;
	damage[en_Birds] = 80;
	damage[en_ThunderBird] = 100;
	damage[en_BigImp] = 20;
	damage[en_Necromancer] = 150;

	if (en_SandSlime == type)
	{
		m_IsBurning = true;
	}
	m_HP = hp[m_TypeOfEnemy];
	m_Damage = damage[m_TypeOfEnemy];
	m_Transform[0] = x;
	m_Transform[1] = y;
	float vertices[4];
	GetEnemyVerticesByType(m_TypeOfEnemy, vertices);
	m_OnFire.constructorFire(vertices, 4, 0.2f);

}

int Enemy::walkingToTarget(float deltaTime
	, std::vector<std::vector<Block>>& blocks
	, float* vertices
	, float* targetPos
	, float* playerPos
	, bool* hit)
{
	bool goDownPlatform = false;
	bool wantJump = false;
	int RE = 0;
	if (targetPos)
	{
		goDownPlatform = targetPos[1] < m_Transform[1];
		int direction[2];
		float distance[2];
		WhereIsPlayer(targetPos, distance, direction);
		m_LookAt = direction[0];
		if ((std::abs(distance[0]) < 2 || ENEMIESMOVEMENT < m_Velocity[0] * direction[0]) && m_Velocity[0])
		{
			int oldVelocity = std::abs(m_Velocity[0]) / m_Velocity[0];
			m_Velocity[0] -= direction[0] * ENEMIESMOVEMENT * deltaTime;
			if (m_Velocity[0])
			{
				if (m_Velocity[0] / std::abs(m_Velocity[0]) != oldVelocity)
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
				if (m_Velocity[0] / std::abs(m_Velocity[0]) != direction[0])
				{
					multi = 3;
				}
			}
			m_Velocity[0] += direction[0] * ENEMIESMOVEMENT * deltaTime * multi;
		}
		
		if (std::abs(distance[0]) < 2.0f && distance[1] > 0)
		{
			wantJump = true;
		}
	}
	else if(m_Velocity[0])
	{
		int dir = m_Velocity[0] / std::abs(m_Velocity[0]);
		m_Velocity[0] = m_Velocity[0] -m_Velocity[0] / std::abs(m_Velocity[0]) * ENEMIESMOVEMENT * 2 * deltaTime;
		if (m_Velocity[0] / std::abs(m_Velocity[0]) != dir)
		{
			m_Velocity[0] = 0;
		}
	}
	m_Velocity[1] += deltaTime * GRAVITY;
	if (m_Velocity[1] < GRAVITY)
	{
		m_Velocity[1] = GRAVITY;
	}
	float relVertices[4] = {};
	GetEnemyVerticesByType(m_TypeOfEnemy,relVertices);
	CharacterHitbox(deltaTime, m_Transform, m_Velocity, relVertices,goDownPlatform, false, blocks, hit);
	if (PlayerInWay(deltaTime, playerPos, vertices) && m_PlayerHitTimer >= COOLDOWNHIT)
	{
		RE = m_Damage;
		m_PlayerHitTimer = 0;
	}
	
	AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit, deltaTime);
	if ((wantJump ||hit[0] || hit[2]) && hit[3])
	{
		m_Velocity[1] = JUMPSTRENGHT;
	}
	return RE;
}
int Enemy::EnemyEveryFrame(float deltaTime
	, std::vector<Projectile>& projectiles
	, std::vector<std::vector<Block>>& blocks
	, float* playerTransform
	, float* playerVelocity)
{
	float oldVelocity[2] = {m_Velocity[0], m_Velocity[1]};
	int RE = 0;
	if (m_PlayerHitTimer < COOLDOWNHIT)
	{
		m_PlayerHitTimer += deltaTime;
	}
	float vertices[4];
	float relVertices[4];
	GetEnemyVerticesByType(m_TypeOfEnemy, relVertices);

	vertices[0] = m_Transform[0] + relVertices[0];
	vertices[1] = m_Transform[1] + relVertices[1];
	vertices[2] = m_Transform[0] + relVertices[2];
	vertices[3] = m_Transform[1] + relVertices[3];
	int direction[2];
	float distance[2];
	WhereIsPlayer(playerTransform, distance, direction);
	switch (m_TypeOfEnemy)
	{
	case en_Zombie:
	case en_Mummy:
	{
	
		m_LookAt = direction[0];
		if (m_AbilityTimer)
		{
			m_AbilityTimer += deltaTime;
		}
		if(m_AbilityTimer > ZOMBIEDELAY)
		{
			
			m_Velocity[1] = JUMPSTRENGHT;
			m_AbilityTimer = 0;
		}
		bool hit[4] = {};
		RE = walkingToTarget(deltaTime, blocks, vertices, playerTransform, playerTransform,hit);
		if (hit[3])
		{
			m_AnimTimer += deltaTime;
		}
		if ((Input::SpacePress && std::abs(distance[1]) < vertices[1] - m_Transform[1] + 2) && hit[3])
		{
			m_AbilityTimer += deltaTime;
		}
		break;
	}		
	case en_Slime:
	case en_SandSlime:
	{
		m_LookAt = direction[0];
		m_Velocity[1] += deltaTime * GRAVITY;
		bool hit[4] = { false, false, false, false };
		m_AnimTimer += deltaTime;

		int behavior = DynamicHitbox(deltaTime, m_Transform, m_Velocity, relVertices, playerTransform[1] < m_Transform[1],false, blocks, hit);
		if (PlayerInWay(deltaTime, playerTransform, vertices) && m_PlayerHitTimer >= COOLDOWNHIT)
		{
			RE = m_Damage;
			m_PlayerHitTimer = 0;
		}
	
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit, deltaTime);
		m_AbilityTimer += deltaTime;
		m_AnimTimer += deltaTime;

		if (m_AbilityTimer > SLIMEABILITIESCOOLDOWN)
		{
			m_AbilityTimer = 0;
			m_Velocity[1] = 20;
			m_Velocity[0] = direction[0] * Clamp((std::abs(distance[0]) - 0.5f), 0, 7.5f);
			hit[3] = false;
		}
		m_Velocity[0] += direction[0] * deltaTime;
		if (hit[3] && behavior != b_Slippery)
		{
			m_Velocity[0] = 0;
		}
		break;
	}
	case en_FrostSlime:
	{
		m_LookAt = direction[0];
		m_Velocity[1] += deltaTime * GRAVITY;
		bool hit[4] = { false, false, false, false };
		m_AnimTimer += deltaTime;
		int behavior = DynamicHitbox(deltaTime, m_Transform, m_Velocity, relVertices, playerTransform[1] < m_Transform[1], false, blocks, hit);
		if (PlayerInWay(deltaTime, playerTransform, vertices) && m_PlayerHitTimer >= COOLDOWNHIT)
		{
			RE = m_Damage;
			m_PlayerHitTimer = 0;
		}

		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit, deltaTime);
		m_AbilityTimer += deltaTime;
		m_AnimTimer += deltaTime;

		if (m_AbilityTimer > SLIMEABILITIESCOOLDOWN)
		{
			float velocity = Clamp(std::abs(distance[0]), 0, 8);
			if (std::abs(distance[0]) < 4)
			{
				velocity *= 2;
			}
			projectiles.emplace_back(p_FrostSpike, m_Transform[0], m_Transform[1], velocity, 20, m_Damage);
			projectiles.emplace_back(p_FrostSpike, m_Transform[0], m_Transform[1], -velocity, 20, m_Damage);
			projectiles.emplace_back(p_FrostSpike, m_Transform[0], m_Transform[1], velocity/2.0f,20, m_Damage);
			projectiles.emplace_back(p_FrostSpike, m_Transform[0], m_Transform[1], -velocity /2.0f, 20, m_Damage);
			projectiles.emplace_back(p_FrostSpike, m_Transform[0], m_Transform[1], 0, 20, m_Damage);
			m_AbilityTimer = 0;
		}
		if (hit[3] && behavior != b_Slippery)
		{
			m_Velocity[0] = 0;
		}
		break;
	}
	case en_Skeleton:
	{
		m_LookAt = direction[0];
		m_AbilityTimer += deltaTime;
		float dis = Pyt2D(distance);
		m_Velocity[1] += deltaTime * GRAVITY;
		bool hit[4] = {};
	
		if (dis > 10 )
		{
			float velocity[2] = { 0,4 };
			velocity[1] += distance[1];
			if (velocity[1] * velocity[1] <= 625)
			{
				velocity[0] = sqrt(velocity[1] * velocity[1] - 625);
			}
			else
			{
				velocity[0] = 0;
				distance[0] = 1;
			}
			if (velocity[0] < std::abs(distance[0]))
			{
				RE = walkingToTarget(deltaTime, blocks, relVertices, playerTransform, playerTransform, hit);
			}
			else
			{
				RE = walkingToTarget(deltaTime, blocks, relVertices, NULL, playerTransform, hit);
				if (m_AbilityTimer >= SKELETONCOOLDOWN)
				{
					velocity[0] = std::abs(distance[0]) * m_LookAt;
					projectiles.emplace_back(p_BoneArrow, HANDOFFSETX * m_LookAt + m_Transform[0], HANDOFFSETY + m_Transform[1], velocity[0], velocity[1], m_Damage);
					m_AbilityTimer = 0;
				}
			}
		}
		else
		{
			if (m_AbilityTimer >= SKELETONCOOLDOWN)
			{
				float velocity[2] = { playerTransform[0] - HANDOFFSETX * m_LookAt - m_Transform[0], playerTransform[1] - HANDOFFSETY - m_Transform[1] };
				NormalizeVector(velocity);
				projectiles.emplace_back(p_BoneArrow, HANDOFFSETX* m_LookAt + m_Transform[0], HANDOFFSETY + m_Transform[1], velocity[0] * 25, velocity[1] * 25, m_Damage);
				m_AbilityTimer = 0;
			}
			RE = walkingToTarget(deltaTime, blocks, relVertices, NULL, playerTransform, hit);
		}
		if (hit[3] && m_Velocity[0])
		{
			m_AnimTimer += deltaTime;
		}
		RE = 0;
		break;
	}
	case en_Imp:
	{
		
		m_LookAt = direction[0];
		m_AbilityTimer += deltaTime;
		float dis = Pyt2D(distance);
		m_Velocity[1] += deltaTime * GRAVITY;
		bool hit[4] = {};

		if (dis > 7)
		{
			if (m_AbilityTimer >= SKELETONCOOLDOWN)
			{
				bool walkCloser = true;
				for(int i = 1; i < 6;i++)
				{
					float dir[2] = {playerTransform[0] + playerVelocity[0] * i- (HANDOFFSETX + 0.35f)* m_LookAt - m_Transform[0] ,  playerTransform[1] + playerVelocity[1] * i -HANDOFFSETY - 0.45f - m_Transform[1]};
					if(Pyt2D(dir) <= 20 * i)
					{
						NormalizeVector(dir);
						dir[0] *= 20;
						dir[1] *= 20;
						projectiles.emplace_back(p_FireBall, (HANDOFFSETX + 0.35f)* m_LookAt + m_Transform[0], HANDOFFSETY - 0.45f + m_Transform[1], dir[0], dir[1], m_Damage);
						m_AbilityTimer = 0;
						break;
					}
				}
				if(walkCloser)
				{
					RE = walkingToTarget(deltaTime, blocks, relVertices,  playerTransform, playerTransform, hit);
				}
				else
				{
					RE = walkingToTarget(deltaTime, blocks, relVertices,  NULL, playerTransform, hit);
				}
			}
			else if (dis > 20)
			{
				RE = walkingToTarget(deltaTime, blocks, relVertices,  playerTransform, playerTransform, hit);
			}
			else
			{
				RE = walkingToTarget(deltaTime, blocks, relVertices,  NULL, playerTransform, hit);
			}
			
		}
		else
		{
			NormalizeVector(distance);
			float target[2] = {m_Transform[0] - distance[0]*10,m_Transform[1] - distance[1] * 10 };
			RE = walkingToTarget(deltaTime, blocks, relVertices, target, playerTransform, hit);
		}
		if (hit[3] && m_Velocity[0])
		{
			m_AnimTimer += deltaTime;
		}
		m_LookAt = direction[0];
		RE = RE / 2;
		break;
	}
	case en_Ghost:
	{
		m_LookAt = direction[0];
		float dist = Pyt2D(distance);
		if(dist >1)
		{
			NormalizeVector(distance);
			m_Velocity[0] = distance[0] * 4;
			m_Velocity[1] = distance[1] * 4;
			m_AbilityTimer = dist;
			m_AnimTimer += deltaTime;
		}
		if (PlayerInWay(deltaTime, playerTransform, vertices) && m_PlayerHitTimer >= COOLDOWNHIT)
		{
			RE = m_Damage;
			m_PlayerHitTimer = 0;
		}
		bool hit[4] = {};
		AddVelocityToTransform(vertices, m_Transform, m_Velocity,hit,deltaTime);

		break;
	}
	case en_Worm:
	{
		m_AnimTimer += deltaTime;
		bool hit[4] = {};
		if(!m_AbilityTimer)
		{
			
			m_LookAt = direction[0];
			if(Pyt2D(distance) < 5)
			{
				m_AbilityTimer += deltaTime;
			}
			walkingToTarget(deltaTime, blocks, relVertices, playerTransform, playerTransform, hit);
		}
		else
		{
			m_AbilityTimer += deltaTime;
			if( m_AbilityTimer > WORMBOOMDELAY)
			{
				projectiles.emplace_back(p_Gas, m_Transform[0], m_Transform[1],7*m_LookAt,2, m_Damage);
				projectiles.emplace_back(p_Gas, m_Transform[0], m_Transform[1],5*m_LookAt,4, m_Damage);
				projectiles.emplace_back(p_Gas, m_Transform[0], m_Transform[1],3*m_LookAt,6, m_Damage);
				
				RE = -1;
			}
			walkingToTarget(deltaTime, blocks, relVertices, NULL, playerTransform, hit);
		}
	}
	case en_Birds:
	{
		m_AbilityTimer += deltaTime;
		if(playerTransform[1] > 100)
		{
			m_LookAt = direction[0];
			float dist = Pyt2D(distance);
			if(dist >15)
			{
				NormalizeVector(distance);
				m_Velocity[0] += distance[0] * 8;
				m_Velocity[1] += distance[1] * 8;
			}
			else if(dist < 5)
			{
				NormalizeVector(distance);
				m_Velocity[0] += -distance[0] * 8;
				m_Velocity[1] += -distance[1] * 8;
			}
			else
			{
				NormalizeVector(distance);
				for(int i = 0; i < 2;i++)
				{
					int dir = 0;
					if(m_Velocity[i])
					{
						dir = m_Velocity[i]/std::abs(m_Velocity[i]);
						m_Velocity[i] += -distance[i] * deltaTime * 8;
					
						if(m_Velocity[i])
						{
							if(dir == m_Velocity[i]/std::abs(m_Velocity[i]))
							{
								m_Velocity[i] = 0;
							}
						}
					}
				}

			}
			m_Velocity[0] = Clamp(m_Velocity[0],-4,4);
			m_Velocity[1] = Clamp(m_Velocity[1],-4,4);
		}
		else
		{
			m_Velocity[1] = 8;
			if(m_Transform[1] > playerTransform[1] + Window::halfHeightOfGameTransform)
			{
				RE =-1;
			}
		}
		bool hit[4] = {};
		m_AnimTimer += deltaTime;
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit,deltaTime);
		if(m_AbilityTimer > BIRDCOOLDOWN)
		{
			float angle = atan2(distance[0], distance[1]);
			projectiles.emplace_back(p_ArcaneBall, m_Transform[0], m_Transform[1], sin(angle) * 15,cos(-angle) * 15 , m_Damage);
			angle -= 5.0f/36.0f	* PI;
			projectiles.emplace_back(p_ArcaneBall, m_Transform[0], m_Transform[1], sin(angle) * 15,cos(-angle) * 15 , m_Damage);
			angle -= 5.0f/36.0f	* PI;
			projectiles.emplace_back(p_ArcaneBall, m_Transform[0], m_Transform[1], sin(angle) * 15,cos(-angle) * 15 , m_Damage);
			angle += 5.0f/36.0f * PI * 3;
			projectiles.emplace_back(p_ArcaneBall, m_Transform[0], m_Transform[1], sin(angle) * 15,cos(-angle) * 15 , m_Damage);
			angle += 5.0f/36.0f	* PI;
			projectiles.emplace_back(p_ArcaneBall, m_Transform[0], m_Transform[1], sin(angle) * 15,cos(-angle) * 15 , m_Damage);
			m_AbilityTimer = 0;
		}
		break;
	}
	}
	return RE;
}


uint8_t animDraw(Shader& animSh
	, float& timer
	, int* order
	, int orderSize
	, float delayBetweenFrames)
{
	int animPhase = -1;
	for (int i = 0; i < orderSize;i++)
	{
		if (timer < (i+1) * delayBetweenFrames)
		{
			animPhase = order[i];
			break;
		}
	}
	if (animPhase == -1)
	{
		timer = 0;
		animPhase = order[0];
	}
	animSh.SetUniform1i(animNumber, animPhase);
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	return animPhase;
}


void Enemy::DrawEnemy(Shader& animSh
	, Shader& handSh
	, Shader& proAnimSh
	, unsigned int* texs
	, unsigned int* DDs
	, unsigned int skeletonHandTex
	, unsigned int skeletonHandDD
	, unsigned int impHandTex
	, unsigned int impHandDD
	, float* playerPos
	, float* transform
	, float* scale
	, float* rotation)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	animSh.SetUniformMat4(animTransform, transform);
	ChangeScale(m_LookAt, 1, scale);
	animSh.SetUniformMat4(animScale, scale);
	ErrorGL(glBindVertexArray(DDs[m_TypeOfEnemy]));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, texs[m_TypeOfEnemy]));
	switch (m_TypeOfEnemy)
	{
	case en_Slime: 
	case en_FrostSlime:
	case en_SandSlime:
	{
		animSh.SetUniform1i(animLeangth, 2);
		if (m_AnimTimer > -0.5f * m_AbilityTimer + 2)
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
		animSh.SetUniform1i(animNumber, m_AnimPhase);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		break;
	}
	case en_Zombie:
	case en_Mummy:
	{
		int animOrder[8] = {0, 3, 1, 3, 0, 4, 2, 4};
		animSh.SetUniform1i(animLeangth, 5);
		animDraw(animSh,m_AnimTimer,animOrder , 8,0.8f/std::abs(m_Velocity[0]));
		break;
	}
	case en_Imp:
	case en_Skeleton:
	{

		float distance[2] = {playerPos[0] - m_Transform[0] - HANDOFFSETX*m_LookAt,playerPos[1] - m_Transform[1] - HANDOFFSETY};
		int animOrder[8] = { 0, 3, 1, 3, 0, 4, 2, 4 };
		animSh.SetUniform1i(animLeangth, 5);
		float animOffset = 0;
		if (animDraw(animSh, m_AnimTimer, animOrder, 8, 0.8f / std::abs(m_Velocity[0])) == 0)
		{
			animOffset = 0.1f;
			ChangeTransform(m_Transform[0], m_Transform[1] + 0.1f, transform);
			animSh.SetUniformMat4(animTransform, transform);
		}
		handSh.Bind();
		handSh.SetUniformMat4(handScale, scale);
		ChangeRotation(atan2f(distance[0],distance[1])/PI *180 * - m_LookAt, rotation);
		handSh.SetUniformMat4(handRotation, rotation);
		if (m_TypeOfEnemy == en_Skeleton)
		{
			handSh.SetUniformMat4(handTransform, transform);
			ErrorGL(glBindVertexArray(skeletonHandDD));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, skeletonHandTex));
		}
		else
		{
			
			ChangeTransform(m_Transform[0] + (0.35f) * m_LookAt, m_Transform[1] + animOffset - (0.45f), transform);
			handSh.SetUniformMat4(handTransform, transform);
			ErrorGL(glBindVertexArray(impHandDD));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, impHandTex));
		}
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		animSh.Bind();
		break;
	}
	case en_Ghost:
	{
		proAnimSh.Bind();
		ChangeTransform(m_Transform[0], m_Transform[1], transform);
		animSh.SetUniformMat4(proAnimTransform, transform);
		proAnimSh.SetUniform1i(proAnimLeangth, 2);
		if (m_AnimTimer > m_AbilityTimer/10)
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
		double angle = atan2(m_Velocity[0], m_Velocity[1]) * 180.0 / PI;
		ChangeRotation(-std::abs(angle), rotation);
		proAnimSh.SetUniformMat4(proAnimRotation, rotation);
		if (angle)
		{
			ChangeScale(angle / std::abs(angle), 1, scale);
		}
		else
		{
			ChangeScale(1, 1, scale);
		}
		proAnimSh.SetUniformMat4(proAnimScale, scale);
		proAnimSh.SetUniform1i(proAnimNumber, m_AnimPhase);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		animSh.Bind();
	break;
	}
	case en_Worm:
	{
		animSh.SetUniform1i(animLeangth, 3);
		if(!m_AbilityTimer)
		{
			int animOrder[2] = {0, 1};
			animDraw(animSh,m_AnimTimer,animOrder , 2,0.5f);
		}
		else
		{
			animSh.SetUniform1i(animNumber, 2);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
		break;
	}
	case en_Birds:
	{
		animSh.SetUniform1i(animLeangth, 2);
		int animOrder[2] = {0, 1};
		animDraw(animSh,m_AnimTimer,animOrder , 2,0.2f);
	
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
	int dimensions[2] = { static_cast<int>(enemyVertices[2] - enemyVertices[0]), static_cast<int>(enemyVertices[1] - enemyVertices[3]) };
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
					spawntransform[0] = i -enemyVertices[0];
					spawntransform[1] = j - enemyVertices[3];
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
	if (spawnTimer > SPAWNCOOLDOWN)
	{
		if (enemies.size() < 8)
		{
			int types[3][4] = {
				{en_Imp,en_Skeleton,	en_FrostSlime, en_Birds }
			, {en_Worm,en_Zombie , en_Slime,en_Birds }
			, {en_Ghost,en_Mummy, en_SandSlime,en_Birds }
			};
			int index[2] = {};
			int type = en_Slime;
			if (cameraTransform[1] < -160)
			{
				index[1] = 0;
			}
			else if (cameraTransform[1] < 0)
			{
				index[1] = 1;
			}
			else if (cameraTransform[1] < 100)
			{
				index[1] = 2;
			}
			else
			{
				index[1] = 3;
			}
			if (cameraTransform[0] < 360)
			{
				index[0] = 0;
			}
			else if (cameraTransform[0] < 720)
			{
				index[0] = 1;
			}
			else
			{
				index[0] = 2;
			}
			type = types[index[0]][index[1]];
			float transform[2];
			if (getLocationForEnemySpawn(type, cameraTransform, transform, blocks))
			{
				enemies.emplace_back(enemies, type, transform[0], transform[1], eob);
			}
			else if (type == en_Ghost || type == en_Birds)
			{
				index[0] = (rand() % 2) * 2;
				index[1] = (rand() % 2) * 2 + 1;
				float cameraVertices[4] = {transform[0] - Window::halfWidthOfGameTransform - 2,transform[1] + Window::halfHeightOfGameTransform+2,transform[0] + Window::halfWidthOfGameTransform+2, transform[1] - Window::halfHeightOfGameTransform-2 };
				enemies.emplace_back(enemies, type, cameraVertices[index[0]], cameraVertices[index[1]], eob);
			}

		}
		
		spawnTimer = 0;
	}
	
	spawnTimer += deltaTime;
}

void Enemy::WhereIsPlayer(float* playerTransform
	, float* distance
	, int* direction)
{
	distance[0] = playerTransform[0] - m_Transform[0];
	distance[1] = playerTransform[1] - m_Transform[1];
	if (distance[0])
	{
		direction[0] = distance[0] / std::abs(distance[0]);
	}
	else
	{
		direction[0] = -1;
	}
	if (distance[1])
	{
		direction[1] = distance[1] / std::abs(distance[1]);
	}
	else
	{
		direction[1] = -1;
	}
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
		vertices[1] = enemyVertices[1] + m_Velocity[1] * deltatime;
	}
	else
	{
		vertices[3] = enemyVertices[3] + m_Velocity[1] * deltatime;
		vertices[1] = enemyVertices[1];
	}
	float playerVertices[4] = { playerTransform[0] - 1, playerTransform[1] + 1.5f, playerTransform[0] + 1, playerTransform[1] - 1.5f };
	if (vertices[1] >= playerVertices[3] && vertices[3] <= playerVertices[1] && vertices[2] >= playerVertices[0] && vertices[0] <= playerVertices[2])
	{
		return true;
	}
	return false;
}

Projectile::Projectile(unsigned char projectileType
	, float x
	, float y
	, float velocityX
	, float velocityY
	, int damage)
{
	m_Transform[0] = x;
	m_Transform[1] = y;
	m_Velocity[0] = velocityX;
	m_Velocity[1] = velocityY;
	m_ProjectileType = projectileType;
	m_Damage = damage;
	switch (projectileType)
	{
	case p_BouncingBullet:
	case p_BouncingCannonBall:
	case p_BouncingArrow:
		m_Bouncing = 5;
		break;
	case p_Gas:
		m_Bouncing = 3;
		break;
	default:
		m_Bouncing = 0;
		break;
	}
}
bool HitEnemy(float deltaTime
	, int damage
	, float* oldVelocity
	, float* velocity
	, float* proVertices
	, float* transform
	, std::vector<BoomParticle>& particles
	, std::vector<Enemy>& enemies
	, bool burning)
{
	float vertices[4];
	if (velocity[0] > 0)
	{
		vertices[0] = proVertices[0];
		vertices[2] = proVertices[2] + oldVelocity[0] * deltaTime + 0.5f * (velocity[0] - oldVelocity[0]) * deltaTime;
	}
	else
	{
		vertices[0] = proVertices[0] + oldVelocity[0] * deltaTime + 0.5f * (velocity[0] - oldVelocity[0]) * deltaTime;;
		vertices[2] = proVertices[2];
	}
	if (velocity[1] > 0)
	{
		vertices[3] = proVertices[3];
		vertices[1] = proVertices[1] + oldVelocity[1] * deltaTime + 0.5f * (velocity[1] - oldVelocity[1]) * deltaTime;
	}
	else
	{
		vertices[3] = proVertices[3] + oldVelocity[1] * deltaTime + 0.5f * (velocity[1] - oldVelocity[1]) * deltaTime;;
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
		if (DoTheyIntersect(enemyVertices,vertices))
		{
			if (burning)
			{
				enemies.at(i).m_IsBurning = true;
			}
			if (enemies.at(i).DamageEnemy(damage, transform))
			{
				enemies.erase(enemies.begin() + i);
			}
			float c[4] = { 1,0,0,0.8f };
			particles.emplace_back(transform, c, 1, 8);
			return true;

		}
	}
	return false;
}
bool HitEnemies(float deltaTime
	, int damage
	, float* oldVelocity
	, float* velocity
	, float* proVertices
	, float* transform
	, std::vector<BoomParticle>& particles
	, std::vector<Enemy>& enemies
	, std::vector<uint8_t>& hitEnemies)
{
	float vertices[4];
	if (velocity[0] > 0)
	{
		vertices[0] = proVertices[0];
		vertices[2] = proVertices[2] + oldVelocity[0] * deltaTime + 0.5f * (velocity[0] - oldVelocity[0]) * deltaTime;
	}
	else
	{
		vertices[0] = proVertices[0] + oldVelocity[0] * deltaTime + 0.5f * (velocity[0] - oldVelocity[0]) * deltaTime;;
		vertices[2] = proVertices[2];
	}
	if (velocity[1] > 0)
	{
		vertices[3] = proVertices[3];
		vertices[1] = proVertices[1] + oldVelocity[1] * deltaTime + 0.5f * (velocity[1] - oldVelocity[1]) * deltaTime;
	}
	else
	{
		vertices[3] = proVertices[3] + oldVelocity[1] * deltaTime + 0.5f * (velocity[1] - oldVelocity[1]) * deltaTime;;
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
		if (DoTheyIntersect(enemyVertices,vertices))
		{
			bool wasHit = false;
			for (int j = 0; j < hitEnemies.size(); j++)
			{
				if (hitEnemies.at(j) == enemies.at(i).m_ID)
				{
					wasHit = true;
				}
			}
			if (!wasHit)
			{
				if (enemies.at(i).DamageEnemy(damage, transform))
				{
					enemies.erase(enemies.begin() + i);
					i--;
				}
				else
				{
					hitEnemies.push_back(enemies.at(i).m_ID);
				}
				float c[4] = { 1,0,0,0.8f };
				particles.emplace_back(transform, c, 1, 8);
			}
		}
	}
	return false;
}
bool HitPlayer(float deltaTime
	, int damage
	, float* oldVelocity
	, float* velocity
	, float* proVertices
	, float* playerPos
	, float* transform
	, int& playerDamage
	, std::vector<BoomParticle>& particles)
{
	float vertices[4];
	if (velocity[0] > 0)
	{
		vertices[0] = proVertices[0];
		vertices[2] = proVertices[2] + oldVelocity[0] * deltaTime + 0.5f * (velocity[0] - oldVelocity[0]) * deltaTime;
	}
	else
	{
		vertices[0] = proVertices[0] + oldVelocity[0] * deltaTime + 0.5f * (velocity[0] - oldVelocity[0]) * deltaTime;;
		vertices[2] = proVertices[2];
	}
	if (velocity[1] > 0)
	{
		vertices[3] = proVertices[3];
		vertices[1] = proVertices[1] + oldVelocity[1] * deltaTime + 0.5f * (velocity[1] - oldVelocity[1]) * deltaTime;
	}
	else
	{
		vertices[3] = proVertices[3] + oldVelocity[1] * deltaTime + 0.5f * (velocity[1] - oldVelocity[1]) * deltaTime;;
		vertices[1] = proVertices[1];
	}

	
	float playerVertices[4];
	playerVertices[0] = playerPos[0] - 0.8;
	playerVertices[1] = playerPos[1] + 1.3;
	playerVertices[2] = playerPos[0] + 0.8;
	playerVertices[3] = playerPos[1] - 1.5;
	if (vertices[1] >= playerVertices[3] && vertices[3] <= playerVertices[1] && vertices[2] >= playerVertices[0] && vertices[0] <= playerVertices[2])
	{
			playerDamage += damage;
			float c[4] = { 1,0,0,0.8f };
			particles.emplace_back(transform, c, 1, 8);
			return true;
	}
	
	return false;
}

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
	, unsigned int* blockTextures)
{
	int playerDamage = 0;
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
		case p_FireBall:
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
		case p_Gas:
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
		case p_FrostSpike:
			c[i][0] = 0.357;
			c[i][1] = 0.792;
			c[i][2] = 0.929;
			c[i][3] = 1;
			break;
		case p_BoneArrow:
			c[i][0] = 1;
			c[i][1] = 1;
			c[i][2] = 1;
			c[i][3] = 1;
			break;
		case p_ArcaneBall:
			c[i][0] = 0.2;
			c[i][1] = 0.2;
			c[i][2] = 1;
			c[i][3] = 1;
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
		case p_Gas:
		case p_BasicArrow:
		case p_PierceArrow:
		case p_BouncingArrow:
		case p_FireArrow:
		case p_BoneArrow:
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
		case p_FireBall:
		case p_ArcaneBall:
			gravity[i] = 0;
			halfSize[i] = 0.3f;
			break;
		case p_FrostSpike:
			gravity[i] = 40;
			halfSize[i] = 0.35f;

			break;
		}
	}

	float vertices[4];
	float relVertices[4];
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
		relVertices[0] =  - halfSize[projectiles.at(i).m_ProjectileType]; relVertices[1] = + halfSize[projectiles.at(i).m_ProjectileType];
		relVertices[2] =  + halfSize[projectiles.at(i).m_ProjectileType]; relVertices[3] = - halfSize[projectiles.at(i).m_ProjectileType];
	
		if (projectiles.at(i).m_ProjectileType != p_FireBall && projectiles.at(i).m_ProjectileType != p_ArcaneBall)
		{
			DynamicHitbox(deltaTime, projectiles.at(i).m_Transform, projectiles.at(i).m_Velocity, relVertices, true, true, blocks, blockHit);
		}
		switch (projectiles.at(i).m_ProjectileType)
		{
		case p_BasicArrow:
		case p_BasicBullet:
		case p_BasicCannonBall:
		{
			if (HitEnemy(deltaTime, projectiles.at(i).m_Damage, oldVelocity, projectiles.at(i).m_Velocity,vertices,projectiles.at(i).m_Transform, particles, enemies, false))
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
			if (HitEnemy(deltaTime, projectiles.at(i).m_Damage, oldVelocity, projectiles.at(i).m_Velocity, vertices,projectiles.at(i).m_Transform, particles, enemies, false))
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
			if (HitEnemies(deltaTime, projectiles.at(i).m_Damage, oldVelocity, projectiles.at(i).m_Velocity, vertices,projectiles.at(i).m_Transform, particles, enemies, projectiles.at(i).m_HitEnemies))
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
			if (HitEnemy(deltaTime, projectiles.at(i).m_Damage, oldVelocity, projectiles.at(i).m_Velocity, vertices, projectiles.at(i).m_Transform, particles, enemies, true))
			{
				destroy = true;
			}

			if (blockHit[0] == true || blockHit[1] == true || blockHit[2] == true || blockHit[3] == true)
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
		case p_FrostSpike:
		case p_BoneArrow:
		{
			if (HitPlayer(deltaTime, projectiles.at(i).m_Damage, oldVelocity, projectiles.at(i).m_Velocity, vertices, playerPos, projectiles.at(i).m_Transform, playerDamage, particles))
			{
				destroy = true;
				break;
			}
			destroy = (blockHit[0] == true || blockHit[1] == true || blockHit[2] == true || blockHit[3] == true);
			break;
		}
		case p_FireBall:
		case p_ArcaneBall:
		{
			if (HitPlayer(deltaTime, projectiles.at(i).m_Damage, oldVelocity, projectiles.at(i).m_Velocity, vertices, playerPos, projectiles.at(i).m_Transform, playerDamage, particles))
			{
				destroy = true;
				break;
			}
			break;
		}
		case p_Gas:
		{
			if (HitPlayer(deltaTime, projectiles.at(i).m_Damage, oldVelocity, projectiles.at(i).m_Velocity, vertices, playerPos, projectiles.at(i).m_Transform, playerDamage, particles))
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
		}
		AddVelocityToTransform(vertices, projectiles.at(i).m_Transform, projectiles.at(i).m_Velocity, wallHit, deltaTime);

		if (wallHit[0] == true || wallHit[1] == true || wallHit[2] == true || wallHit[3] == true)
		{
			destroy = true;
		}

		if (destroy)
		{
			particles.emplace_back(projectiles.at(i).m_Transform, c[projectiles.at(i).m_ProjectileType], 1, 15);
			projectiles.at(i) = projectiles.at(projectiles.size() - 1);
			projectiles.pop_back();
			i--;
		}
		

	}
	return playerDamage;
}
void Projectile::Draw(Shader& sh
	, unsigned int* DDs
	, unsigned int* texs
	, float* transform
	, float* scale
	, float* rotation)
{

	double angle = atan2(m_Velocity[0], m_Velocity[1]) * 180.0 / PI;
	ChangeRotation(-std::abs(angle), rotation);
	sh.SetUniformMat4(advancedRotation, rotation);
	if (angle)
	{
		ChangeScale(angle / std::abs(angle), 1, scale);
	}
	else
	{
		ChangeScale(1, 1, scale);
	}
	sh.SetUniformMat4(advancedScale, scale);
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(advancedTransform, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, texs[m_ProjectileType]));
	ErrorGL(glBindVertexArray(DDs[m_ProjectileType]));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}
void SandEveryFrame(std::vector<int>& isSandOnX
	, std::vector<Projectile>& projectiles
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<uint8_t>>& Walls
	, std::vector<int>& chunksToRebuild)
{
	for (int i = 0; i < isSandOnX.size(); i++)
	{
		int x = isSandOnX.at(i);
		for (int j = Blocks::yMin + 1; j < Blocks::yMax; j++)
		{
			if (blocks.at(x).at(j - Blocks::yMin).m_Behavior == b_Sand && blocks.at(x).at(j - 1 - Blocks::yMin).m_Behavior == b_Air)
			{
				int sizeBefore = isSandOnX.size();
				projectiles.emplace_back(p_Sand, x, j, 0, -5, 5);
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

