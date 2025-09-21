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
#include"glfw/input.h"
#include"player.h"
#include"Collision.h"
#include"glfw/Window.h"
#include"glfw/cursor.h"
#include"NumberRender.h"






int main()
{
	if (!glfwInit())
	{
		std::cout << "GLFW DONT WORK" << std::endl;
		std::cin.get();
		return -1;
	}


	if (!Window::GetInfoForWindow("res/settings.txt"))
	{
		std::cout << "settings error dont read all informations" << std::endl;
		std::cin.get();
		return -1;
	}
	GLFWwindow* window;
	if (Window::fullScreen)
	{
		window = glfwCreateWindow(Window::width, Window::height, "Mikrotium", glfwGetPrimaryMonitor(), NULL);
	}
	else
	{
		window = glfwCreateWindow(Window::width, Window::height, "Mikrotium", NULL, NULL);
	}
	Window::PostWindowSettings(window);
	glfwSetCursorPosCallback(window, Input::CursorPositionCallback);
	glfwSetKeyCallback(window, Input::KeyCallback);
	glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
	glfwSetScrollCallback(window, Input::ScrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!window)
	{
		std::cout << "CAN NOT CREATE WINDOW" << std::endl;
		glfwTerminate();
		std::cin.get();
		return -1;
	}


	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW DONT WORK" << std::endl;
		std::cin.get();
		return -1;
	}



	double pastTime = glfwGetTime();

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


	Shader basicSh("res/shaders/vertexShaderBasic.txt", "res/shaders/fragmentShaderBasic.txt");
	basicSh.Bind();
	unsigned int transformLocation = basicSh.GetUniformLocation("transform");
	unsigned int cameraLocation = basicSh.GetUniformLocation("camera");
	/*Shader fontSh("res/shaders/vertexShader.txt", "res/shaders/fragmentShaderBasic.txt");
	fontSh.Bind();
	unsigned int fontTransformLocation = fontSh.GetUniformLocation("transform");
	unsigned int fontCameraLocation = fontSh.GetUniformLocation("camera");*/
	Shader HUDSh("res/shaders/vertexShaderHUD.txt", "res/shaders/fragmentShaderBasic.txt ");
	HUDSh.Bind();
	unsigned int HUDTransformLocation = HUDSh.GetUniformLocation("HUDTransform");
	unsigned int HUDScaleLocation = HUDSh.GetUniformLocation("HUDScale");
	
	float MoveLeft;
	float MoveUp;

	Player player(eob, HUDTransformLocation, HUDScaleLocation,MoveUp,MoveLeft);

	float transform[16];
	CreateCamera(0, Window::width, 0, Window::height, player.m_Camera);
	HUDSh.SetUniformMat4(HUDSh.GetUniformLocation("HUDCamera"), player.m_Camera);
	CreateTransform(MoveLeft, MoveUp, transform);
	HUDSh.SetUniformMat4(HUDSh.GetUniformLocation("HUDBasicLocation"), transform);
	ChangeCamera(-Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform, player.m_Camera);
	

	unsigned int cursorTextures[5];
	unsigned int cursorDD = CreateCursorDrawData(cursorTextures, eob);
	unsigned int numberTexture;
	unsigned int fontDrawData = CreateDrawDataNumbers(eob, numberTexture);
	unsigned int blockTextures[19];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	CreateAllBlockTextures(blockTextures);
	unsigned int blocksDrawData;
	SetupBlockDrawData(blocksDrawData, eob);
	
	
	std::vector<Block> blocks;
	LoadMap("res/save/map.txt", blocks, blockTextures);


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
		if (timer >= 1)
		{
		std::cout << fps << std::endl;
			timer = 0;
			fps = 0;
		}
		
		
		player.EveryFrame(deltaTime, blocks);
		ChangeCamera(-Window::halfWidthOfGameTransform + player.m_Transform[0], Window::halfWidthOfGameTransform + player.m_Transform[0], -Window::halfHeightOfGameTransform + player.m_Transform[1], Window::halfHeightOfGameTransform + player.m_Transform[1], player.m_Camera);

		
	
		
		basicSh.Bind();
		basicSh.SetUniformMat4(cameraLocation, player.m_Camera);

		ErrorGL(glBindVertexArray(blocksDrawData));
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks.at(i).DrawBlock(basicSh, transformLocation, transform);
		}

		
		player.DrawPlayer(basicSh, HUDSh, transformLocation, transform);

		DrawCursor(cursorTextures, cursorDD, basicSh, transformLocation, transform, cameraLocation, player);

		Input::EndOfLoop();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	std::cin.get();
	return 0;

}