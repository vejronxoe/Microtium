#include"Player.h"


#include"Opengl/Texture.h"
#include"Input.h"


Player::Player(unsigned int eob)
	:m_VAO(0), m_VB(0), m_Tex(0), m_MaxMovementSpeed(15), m_Velocity{0,0}, m_Transform{0, 0}, m_vertices{-1.0f, 3.0f, 0.0f, 0.0f, 1.0f,			1.0f, 3.0f, 0.0f, 1.0f, 1.0f,			1.0f, 0.0f, 0.0f, 1.0f, 0.0f,			-1.0f, 0.0f, 0.0f, 0.0f, 0.0f}
{
	m_Tex = CreateTexture("res/textures/player0.png", true);

	ErrorGL(glGenVertexArrays(1, &m_VAO));

	ErrorGL(glBindVertexArray(m_VAO));
	ErrorGL(glGenBuffers(1, &m_VB));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_VB));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), m_vertices, GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));
	ErrorGL(glBindVertexArray(0));
	
}
void Player::EveryFrame(float deltaTime, std::vector<StaticSquereHitbox>& hitbox)
{
	
	if (Input::DHold)
		m_Velocity[0] += 20.0f * deltaTime;
	if (Input::AHold)
		m_Velocity[0] += -20.0f * deltaTime;

	if (m_Velocity[0] >= m_MaxMovementSpeed)
		m_Velocity[0] = m_MaxMovementSpeed;
	else if(m_Velocity[0] <= -m_MaxMovementSpeed)
		m_Velocity[0] = -m_MaxMovementSpeed;
	if (Input::SpacePress)
	{
		m_Velocity[1] += 20;
	}
	m_Velocity[1] += -20 * deltaTime;
	if (!Input::AHold && !Input::DHold || Input::DHold && Input::AHold)
	{
		float velocity =abs(m_Velocity[0]) - 40 * deltaTime;
		if (velocity < 0)
		{
			m_Velocity[0] = 0;
		}
		else
		{
			m_Velocity[0] = (m_Velocity[0] / (abs(m_Velocity[0]))) * velocity;
		}
	}
	float hitboxvertices[4] = { m_vertices[0] + m_Transform[0],m_vertices[1] + m_Transform[1],m_vertices[10] + m_Transform[0],m_vertices[11] + m_Transform[1] };
	bool wall = false;
	bool floor = false;
	DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, hitboxvertices, hitbox, wall, floor);



	m_Transform[0] += m_Velocity[0] * deltaTime;
	m_Transform[1] += m_Velocity[1] * deltaTime;
}
void Player::DrawPlayer(Shader &Sh)
{
	Sh.Bind();
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Tex));
	ErrorGL(glBindVertexArray(m_VAO));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}

