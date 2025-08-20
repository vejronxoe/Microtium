#pragma once
#include<GL/glew.h>
#include<iostream>
#include<unordered_map>

#include"ErrorSystem.h"

class Shader
{
private:
	unsigned int m_ID;
	std::string fileShaderRead(std::string filePath);
	unsigned int CompileShader(unsigned int type, const char* source, const char* nameShader);
	void LinkBasicShader(unsigned int vertexShader, unsigned int fragmentShader);
public:
	Shader(const char* VertexShaderSource, const char* FragmentShadersource);
	void DeleteShader();
	unsigned int GetUniformLocation(const char* name);
	void SetUniform4f(unsigned int location, float v0, float v1, float v2, float v3);
	void SetUniform1f(unsigned int location, float v);
	void SetUniformMat4(unsigned int location, float* v);
	void Bind();
	void Unbind();
};

