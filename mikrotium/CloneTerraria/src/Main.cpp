#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

#include"Opengl/ErrorSystem.h"
#include"Opengl/shader.h"
#include"Opengl/Texture.h"
#include"Math/matrix.h"
#include"imageLoader/stb_image.h"
#include"Blocks.h"
#include"GLFW/input.h"
#include"player.h"
#include"Collision.h"
#include"GLFW/Window.h"






int main()
{
	if (!Window::GetInfoForWindow("res/settings.txt"))
	{
		std::cout << "settings error dont read all informations" << std::endl;
		return -1;
	}

	GLFWwindow* window;
	if (!glfwInit())
	{
		std::cout << "GLFW DONT WORK" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (Window::fullScreen)
	{
		window = glfwCreateWindow(Window::width, Window::height, "Mikrotium", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		window = glfwCreateWindow(Window::width, Window::height, "Mikrotium", NULL, NULL);
	}


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


	double pastTime = glfwGetTime();
	glfwSetCursorPosCallback(window, Input::CursorPositionCallback);
	glfwSetKeyCallback(window, Input::KeyCallback);
	glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);

	stbi_set_flip_vertically_on_load(true);
	ErrorGL(glEnable(GL_BLEND));
	ErrorGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	unsigned char order[6] =
	{
		0, 1, 3,
		1, 2, 3
	};
	unsigned int eob;
	ErrorGL(glGenBuffers(1, &eob));
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(order), order, GL_STATIC_DRAW));

	

	Player player(eob);
	Camera(-Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform, player.m_Camera);
	Shader playerShader("res/shaders/vertexPlayer.txt", "res/shaders/fragmentShaderBasic.txt");
	playerShader.Bind();
	playerShader.SetUniformMat4("camera", player.m_Camera);
	
	unsigned int blocksDrawData;
	unsigned int blockTextures[2];
	CreateAllBlockTextures(blockTextures);
	SetupBlockDrawData(blocksDrawData, eob);
	Shader blockShader("res/shaders/vertexBlock.txt", "res/shaders/fragmentShaderBasic.txt");
	std::vector<Block> blocks;
	LoadMap("res/save/map.txt", blocks, blockTextures);
	std::vector<Block*> blocksInScene;

	float deltaTime;
	
	float timer = 0;
	int fps = 0;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		deltaTime = glfwGetTime() - pastTime;
		pastTime = glfwGetTime();
		
		
		timer += deltaTime;
		fps++;
		if (timer > 1)
		{
			std::cout << fps << std::endl;
			timer = 0;
			fps = 0;
		}
		


		player.EveryFrame(deltaTime, blocks);
		Camera(-Window::halfWidthOfGameTransform + player.m_Transform[0], Window::halfWidthOfGameTransform + player.m_Transform[0], -Window::halfHeightOfGameTransform + player.m_Transform[1], Window::halfHeightOfGameTransform + player.m_Transform[1], player.m_Camera);

		
		player.DrawPlayer(playerShader);
		blockShader.Bind();
		blockShader.SetUniformMat4("camera", player.m_Camera);
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks.at(i).DrawBlock(blocksDrawData);
		}
		blockShader.Unbind();
		Input::EndOfLoop();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;

}