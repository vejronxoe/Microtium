#include"Texture.h"

#include<gl/glew.h>

#include"ErrorSystem.h"
#include"../imageLoader/stb_image.h"


unsigned int CreateTextureRGB(const char* filePath)
{
	unsigned int iD;
	ErrorGL(glGenTextures(1, &iD));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, iD));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

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
	return iD;
}
unsigned int CreateTextureLinearRGBA(const char* filePath)
{
	unsigned int iD;
	ErrorGL(glGenTextures(1, &iD));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, iD));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	}
	else
	{
		std::cout << "failed to Load a texture" << filePath << std::endl;
	}
	stbi_image_free(data);
	return iD;
}
unsigned int CreateTextureRGBA(const char* filePath)
{
	unsigned int iD;
	ErrorGL(glGenTextures(1, &iD));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, iD));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	}
	else
	{
		std::cout << "failed to Load a texture" << filePath << std::endl;
	}
	stbi_image_free(data);
	return iD;
}
unsigned int CreateTextureRepeatRGBA(const char* filePath)
{
	unsigned int iD;
	ErrorGL(glGenTextures(1, &iD));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, iD));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);

	if (data)
	{
		ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	}
	else
	{
		std::cout << "failed to Load a texture" << filePath << std::endl;
	}
	stbi_image_free(data);
	return iD;
}

