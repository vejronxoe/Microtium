#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>

#include"Opengl/ErrorSystem.h"
#include"Opengl/shader.h"
#include"Opengl/DrawData.h"
#include"Opengl/Texture.h"
#include"Math/matrix.h"
#include"imageLoader/stb_image.h"
#include"glfw/Window.h"
#include"glfw/cursor.h"
#include"glfw/input.h"
#include"player.h"
#include"Collision.h"
#include"Blocks.h"
#include"NumberRender.h"
#include"DroppedItems.h"





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
	Shader fontSh("res/shaders/vertexShaderfont.txt", "res/shaders/fragmentShaderBasic.txt");
	fontSh.Bind();
	unsigned int fontTransformLocation = fontSh.GetUniformLocation("fontTransform");
	unsigned int fontscaleLocation = fontSh.GetUniformLocation("fontScale");
	unsigned int fontLetterLocation = fontSh.GetUniformLocation("fontLetter");
	Shader HUDSh("res/shaders/vertexShaderHUD.txt", "res/shaders/fragmentShaderBasic.txt ");
	HUDSh.Bind();
	unsigned int HUDTransformLocation = HUDSh.GetUniformLocation("HUDTransform");
	unsigned int HUDScaleLocation = HUDSh.GetUniformLocation("HUDScale");
	
	unsigned int cursorTextures[5];
	unsigned int cursorDD = CreateCursorDrawData(cursorTextures, eob);
	unsigned int numberTexture;
	unsigned int fontDrawData = CreateDrawDataNumbers(eob, numberTexture);
	unsigned int blockTextures[19];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	CreateAllBlockTextures(blockTextures);
	unsigned int blocksDrawData;
	SetupBlockDrawData(blocksDrawData, eob);
	unsigned int damageTexture = CreateTextureRGBA("res/textures/DamageBlock.png");
	unsigned int itemDD = CreateDrawData(eob, 0.4f, -0.4f, 0.4f, -0.4f);

	float MoveLeft;
	float MoveUp;

	Player player(eob, HUDTransformLocation, HUDScaleLocation, MoveUp, MoveLeft, blockTextures);

	float transform[16];
	CreateCamera(0, Window::width, 0, Window::height, player.m_Camera);
	HUDSh.SetUniformMat4(HUDSh.GetUniformLocation("HUDCamera"), player.m_Camera);
	CreateTransform(MoveLeft, MoveUp, transform);
	HUDSh.SetUniformMat4(HUDSh.GetUniformLocation("HUDBasicLocation"), transform);
	fontSh.Bind();
	fontSh.SetUniformMat4(fontSh.GetUniformLocation("fontCamera"), player.m_Camera);
	ChangeCamera(-Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform, player.m_Camera);


	
	std::vector<DroppedItem> dropItems;
	dropItems.emplace_back(124, 0, 1, i_Ice, 1, true);
	

	
	std::vector<std::vector<Block>> blocks;
	std::vector<DamagedBlock> damagedBlocks;
	LoadMap("res/save/map.txt", blocks, 0, 200, -100, 100, blockTextures);


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

		float CameraCoordinates[2];
		CameraCoordinates[0] = CameraHitboxX(player.m_Transform[0]);
		CameraCoordinates[1] = CameraHitboxY(player.m_Transform[1]);


		player.EveryFrame(deltaTime, blocks, damagedBlocks, CameraCoordinates, blockTextures,dropItems);
		for (int i = 0; i < dropItems.size();i++)
		{
			if (dropItems.at(i).EveryFrame(deltaTime, blocks, player.m_Transform, player.HavePlayerSpace(dropItems.at(i).m_Item)))
			{
				unsigned short int itemSwapCheck;
				if (player.m_UseSlot == 0)
				{
					itemSwapCheck = player.m_PlayerSlots[player.m_HUDUseSlot];
				}
				if (player.ItermGetToInventory(dropItems.at(i).m_Amount, dropItems.at(i).m_Item))
				{
					if (player.m_UseSlot == 0 && itemSwapCheck != player.m_PlayerSlots[player.m_HUDUseSlot])
					{
						player.m_PlayerSlots[0] = player.m_PlayerSlots[player.m_HUDUseSlot];
						player.m_AmountInSlots[0] = player.m_AmountInSlots[player.m_HUDUseSlot];
						player.SwapItemStats();
					}
					dropItems.erase(dropItems.begin() + i);
				}
			}
		}
		CameraCoordinates[0] = CameraHitboxX(player.m_Transform[0]);
		CameraCoordinates[1] = CameraHitboxY(player.m_Transform[1]);
		ChangeCamera(-Window::halfWidthOfGameTransform + CameraCoordinates[0], Window::halfWidthOfGameTransform + CameraCoordinates[0], -Window::halfHeightOfGameTransform + CameraCoordinates[1], Window::halfHeightOfGameTransform + CameraCoordinates[1], player.m_Camera);

		
	
		
		basicSh.Bind();
		basicSh.SetUniformMat4(cameraLocation, player.m_Camera);

		ErrorGL(glBindVertexArray(blocksDrawData));
		for (int j = 0; j < blocks.size(); j++)
		{
			for (int i = 0; i < blocks.at(j).size(); i++)
			{
				blocks.at(j).at(i).DrawBlock(basicSh, transformLocation, transform);
			}
		}
		for (int i = 0; i < damagedBlocks.size(); i++)
		{
			damagedBlocks.at(i).DrawDamage(basicSh, transformLocation, transform, damageTexture);
		}
		for (int i = 0; i < dropItems.size(); i++)
		{
			dropItems.at(i).DrawItem(player.m_AllItemTextures, itemDD, basicSh, transformLocation, transform);
		}

		player.DrawPlayer(basicSh, HUDSh, fontSh,transformLocation, transform, fontDrawData, fontLetterLocation, fontTransformLocation, fontscaleLocation,numberTexture);

		DrawCursor(cursorTextures, cursorDD, blocksDrawData, basicSh, transformLocation, transform, cameraLocation, player);


		Input::EndOfLoop();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	std::cin.get();
	return 0;

}