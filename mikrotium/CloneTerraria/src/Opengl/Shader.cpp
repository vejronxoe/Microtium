#include "Shader.h"

#include<iostream>
#include<fstream>
#include<sstream>

void Shader::DeleteShader() 
{
	ErrorGL(glDeleteProgram(m_ID));
}
std::string Shader::fileShaderRead(std::string filePath)
{
	std::string shader;
	std::string line;
	std::ifstream shaderFile(filePath);
	if (!shaderFile)
	{
		std::cout << "can not open file :" << filePath<< std::endl;
	}
	while (std::getline(shaderFile, line))
	{
		shader += line + "\n";
	}
	shaderFile.close();
	return shader;
}
unsigned int Shader::GetUniformLocation(const char* name)
{
	 unsigned int location = glGetUniformLocation(m_ID, name);
	if (location == -1)
	{
		std::cout << "can not find uniform named "<< name << " or your dont bind shader" << std::endl;
	}
	return location;
}

Shader::Shader(const char* filePathVertexShader, const char* filePathFragmentShader)
		:m_ID(glCreateProgram())
	{
	std::string vertexShaderSource = fileShaderRead(filePathVertexShader);
	std::string fragmentShaderSource = fileShaderRead(filePathFragmentShader);
		unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str(), "Vertex Shader");
		unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str(), "Fragment Shader");
		LinkBasicShader(vertexShader, fragmentShader);
	}

void Shader::SetUniform4f(unsigned int location,float v0, float v1, float v2, float v3)
{
	ErrorGL(glUniform4f(location, v0, v1, v2, v3));
}
void Shader::SetUniform1f(unsigned int location, float v)
{
	ErrorGL(glUniform1f(location, v));
}

void Shader::SetUniform1i(unsigned int location, float v)
{
	ErrorGL(glUniform1i(location, v));
}

void Shader::SetUniformMat4(unsigned int location, float* v)
{
	ErrorGL(glUniformMatrix4fv(location, 1, GL_FALSE, v));
}


unsigned int Shader::CompileShader(unsigned int type, const char* source, const char* nameShader = "no shader name was assigned")
{
	int success;
	char errorMessage[512];
	unsigned int shader;
	shader = glCreateShader(type);
	ErrorGL(glShaderSource(shader, 1, &source, 0));
	ErrorGL(glCompileShader(shader));
	ErrorGL(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, 0, errorMessage);
		std::cout << "[OpenGLError] Shader error {" << nameShader << "} " << errorMessage << std::endl;
	}
	return shader;
}

void Shader::LinkBasicShader(unsigned int vertexShader, unsigned int fragmentShader)
{
	int success;
	char errorMessage[512];
	ErrorGL(glAttachShader(m_ID, vertexShader));
	ErrorGL(glAttachShader(m_ID, fragmentShader));
	ErrorGL(glLinkProgram(m_ID));
	ErrorGL(glGetProgramiv(m_ID, GL_LINK_STATUS, &success));
	if (!success)
	{
		glGetShaderInfoLog(m_ID, 512, NULL, errorMessage);
		std::cout << "[OpenGLError] Shader error " << "linking fail " << errorMessage << std::endl;
	}
	ErrorGL(glDeleteShader(vertexShader));
	ErrorGL(glDeleteShader(fragmentShader));
}

void Shader::Bind()
{
	ErrorGL(glUseProgram(m_ID));
}

void Shader::Unbind()
{
	ErrorGL(glUseProgram(0));
}
