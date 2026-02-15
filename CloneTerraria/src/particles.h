#pragma once
#include<vector>

#include"Opengl/Shader.h"

class Particles
{
public:
	std::vector<float> m_X;
	std::vector<float> m_Y;
	float m_Scale;
	int m_Amount;
	virtual bool DrawParticles(Shader particlesSh
		, float deltaTime
		, float* targetTransform
		, float* transform
		, float* scale
		, float* rotation) = 0;

};

class FireParticles: Particles
{
public:

	FireParticles(float* vertices
		, float Scale
		, int Amount);
	float m_Vertices[4];
	std::vector<float> m_LifeTimer;
	bool DrawParticles(Shader particlesSh
		, float deltaTime
		, float* targetTransform
		, float* transform
		, float* scale
		, float* rotation) override;
};
class BoomParticles : Particles
{
public:
	std::vector<float> m_VelocityX;
	std::vector<float> m_VelocityY;
	float m_Color[4];
	BoomParticles(float* transform
		, float* color
		, float scale
		, int amount);
	bool DrawParticles(Shader particlesSh
		, float deltaTime
		, float* targetTransform
		, float* transform
		, float* scale
		, float* rotation) override;
};