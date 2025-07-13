#include "Shader.h"

#include<iostream>
#include<fstream>
#include<sstream>

Shader::~Shader()
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
	return shader;
}
unsigned int Shader::GetUniformLocation(std::string &name)
{
	if (m_UniformsLocations.find(name) == m_UniformsLocations.end())
	{
		m_UniformsLocations[name] = glGetUniformLocation(m_ID, name.c_str());
	}
	if (m_UniformsLocations[name] == -1)
	{
		std::cout << "can not find uniform named "<< name << "Or bind shader" << std::endl;
	}
	return m_UniformsLocations[name];
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

void Shader::SetUniform4f(std::string name,float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}
void Shader::SetUniform1f(std::string name, float v)
{
	glUniform1i(GetUniformLocation(name), v);
}

void Shader::SetUniformMat4(std::string name, float* v)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, v);
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
