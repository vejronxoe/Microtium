#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<vector>
#include<ctime>

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
#include"BlocksAndWalls.h"
#include"NumberRender.h"
#include"DroppedItems.h"
#include"flora.h"
#include"projectile.h"


int main()
{
	srand(time(0));
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
	basicSh.GetUniformLocation("camera");
	basicSh.GetUniformLocation("transform");
	Shader fontSh("res/shaders/vertexShaderfont.txt", "res/shaders/fragmentShaderBasic.txt");
	fontSh.Bind();
	fontSh.GetUniformLocation("fontCamera");
	fontSh.GetUniformLocation("fontTransform");
	fontSh.GetUniformLocation("fontScale");
	fontSh.GetUniformLocation("fontLetter");
	Shader HUDSh("res/shaders/vertexShaderHUD.txt", "res/shaders/fragmentShaderShadow.txt ");
	HUDSh.Bind();
	HUDSh.GetUniformLocation("HUDCamera");
	HUDSh.GetUniformLocation("HUDTransform");
	HUDSh.GetUniformLocation("HUDScale");
	HUDSh.GetUniformLocation("HUDBasicLocation");
	HUDSh.GetUniformLocation("shadow");
	Shader shadowSh("res/shaders/vertexShaderbasic.txt", "res/shaders/fragmentShaderShadow.txt ");
	shadowSh.Bind();
	shadowSh.GetUniformLocation("camera");
	shadowSh.GetUniformLocation("transform");
	shadowSh.GetUniformLocation("shadow");
	Shader treeSh("res/shaders/vertexShaderTree.txt", "res/shaders/fragmentShaderBasic.txt");
	treeSh.Bind();
	treeSh.GetUniformLocation("treeCamera");
	treeSh.GetUniformLocation("treeTransform");
	treeSh.GetUniformLocation("treeRotation");
	Shader animSh("res/shaders/vertexShaderAnimation.txt", "res/shaders/fragmentShaderBasic.txt ");
	animSh.Bind();
	animSh.GetUniformLocation("animCamera");
	animSh.GetUniformLocation("animTransform");
	animSh.GetUniformLocation("animScale");
	animSh.GetUniformLocation("animNumber");
	animSh.GetUniformLocation("animLeangth");
	Shader handSh("res/shaders/vertexShaderHand.txt", "res/shaders/fragmentShaderBasic.txt");
	handSh.Bind();
	handSh.GetUniformLocation("camera");
	handSh.GetUniformLocation("transform");
	handSh.GetUniformLocation("scale");
	handSh.GetUniformLocation("rotation");
	handSh.GetUniformLocation("beginTransform");
	float camera[16];
	float scale[16];
	float transform[16];
	float rotation[16];
	CreateScale(1, 1, scale);
	CreateRotation(0, rotation);
	treeSh.Bind();
	treeSh.SetUniformMat4( treeRotation, rotation);


	HUDSh.Bind();
	CreateCamera(0, Window::width, 0, Window::height, camera);
	HUDSh.SetUniformMat4( HUDCamera, camera);

	fontSh.Bind();
	fontSh.SetUniformMat4(fontCamera, camera);
	ChangeCamera(-Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform, camera);

	unsigned int projectileTextures[1];
	unsigned int blockTextures[21];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	unsigned int cursorTextures[6];
	unsigned int treeTextures[3];
	unsigned int CutTextures[4];
	unsigned int structuresTextures[1];
	unsigned int damageTexture[2] = { CreateTextureRGBA("res/textures/DamageBlock.png"), CreateTextureRGBA("res/textures/lightDamageBlock.png") };
	CutTextures[0] = CreateTextureRGBA("res/textures/cut4.png");
	CutTextures[1] = CreateTextureRGBA("res/textures/cut3.png");
	CutTextures[2] = CreateTextureRGBA("res/textures/cut2.png");
	CutTextures[3] = CreateTextureRGBA("res/textures/cut1.png");
	structuresTextures[s_Sapling] = CreateTextureRGBA("res/textures/sapling.png");
	treeTextures[part_Crown] = CreateTextureRGBA("res/textures/forestBush.png");
	treeTextures[part_SmallCrown] = CreateTextureRGBA("res/textures/forestSmallBush.png");
	treeTextures[part_Log] = CreateTextureRGBA("res/textures/woodLog.png");
	projectileTextures[p_Sand] = CreateTextureRGBA("res/textures/proSand.png");
	unsigned int treeDD[3];
	unsigned int structuresDD[1];
	structuresDD[s_Sapling] = CreateDrawData(eob, 1.5f, -0.5f, 0.5f, -0.5f);
	treeDD[part_Log] = CreateDrawData(eob, 0.5f, -0.5f, 0.5f, -0.5f);
	treeDD[part_Crown] = CreateDrawData(eob, 4.5f, -0.5f, 3.5f, -3.5f);
	treeDD[part_SmallCrown] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -1.5f);

	CreateAllBlockTextures(blockTextures);
	unsigned int cursorDD = CreateCursorDrawData(cursorTextures, eob);
	unsigned int numberTexture;
	unsigned int fontDrawData = CreateDrawDataNumbers(eob, numberTexture);
	unsigned int blocksDrawData = CreateDrawData(eob, 0.5f, -0.5, -0.5f, 0.5f, 1, 0, 0, 1);

	unsigned int itemDD = CreateDrawData(eob, 0.4f, -0.4f, 0.4f, -0.4f);


	float MoveLeft;
	float MoveUp;

	Player player(eob, MoveUp, MoveLeft, blockTextures);
	HUDSh.Bind();
	CreateTransform(MoveLeft, MoveUp, transform);
	HUDSh.SetUniformMat4(HUDBasicLocation, transform);



	std::vector<DroppedItem> dropItems;
	dropItems.emplace_back(124, 0, 1, i_Ice, 1, true);
	
	std::vector<std::vector<Block>> blocks;
	std::vector<DamagedBlock> damagedBlocks;
	std::vector<std::vector<wall>> walls;
	std::vector<DamagedBlock> damagedWalls;
	std::vector<tree> trees;
	std::vector<damagedWood> damagedTrees;
	std::vector<seedling> seedlings;
	std::vector<bool> isSandOnX;

	std::vector<Projectile> projectiles;

	LoadMapBlocksAndWalls("res/save/mapWalls.txt", "res/save/mapBlocks.txt", walls, blocks,isSandOnX, 0, 200, -100, 100, blockTextures);
	




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
		for (int i = 0; i < seedlings.size(); i++)
		{
			if (seedlings.at(i).everyFrame(deltaTime, treeTextures, treeDD, blocks, seedlings, trees))
			{
				seedlings.erase(seedlings.begin() + i);
			}

		}

		player.EveryFrame(deltaTime, blocks, walls, isSandOnX, damagedTrees, damagedBlocks, damagedWalls, CameraCoordinates, blockTextures, structuresTextures, trees, seedlings,dropItems);
		SandEveryFrame(isSandOnX, projectiles, blocks, walls, projectileTextures[p_Sand], blocksDrawData);
		
		for (int i = 0; i < projectiles.size(); i++)
		{
			if (projectiles.at(i).EveryFrame(deltaTime, blocks, walls, isSandOnX, blockTextures))
			{
				projectiles.erase(projectiles.begin() + i);
			}

		}

		
		
		if (damagedBlocks.size() > 20)
		{
			damagedBlocks.erase(damagedBlocks.begin());
		}
		else if (damagedWalls.size() > 20)
		{
			damagedWalls.erase(damagedWalls.begin());
		}
		else if (damagedTrees.size() > 20)
		{
			damagedTrees.erase(damagedTrees.begin());
		}
		for (int i = 0; i < damagedBlocks.size(); i++)
		{
			bool ExistenceOfBlock = false;
			FindBlock(blocks, damagedBlocks.at(i).m_Transform[0], damagedBlocks.at(i).m_Transform[1], ExistenceOfBlock);
			if (!ExistenceOfBlock)
			{
				damagedBlocks.erase(damagedBlocks.begin() + i);
			}
		}
		for (int j = 0; j < damagedTrees.size(); j++)
		{
			bool ExistenceOfWood = false;
			for (int i = 0; i < trees.size(); i++)
			{
				if (trees.at(i).m_Transform[0] == damagedTrees.at(j).m_Transform[0] && trees.at(i).m_Transform[1] == damagedTrees.at(j).m_Transform[1] )
				{
					ExistenceOfWood = true;
					break;
				}	
			}
			if (!ExistenceOfWood)
			{
				damagedTrees.erase(damagedTrees.begin() + j);
			}
		}
		CameraCoordinates[0] = CameraHitboxX(player.m_Transform[0]);
		CameraCoordinates[1] = CameraHitboxY(player.m_Transform[1]);
		ChangeCamera(-Window::halfWidthOfGameTransform + CameraCoordinates[0], Window::halfWidthOfGameTransform + CameraCoordinates[0], -Window::halfHeightOfGameTransform + CameraCoordinates[1], Window::halfHeightOfGameTransform + CameraCoordinates[1], camera);
		animSh.Bind();
		animSh.SetUniformMat4(animCamera, camera);
		handSh.Bind();
		handSh.SetUniformMat4(handCamera, camera);
		
		shadowSh.Bind();
		ErrorGL(glBindVertexArray(blocksDrawData));
		drawWalls(damagedWalls, damageTexture, walls, shadowSh, camera, transform, CameraCoordinates);
		basicSh.Bind();
		drawBlocks(blocks, damagedBlocks, CameraCoordinates, basicSh, damageTexture, transform, camera);
	
		basicSh.Bind();
		ErrorGL(glBindVertexArray(structuresDD[s_Sapling]));
		for (int i = 0; i < seedlings.size(); i++)
		{
			seedlings.at(i).drawSeedling(basicSh, transform);
		}
		treeSh.Bind();
		treeSh.SetUniformMat4( treeCamera, camera);
		for (int i = 0; i < trees.size(); i++)
		{
			trees.at(i).drawTree(treeSh, CameraCoordinates, transform, rotation);	
		}
		ErrorGL(glBindVertexArray(blocksDrawData));
		for (int i = 0; i < damagedTrees.size(); i++)
		{
			damagedTrees.at(i).DrawCut(treeSh, rotation, transform, CutTextures);
		}
		shadowSh.Bind();
		shadowSh.SetUniform1i(basicSize + ShadowLocation, 0);
		ErrorGL(glBindVertexArray(itemDD));
		for (int i = 0; i < dropItems.size(); i++)
		{
			if (dropItems.at(i).m_Item >= i_WallDirt && dropItems.at(i).m_Item <= i_WallIce)
			{
				shadowSh.SetUniform1i(basicSize + ShadowLocation, 1);
			}
			dropItems.at(i).DrawItem(player.m_AllItemTextures, shadowSh, basicTranform, transform);
			if (dropItems.at(i).m_Item >= i_WallDirt && dropItems.at(i).m_Item <= i_WallIce)
			{
				shadowSh.SetUniform1i(basicSize + ShadowLocation, 0);
			}
		}
		basicSh.Bind();

		for (int i = 0; i < projectiles.size(); i++)
		{
			projectiles.at(i).Draw(basicSh, transform);
		}
		player.DrawPlayer(basicSh, HUDSh, fontSh, animSh, handSh, transform, scale, rotation, fontDrawData, numberTexture);

		DrawCursor(cursorTextures, structuresTextures, structuresDD, cursorDD, blocksDrawData, shadowSh, fontSh, transform, camera, scale, fontDrawData, numberTexture, player, CameraCoordinates);
		

		Input::EndOfLoop();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	std::cin.get();
	return 0;

}