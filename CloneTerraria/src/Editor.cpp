#include"Editor.h"
#include "glfw/input.h"




void Editor::Update(float deltaTime)
{ 
	float oldVelocity[2] = { m_Velocity[0], m_Velocity[1]};
	if (Input::AHold)
	{
		m_Velocity[0] -= deltaTime * 100;
	}

	if (Input::SHold)
	{
		m_Velocity[1] -= deltaTime * 100;

	}

	if (Input::DHold)
	{
		m_Velocity[0] += deltaTime * 100;

	}

	if (Input::WHold)
	{
		m_Velocity[1] += deltaTime * 100;

	}

	if (!(Input::WHold || Input::SHold))
	{
		m_Velocity[1] = 0;
		oldVelocity[1] = 0;
	}
	if (!(Input::AHold || Input::DHold))
	{
		m_Velocity[0] = 0;
		oldVelocity[0] = 0;
	}
	m_Transform[1] += oldVelocity[1] * deltaTime + 0.5f * (m_Velocity[1] - oldVelocity[1]) * deltaTime;
	m_Transform[0] += oldVelocity[0] * deltaTime + 0.5f * (m_Velocity[0] - oldVelocity[0]) * deltaTime;


}
