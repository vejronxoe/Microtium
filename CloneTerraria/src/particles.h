#pragma once
#include<vector>

#include"Opengl/Shader.h"


class FireParticle
{
public:
	
	void constructorFire(float* vertices
		, int firePerTic
		, float period);		
	std::vector<float> m_X;
	std::vector<float> m_Y;
	int m_FirePerTic;
	float m_Period;
	float m_Vertices[4];
	std::vector<float> m_LifeTimer;
	bool DrawParticles(Shader& particlesSh
		, float deltaTime
		, bool onFire
		, float* targetTransform
		, float* transform);
};
class BoomParticle
{
public:
	std::vector<float> m_X;
	std::vector<float> m_Y;
	int m_Amount;
	std::vector<float> m_VelocityX;
	std::vector<float> m_VelocityY;
	float m_Color[4];
	float m_BaseAlfa;
	float m_Scale;
	float m_LifeTimer;
	BoomParticle(float* transform
		, float* color
		, float scale
		, int amount);
	bool DrawParticles(Shader& particlesSh
		, float deltaTime
		, float* transform
		, float* scale
		, float* rotation);
};