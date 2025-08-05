#include"Player.h"


#include"Opengl/Texture.h"
#include"Input.h"


Player::Player(unsigned int eob)
	:m_VAO(0), m_VB(0), m_Tex(0),m_FloorHit(false), m_CeilHit(false), m_WallHit(false), m_CoyoteTimer(0), m_JumpTimer(0), m_CanJump(false), m_JumpPower(20), m_Gravity(-60.0f), m_Acceleration(25.0f), m_Friction(30), m_MaxMovementSpeed(10), m_Velocity{0,0}, m_Transform{0, 0}, m_vertices{-1.0f, 3.0f, 0.0f, 0.0f, 1.0f,			1.0f, 3.0f, 0.0f, 1.0f, 1.0f,			1.0f, 0.0f, 0.0f, 1.0f, 0.0f,			-1.0f, 0.0f, 0.0f, 0.0f, 0.0f}
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
	m_CoyoteTimer += deltaTime;


	if (Input::DHold)
	{
		m_Velocity[0] += m_Acceleration * deltaTime;
	}
	else if(m_Velocity[0] > 0)
	{
		float velocity = m_Velocity[0] - m_Friction * deltaTime;
		if (velocity < 0)
		{
			m_Velocity[0] = 0;
		}
		else
		{
			m_Velocity[0] = velocity;
		}
	}
	if (Input::AHold)
	{
		m_Velocity[0] += -m_Acceleration * deltaTime;
	}
	else if (m_Velocity[0] < 0)
	{
		float velocity = m_Velocity[0] + m_Friction * deltaTime;
		if (velocity > 0)
		{
			m_Velocity[0] = 0;
		}
		else
		{
			m_Velocity[0] = velocity;
		}
	}

	if (abs(m_Velocity[0]) > m_MaxMovementSpeed)
		m_Velocity[0] = m_MaxMovementSpeed * (m_Velocity[0]/abs(m_Velocity[0]));

	if (Input::SpacePress && m_CanJump && !m_CeilHit)
	{
		m_Velocity[1] += m_JumpPower;
		m_CanJump = false;
		m_JumpTimer += deltaTime;
	}
	else if(Input::SpaceHold && m_JumpTimer > 0 && m_JumpTimer < 0.25f && !m_CeilHit)
	{
		m_Velocity[1] += ( - m_JumpPower / 2)* deltaTime;
		m_JumpTimer += deltaTime;
	}
	else
	{
		m_Velocity[1] += m_Gravity * deltaTime;
		m_JumpTimer = 0;
	}

	m_FloorHit = false;
	m_CeilHit = false;
	m_WallHit =false;
	float hitboxvertices[4] = { m_vertices[0] + m_Transform[0],m_vertices[1] + m_Transform[1],m_vertices[10] + m_Transform[0],m_vertices[11] + m_Transform[1] };
	
	DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, hitboxvertices, hitbox, m_WallHit, m_WallHit, m_FloorHit, m_CeilHit);
	if (m_FloorHit)
	{
		m_CanJump = true;
		m_CoyoteTimer = 0.0f;
	}
	if (!m_FloorHit && m_CanJump && m_CoyoteTimer >= 0.25f)
	{
		m_CanJump = false;
		m_CoyoteTimer = 0.0f;
	}


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

