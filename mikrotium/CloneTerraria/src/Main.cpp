
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include"ErrorSystem.h"




int main()
{
	GLFWwindow* window;
	if (!glfwInit())
	{
		std::cout << "GLFW DONT WORK" << std::endl;
		return -1;
	}



	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	window = glfwCreateWindow(840, 720, "Mikrotium", NULL, NULL);
	if (!window)
	{
		std::cout << "CAN NOT CREATE WINDOW" << std::endl;
		glfwTerminate();
		return -1;
	}


	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW DONT WORK" << std::endl;
		return -1;
	}


	ErrorGL(glEnable(GL_BLEND));
	ErrorGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	

	unsigned int vao;
	glGenVertexArrays(1, &vao);

	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	char errorMessage[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorMessage);
		std::cout << "[OpenGLError] Shader error " << "vertex shader " << errorMessage << std::endl;
	}



	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorMessage);
		std::cout << "[OpenGLError] Shader error " << "fragment shader " << errorMessage << std::endl;
	}
	
	
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorMessage);
		std::cout << "[OpenGLError] Shader error " << "linking fail " << errorMessage << std::endl;
	}
	
	ErrorGL(glDeleteShader(vertexShader));
	ErrorGL(glDeleteShader(fragmentShader));
	
	float vertices[12] =
	{
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};
	unsigned int order[6] = 
	{
		0, 1, 3,
		1, 2, 3 
	};
	ErrorGL(glBindVertexArray(vao));
	unsigned int VBID;
	ErrorGL(glGenBuffers(1, &VBID));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, VBID));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER,sizeof(vertices) , vertices, GL_STATIC_DRAW));
	ErrorGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(float), nullptr));
	ErrorGL(glEnableVertexAttribArray(0));

	unsigned int eob;
	ErrorGL(glGenBuffers(1, &eob));
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(order), order, GL_STATIC_DRAW));

	ErrorGL(glBindVertexArray(0));

	
	
	ErrorGL(glUseProgram(shaderProgram));
	ErrorGL(glBindVertexArray(vao));



	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);


		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}