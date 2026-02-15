#include"particles.h"

#include"math/matrix.h"

#define FIRELIFETIME 2
#define BOOMLIFETIME 1
#define GRAVITYBOOM 5

FireParticles::FireParticles(float* vertices
	, float Scale
	, int amount)
{
	m_Vertices[0] = vertices[0];
	m_Vertices[1] = vertices[1];
	m_Vertices[2] = vertices[2];
	m_Vertices[3] = vertices[3];
	m_Scale = Scale;
	m_Amount = amount;
	
}
bool FireParticles::DrawParticles(Shader particlesSh
	, float deltaTime
	, float* targetTransform
	, float* transform
	, float* scale
	, float* rotation)
{
	float vertice[4] = {m_Vertices[0] + targetTransform[0], m_Vertices[1] + targetTransform[1], m_Vertices[2] + targetTransform[0], m_Vertices[3] + targetTransform[1] };
	for (int i = m_X.size(); i < m_Amount; i++)
	{
	}
	//particlesSh.SetUniform4f(particlesSize, );
	return false;
}
BoomParticles::BoomParticles(float* transform
	, float* color
	, float scale
	, int amount)
{
	m_Color[0] = color[0];
	m_Color[1] = color[1];
	m_Color[2] = color[2];
	m_Color[3] = color[3];
	m_Scale = scale;
	m_Amount = amount;
	for (int i = 0; i < amount; i++)
	{
		m_X.push_back(transform[0]);
		m_Y.push_back(transform[1]);
		m_VelocityY.push_back((float)(rand() % 1000) / 1000.0f);
		m_VelocityX.push_back((float)(rand() % 1000)/1000.0f);

	}
}
bool BoomParticles::DrawParticles(Shader particlesSh
	, float deltaTime
	, float* targetTransform
	, float* transform
	, float* scale
	, float* rotation)
{
	ChangeScale(m_Scale, m_Scale, scale);
	particlesSh.SetUniformMat4(particlesScale, scale);
	particlesSh.SetUniform4f(particlesSize + particlesColor, m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
	for (int i = 0 ; i < m_Amount; i++)
	{
		m_X.at(i) -= 5 * deltaTime;
		ChangeRotation(-atan2(m_X.at(i), m_Y.at(i)),rotation);
		particlesSh.SetUniformMat4(particlesRotation, rotation);
		ChangeTransform(m_X.at(i), m_Y.at(i), transform);
		particlesSh.SetUniformMat4(particlesTransform, transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
	}
	return false;

}
