#include "projectile.h"

#include "Collision.h"
#include "itemList.h"
#include "math/matrix.h"


Projectile::Projectile(unsigned char projectileType
	, float x
	, float y
	, float velocityX
	, float velocityY
	, unsigned int projectileTexture)
	:m_Transform{ x, y }, m_Velocity{ velocityX, velocityY}, m_ProjectileType{projectileType}, m_Texture{ projectileTexture}
{}
bool Projectile::EveryFrame(float deltaTime
	, std::vector<std::vector<Block>>& blocks
	, std::vector<std::vector<wall>>& walls
	, std::vector<bool>& isSandOnX
	, unsigned int* blockTextures)
{
	switch (m_ProjectileType)
	{
	case p_Sand:
		float vertices[4];
		vertices[0] = m_Transform[0] - 0.5f; vertices[1] = m_Transform[1] + 0.5f;
		vertices[0] = m_Transform[0] + 0.5f; vertices[1] = m_Transform[1] - 0.5f;
		bool hit[2];
		DynamicSquereHitbox(deltaTime, m_Transform, m_Velocity, vertices, blocks, hit[0], hit[0], hit[0], hit[0]);
		AddVelocityToTransform(vertices, m_Transform, m_Velocity, hit[1], deltaTime);

		if (hit[0] || hit[1])
		{
			CreateBlock(roundf(m_Transform[0]), roundf(m_Transform[1]), i_Sand, walls, blocks, isSandOnX, blockTextures);
			return true;
		}
		break;
	}
	return false;
}
void Projectile::Draw(Shader sh
	, unsigned int transformLocation
	, float* transform)
{
	ChangeTransform(m_Transform[0], m_Transform[1], transform);
	sh.SetUniformMat4(transformLocation, transform);
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_Texture));
	ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}