#include"particles.h"

#include"math/matrix.h"

#define FIRELIFETIME 2
#define FIREVELOCITY 4
#define BOOMLIFETIME 0.9f
#define GRAVITYBOOM 6

void FireParticle::constructorFire(float* vertices
	, int firePerTic
	, float period)
{
	m_Vertices[0] = vertices[0];
	m_Vertices[1] = vertices[1];
	m_Vertices[2] = vertices[2];
	m_Vertices[3] = vertices[3];
	m_Period = period;
	m_FirePerTic = firePerTic;
	
}
bool FireParticle::DrawParticles(Shader& particlesSh
	, float deltaTime
	, bool onFire
	, float* targetTransform
	, float* transform)
{
	
	bool RE = false;
	if (onFire)
	{
		float vertice[4] = { m_Vertices[0] + targetTransform[0], m_Vertices[1] + targetTransform[1], m_Vertices[2] + targetTransform[0], m_Vertices[3] + targetTransform[1] };
		if (m_LifeTimer.size())
		{
			if (m_Period < m_LifeTimer.at(m_LifeTimer.size() - 1))
			{
				for (int i = 0; i < m_FirePerTic; i++)
				{
					m_X.push_back((float)(rand() % (int)((m_Vertices[2] - m_Vertices[0]) * 1000)) / 1000.0f + vertice[0]);
					m_Y.push_back((float)(rand() % (int)((m_Vertices[1] - m_Vertices[3]) * 1000)) / 1000.0f + vertice[3]);
					m_LifeTimer.push_back(0);
				}
				RE = true;
			}
		}
		else
		{
			for (int i = 0; i < m_FirePerTic; i++)
			{
				m_X.push_back((float)(rand() % (int)((m_Vertices[2] - m_Vertices[0]) * 1000)) / 1000.0f + vertice[0]);
				m_Y.push_back((float)(rand() % (int)((m_Vertices[1] - m_Vertices[3]) * 1000)) / 1000.0f + vertice[3]);
				m_LifeTimer.push_back(0);
			}
			RE = true;
		}
	}
	for (int i = 0; i < m_X.size(); i++)
	{
		m_LifeTimer.at(i) += deltaTime;
		if (FIRELIFETIME <= m_LifeTimer.at(i))
		{
			m_LifeTimer.erase(m_LifeTimer.begin() + i);
			m_X.erase(m_X.begin() + i);
			m_Y.erase(m_Y.begin() + i);
			i--;
		}
		else
		{
			m_Y.at(i) += FIREVELOCITY * deltaTime;
			ChangeTransform(m_X.at(i), m_Y.at(i), transform);
			particlesSh.SetUniformMat4(particlesTransform, transform);
			particlesSh.SetUniform4f(particlesSize + particlesColor, 1, m_LifeTimer.at(i)/FIRELIFETIME, 0, 1 - m_LifeTimer.at(i) / FIRELIFETIME);
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}
	}
	return RE;
}
BoomParticle::BoomParticle(float* transform
	, float* color
	, float scale
	, int amount)
{
	m_Color[0] = color[0];
	m_Color[1] = color[1];
	m_Color[2] = color[2];
	m_Color[3] = color[3];
	m_BaseAlfa = color[3];
	m_Scale = scale;
	m_Amount = amount;
	m_LifeTimer = 0;
	for (int i = 0; i < amount; i++)
	{
		m_X.push_back(transform[0]);
		m_Y.push_back(transform[1]);
		m_VelocityY.push_back((float)(rand() % 1000) / 1000.0f * 3);
		m_VelocityX.push_back((float)(rand() % 1000)/1000.0f * 3) ;

	}
}
bool BoomParticle::DrawParticles(Shader& particlesSh
	, float deltaTime
	, float* transform
	, float* scale
	, float* rotation)
{
	m_LifeTimer += deltaTime;
	if (m_LifeTimer >= BOOMLIFETIME)
	{
		return true;
	}
	m_Color[3] -= m_BaseAlfa/BOOMLIFETIME * deltaTime;

	ChangeScale(m_Scale, m_Scale, scale);
	particlesSh.SetUniformMat4(particlesScale, scale);
	particlesSh.SetUniform4f(particlesSize + particlesColor, m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	for (int i = 0 ; i < m_Amount; i++)
	{

		m_VelocityY.at(i) -= GRAVITYBOOM * deltaTime;
		if (m_VelocityY.at(i) > 30)
		{
			m_VelocityY.at(i) = 30;
		}
		m_X.at(i) += m_VelocityX.at(i) * deltaTime;
		m_Y.at(i) += m_VelocityY.at(i) * deltaTime;
		ChangeRotation(-atan2(m_X.at(i), m_Y.at(i)),rotation);
		particlesSh.SetUniformMat4(particlesRotation, rotation);
		ChangeTransform(m_X.at(i), m_Y.at(i), transform);
		particlesSh.SetUniformMat4(particlesTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	return false;

}
