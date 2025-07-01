
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>

#include"ErrorSystem.h"
#include"shader.h"
#include"Texture.h"
#include"imageLoader/stb_image.h"
#include"matrix.h"



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



	window = glfwCreateWindow(1500, 720, "Mikrotium", NULL, NULL);
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

	stbi_set_flip_vertically_on_load(true);
	ErrorGL(glEnable(GL_BLEND));
	ErrorGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	const char* fragmentShaderSource =
		"#version 330 core\n"

		"out vec4 FragColor;\n"
		"in vec2 texCoord;\n"
		"uniform sampler2D ourTexture;\n"

		"void main()\n"
		"{\n"
		"	FragColor = texture(ourTexture, texCoord);\n"
		"}\0";
	const char* vertexShaderSource =
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec2 aTexCoord;\n"

		"out vec2 texCoord;\n"

		"uniform mat4 camera;"

		"void main()\n"
		"{\n"
			"texCoord = aTexCoord;\n"
		"   gl_Position = camera * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	Shader sh(vertexShaderSource, fragmentShaderSource);

	float vertices[20] =
	{
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f
	};

	unsigned char order[6] = 
	{
		0, 1, 3,
		1, 2, 3 
	};


	ErrorGL(glBindVertexArray(vao));
	unsigned int VBID;
	ErrorGL(glGenBuffers(1, &VBID));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, VBID));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER,sizeof(vertices) , vertices, GL_STATIC_DRAW));

	ErrorGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	ErrorGL(glEnableVertexAttribArray(0));
	ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
	ErrorGL(glEnableVertexAttribArray(1));

	unsigned int eob;
	ErrorGL(glGenBuffers(1, &eob));
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(order), order, GL_STATIC_DRAW));

	ErrorGL(glBindVertexArray(0));
	Texture tex("res/textures/player0.jpg");

	sh.Bind();
	float* posicionCamera = Camera(0.0f, 1500.0f, 0.0f, 720.0f);
	sh.SetUniformMat4("camera", posicionCamera);
	tex.Bind();
	ErrorGL(glBindVertexArray(vao));
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}