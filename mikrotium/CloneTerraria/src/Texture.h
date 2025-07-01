#pragma once
class Texture
{
private:
	unsigned int m_ID;

public:
	Texture(const char* filePath);
	void Bind();
	void Unbind();
};