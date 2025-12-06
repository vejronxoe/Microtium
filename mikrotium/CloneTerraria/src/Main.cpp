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
#include"walls.h"
#include"flora.h"




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
	Shader HUDSh("res/shaders/vertexShaderHUD.txt", "res/shaders/fragmentShaderShadow.txt ");
	HUDSh.Bind();
	unsigned int HUDTransformLocation = HUDSh.GetUniformLocation("HUDTransform");
	unsigned int HUDScaleLocation = HUDSh.GetUniformLocation("HUDScale");
	unsigned int HUDShadowLocation = HUDSh.GetUniformLocation("shadow");
	Shader shadowSh("res/shaders/vertexShaderbasic.txt", "res/shaders/fragmentShaderShadow.txt ");
	shadowSh.Bind();
	unsigned int shadowTransformLocation = shadowSh.GetUniformLocation("transform");
	unsigned int shadowCameraLocation = shadowSh.GetUniformLocation("camera");
	unsigned int shadowLocation = shadowSh.GetUniformLocation("shadow");
	Shader treeSh("res/shaders/vertexShaderTree.txt", "res/shaders/fragmentShaderBasic.txt");
	treeSh.Bind();
	unsigned int treeTransformLocation = treeSh.GetUniformLocation("treeTransform");
	unsigned int treeCameraLocation = treeSh.GetUniformLocation("treeCamera");
	unsigned int treeRotationLocation = treeSh.GetUniformLocation("treeRotation");
	float camera[16];
	float scale[16];
	float transform[16];
	float rotation[16];
	CreateRotation(0, rotation);
	treeSh.SetUniformMat4(treeRotationLocation, rotation);

	CreateScale(1, 1, scale);
	CreateRotation(0, rotation);
	HUDSh.Bind();
	CreateCamera(0, Window::width, 0, Window::height, camera);
	HUDSh.SetUniformMat4(HUDSh.GetUniformLocation("HUDCamera"), camera);

	fontSh.Bind();
	fontSh.SetUniformMat4(fontSh.GetUniformLocation("fontCamera"), camera);
	ChangeCamera(-Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform, camera);


	unsigned int treeTextures[3];
	unsigned int CutTextures[4];
	CutTextures[0] = CreateTextureRGBA("res/textures/cut4.png");
	CutTextures[1] = CreateTextureRGBA("res/textures/cut3.png");
	CutTextures[2] = CreateTextureRGBA("res/textures/cut2.png");
	CutTextures[3] = CreateTextureRGBA("res/textures/cut1.png");
	unsigned int treeDD[3];
	treeDD[p_Log] = CreateDrawData(eob, 0.5f, -0.5f, 0.5f, -0.5f);
	treeDD[p_Crown] = CreateDrawData(eob, 4.5f, -0.5f, 3.5f, -3.5f);
	treeDD[p_SmallCrown] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -1.5f);
	treeTextures[p_Crown] = CreateTextureRGBA("res/textures/forestBush.png");
	treeTextures[p_SmallCrown] = CreateTextureRGBA("res/textures/forestSmallBush.png");
	treeTextures[p_Log] = CreateTextureRGBA("res/textures/woodLog.png");

	unsigned int blockTextures[20];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	CreateAllBlockTextures(blockTextures);
	unsigned int cursorTextures[6];
	unsigned int cursorDD = CreateCursorDrawData(cursorTextures, eob);
	unsigned int numberTexture;
	unsigned int fontDrawData = CreateDrawDataNumbers(eob, numberTexture);
	unsigned int blocksDrawData = CreateDrawData(eob, 0.5f, -0.5, -0.5f, 0.5f, 1, 0, 0, 1);

	unsigned int damageTexture[2] = {CreateTextureRGBA("res/textures/DamageBlock.png"), CreateTextureRGBA("res/textures/lightDamageBlock.png")};
	unsigned int itemDD = CreateDrawData(eob, 0.4f, -0.4f, 0.4f, -0.4f);


	float MoveLeft;
	float MoveUp;

	Player player(eob, HUDTransformLocation, HUDScaleLocation, MoveUp, MoveLeft, blockTextures);
	HUDSh.Bind();
	CreateTransform(MoveLeft, MoveUp, transform);
	HUDSh.SetUniformMat4(HUDSh.GetUniformLocation("HUDBasicLocation"), transform);



	std::vector<DroppedItem> dropItems;
	dropItems.emplace_back(124, 0, 1, i_Ice, 1, true);
	
	std::vector<std::vector<Block>> blocks;
	std::vector<DamagedBlock> damagedBlocks;
	LoadMap("res/save/mapBlocks.txt", blocks, 0, 200, -100, 100, blockTextures);
	std::vector<std::vector<wall>> walls;
	std::vector<DamagedBlock> damagedWalls;
	std::vector<tree> trees;
	std::vector<damagedWood> damagedTrees;
	LoadMapWall("res/save/mapWalls.txt", blocks, walls, blockTextures);


	trees.emplace_back(treeTextures[p_Crown], treeDD[p_Crown], i_ForestPlank, 35, p_Crown, 40, 5, 0.0f);
	trees.emplace_back(treeTextures[p_SmallCrown], treeDD[p_SmallCrown], i_Nothing, 35, p_SmallCrown, 38, 2, 90.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 39, 2, 90.0f);
	trees.emplace_back(treeTextures[p_SmallCrown], treeDD[p_SmallCrown], i_Nothing, 35, p_SmallCrown, 42, -3, -90.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 41, -3, -90.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, 4, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, 3, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, 2, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, 1, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, 0, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -1, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -2, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -3, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -4, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -5, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -6, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -7, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -8, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -9, 0.0f);
	trees.emplace_back(treeTextures[p_Log], treeDD[p_Log], i_ForestPlank, 35, p_Log, 40, -10, 0.0f);



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


		player.EveryFrame(deltaTime, blocks, walls, damagedTrees, damagedBlocks, damagedWalls, CameraCoordinates, blockTextures, trees,dropItems);
		if (damagedBlocks.size() > 20)
		{
			damagedBlocks.erase(damagedBlocks.begin());
		}
		else if (damagedWalls.size() > 20)
		{
			damagedWalls.erase(damagedWalls.begin());
		}

		CameraCoordinates[0] = CameraHitboxX(player.m_Transform[0]);
		CameraCoordinates[1] = CameraHitboxY(player.m_Transform[1]);
		ChangeCamera(-Window::halfWidthOfGameTransform + CameraCoordinates[0], Window::halfWidthOfGameTransform + CameraCoordinates[0], -Window::halfHeightOfGameTransform + CameraCoordinates[1], Window::halfHeightOfGameTransform + CameraCoordinates[1], camera);
		
		
	
		
		

		ErrorGL(glBindVertexArray(blocksDrawData));
		drawBlocks(blocks, damagedBlocks, CameraCoordinates, basicSh, damageTexture, transformLocation, transform, cameraLocation, camera);
		drawWalls(damagedWalls, damageTexture, walls, shadowSh,shadowLocation, shadowCameraLocation, camera, shadowTransformLocation, transform, CameraCoordinates);
		treeSh.Bind();
		treeSh.SetUniformMat4(treeCameraLocation, camera);
		for (int i = 0; i < trees.size(); i++)
		{
			trees.at(i).drawTree(treeSh, treeTransformLocation, treeRotationLocation, CameraCoordinates, transform, rotation);	
		}
		for (int i = 0; i < damagedTrees.size(); i++)
		{
			damagedTrees.at(i).DrawCut(treeSh, treeTransformLocation, treeRotationLocation, rotation, transform, CutTextures);
		}
		shadowSh.Bind();
		ErrorGL(glBindVertexArray(itemDD));
		shadowSh.SetUniform1i(shadowLocation, 0);
		for (int i = 0; i < dropItems.size(); i++)
		{
			if (dropItems.at(i).m_Item >= i_WallDirt && dropItems.at(i).m_Item <= i_WallIce)
			{
				shadowSh.SetUniform1i(shadowLocation, 1);
			}
			dropItems.at(i).DrawItem(player.m_AllItemTextures, shadowSh, shadowTransformLocation, transform);
			if (dropItems.at(i).m_Item >= i_WallDirt && dropItems.at(i).m_Item <= i_WallIce)
			{
				shadowSh.SetUniform1i(shadowLocation, 0);
			}
		}
		basicSh.Bind();
		player.DrawPlayer(basicSh, HUDSh, fontSh, HUDShadowLocation, transformLocation, transform, scale, fontDrawData, fontLetterLocation, fontTransformLocation, fontscaleLocation,numberTexture);

		DrawCursor(cursorTextures, cursorDD, blocksDrawData, shadowSh, fontSh, shadowLocation, shadowTransformLocation, transform, camera, scale, shadowCameraLocation, fontDrawData, fontLetterLocation, fontTransformLocation, fontscaleLocation, numberTexture, player, CameraCoordinates);
		

		Input::EndOfLoop();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	std::cin.get();
	return 0;

}