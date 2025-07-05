#include"Texture.h"

#include<gl/glew.h>

#include"ErrorSystem.h"
#include"../imageLoader/stb_image.h"


Texture::Texture(const char* filePath, bool isItRGBA)
{
	if (isItRGBA)
	{
		ErrorGL(glGenTextures(1, &m_ID));
		ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ID));
		ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

		if (data)
		{
			ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "failed to Load a texture" << filePath << std::endl;
		}
		stbi_image_free(data);
		return;
	}
	ErrorGL(glGenTextures(1, &m_ID));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ID));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to Load a texture" << filePath << std::endl;
	}
	stbi_image_free(data);
}
void Texture::Bind()
{
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ID));
}
void Texture::Unbind()
{
	ErrorGL(glBindTexture(GL_TEXTURE_2D, 0));

}
