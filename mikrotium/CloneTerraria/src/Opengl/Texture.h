#pragma once
class Texture
{
private:
	unsigned int m_ID;

public:
	Texture(const char* filePath, bool isItRGBA);
	void Bind();
	void Unbind();
};