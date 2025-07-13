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
	std::unordered_map<std::string, unsigned int> m_UniformsLocations;
	unsigned int GetUniformLocation(std::string& name);
	unsigned int CompileShader(unsigned int type, const char* source, const char* nameShader);
	void LinkBasicShader(unsigned int vertexShader, unsigned int fragmentShader);
public:
	Shader(const char* VertexShaderSource, const char* FragmentShadersource);
	~Shader();
	void SetUniform4f(std::string name, float v0, float v1, float v2, float v3);
	void SetUniform1f(std::string name, float v);
	void SetUniformMat4(std::string name, float* v);
	void Bind();
	void Unbind();
};