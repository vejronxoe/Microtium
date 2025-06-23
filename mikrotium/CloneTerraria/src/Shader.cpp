#include "Shader.h"

#include<GL/glew.h>
#include<iostream>

unsigned int CompileShader(unsigned int type, const char* source,const char* nameShader = "no shader name was assigned")
{
	int success;
	char errorMessage[512];
	unsigned int shader;
	shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, 0, errorMessage);
		std::cout << "[OpenGLError] Shader error {" << nameShader << "} " << errorMessage << std::endl;
	}
	return shader;
}

void LinkShader(unsigned int shaderProgram)
{
	int success;
	char errorMessage[512];
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, errorMessage);
		std::cout << "[OpenGLError] Shader error " << "linking fail (it could be you dont attach the shaders) " << errorMessage << std::endl;
	}
}
