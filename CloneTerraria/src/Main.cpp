#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include<vector>
#include<ctime>
#include<iostream>
#include<string.h>

#include"Opengl/ErrorSystem.h"
#include"Opengl/shader.h"
#include"Opengl/DrawData.h"
#include"Opengl/Texture.h"
#include"Math/matrix.h"
#include"libraries/stb_image.h"
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
#include"Enemy.h"
#include"background.h"
#include"particles.h"
#include"structures.h"
#include"glfw/Font.h"
#include"math/VectorOperation.h"


int main()
{
	unsigned int gameState = stateMainMenu;
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

	float lineHeight;
	std::vector<Letter> letters;	
	LoadFont(letters, lineHeight, "res/font/atlas.json");
	unsigned int fontTex = CreateTextureRGB("res/font/atlas.png");

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

	Shader fontSh("res/shaders/verFont.txt", "res/shaders/fragFont.txt");
	fontSh.Bind();
	fontSh.GetUniformLocation("camera");
	fontSh.GetUniformLocation("transform");
	Shader basicSh("res/shaders/verBasic.txt", "res/shaders/fragBasic.txt");
	basicSh.Bind();
	basicSh.GetUniformLocation("camera");
	basicSh.GetUniformLocation("transform");
	unsigned int cursorTextures[8];
	unsigned int cursorDD = CreateCursorDrawData(cursorTextures, eob);

	unsigned int numberTexture;
	unsigned int dotTex;
	unsigned int dotDD;
	unsigned int fontDrawData = CreateDrawDataNumbers(eob, numberTexture, dotTex, dotDD);
	unsigned int TextBackGroundTex = CreateTextureRGBA("res/textures/blue.png");


	float camera[16];
	float scale[16];
	float transform[16];
	float rotation[16];
	CreateTransform(0, 0, transform);
	CreateCamera(0, Window::width, 0, Window::height, camera);
	basicSh.Bind();
	basicSh.SetUniformMat4(basicCamera, camera);		
	fontSh.Bind();
	fontSh.SetUniformMat4(fontCamera, camera);
	
	float deltaTime;
	float printFPSTimer = 1;
	float oldDeltaTime;
	float spawnTimer =0;
	while (!glfwWindowShouldClose(window))
	{
		switch (gameState)
		{
		case stateMainMenu:
		{
			unsigned int backGroundVBO;
			unsigned int backGroundDD = CreateDrawData(eob, Window::height, 0, Window::width, 0, backGroundVBO);
			unsigned int backGroundTex;

			{
				int randomNumber = rand() % 3;
				switch (randomNumber)
				{
				case 0:
					backGroundTex = CreateTextureRGBA("res/textures/BackgroundForest.png");
					break;
				case 1:
					backGroundTex = CreateTextureRGBA("res/textures/BackgroundSand.png");
					break;
				default:
					backGroundTex = CreateTextureRGBA("res/textures/BackgroundSnow.png");
					break;
				}
			}
			Text menu[4];
			std::string Texts[4] = { "Start", "Editor", "Options", "Exit" };
			for(int i = 0;i < 4 ; i++)
			{ 
				menu[i].CreateText(Texts[i], std::vector<Format>{ Format(6, 8, 0, 0, 0, 1)}, letters, eob, lineHeight, middleBottom, Window::width / 2.0f, Window::height / 6.0f * (4 - i));
			}
			
			while (!glfwWindowShouldClose(window) && gameState == stateMainMenu)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				int aimingAt = -1;
				for (int i = 0; i < 4; i++)
				{
					float vertices[4] =
					{
						menu[i].m_TextVertices[0] + menu[i].m_Transform[0]
						, menu[i].m_TextVertices[1] + menu[i].m_Transform[1]
						, menu[i].m_TextVertices[2] + menu[i].m_Transform[0]
						, menu[i].m_TextVertices[3] + menu[i].m_Transform[1]
					};
					if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
					{
						aimingAt = i;
						break;
					}
				}
				if (Input::LeftMousePress)
				{
					switch (aimingAt)
					{
					case 0:
						gameState = stateInGame;
						break;
					case 1:
						break;
					case 2:
						break;
					case 3:	
						glfwSetWindowShouldClose(window, true);
						break;

					default:
						break;
					}
				}
				basicSh.Bind();
				ChangeTransform(0, 0, transform);
				basicSh.SetUniformMat4(basicTransform, transform);
				ErrorGL(glBindVertexArray(backGroundDD));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, backGroundTex));
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

				fontSh.Bind();
				for (int i = 0; i < 4; i++)
				{
					menu[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, i == aimingAt);
				}

				
				basicSh.Bind();
				DrawCursor(cursorTextures, aimingAt + 1, cursorDD, basicSh, transform, camera);
				Input::EndOfLoop();
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
			for (int i = 0; i < 4; i++)
			{
				menu[i].deleteText();
			}
			ErrorGL(glDeleteTextures(1, &backGroundTex));
			ErrorGL(glDeleteBuffers(1, &backGroundVBO));
			glDeleteVertexArrays(1, &backGroundDD);
			break;
		}
		case stateInGame:
		{
			Shader numberSh("res/shaders/verNumber.txt", "res/shaders/fragHUD.txt");
			numberSh.Bind();
			numberSh.GetUniformLocation("fontCamera");
			numberSh.GetUniformLocation("fontTransform");
			numberSh.GetUniformLocation("fontScale");
			numberSh.GetUniformLocation("fontLetter");
			numberSh.GetUniformLocation("shadow");
			numberSh.GetUniformLocation("craftingY");
			numberSh.SetUniform1f(numberSize + HUDCraftingY, 0);

			Shader HUDSh("res/shaders/verHUD.txt", "res/shaders/fragHUD.txt ");
			HUDSh.Bind();
			HUDSh.GetUniformLocation("HUDCamera");
			HUDSh.GetUniformLocation("HUDTransform");
			HUDSh.GetUniformLocation("HUDScale");
			HUDSh.GetUniformLocation("HUDBasicLocation");
			HUDSh.GetUniformLocation("shadow");
			HUDSh.GetUniformLocation("craftingY");
			HUDSh.SetUniform1f(HUDSize + HUDCraftingY, 0);

			Shader shadowSh("res/shaders/verbasic.txt", "res/shaders/fragShadow.txt ");
			shadowSh.Bind();
			shadowSh.GetUniformLocation("camera");
			shadowSh.GetUniformLocation("transform");
			shadowSh.GetUniformLocation("shadow");
			Shader treeSh("res/shaders/verTree.txt", "res/shaders/fragBasic.txt");
			treeSh.Bind();
			treeSh.GetUniformLocation("treeCamera");
			treeSh.GetUniformLocation("treeTransform");
			treeSh.GetUniformLocation("treeRotation");
			Shader animSh("res/shaders/verAnimation.txt", "res/shaders/fragBasic.txt ");
			animSh.Bind();
			animSh.GetUniformLocation("animCamera");
			animSh.GetUniformLocation("animTransform");
			animSh.GetUniformLocation("animScale");
			animSh.GetUniformLocation("animNumber");
			animSh.GetUniformLocation("animLeangth");
			Shader handSh("res/shaders/verHand.txt", "res/shaders/fragBasic.txt");
			handSh.Bind();
			handSh.GetUniformLocation("camera");
			handSh.GetUniformLocation("transform");
			handSh.GetUniformLocation("scale");
			handSh.GetUniformLocation("rotation");
			handSh.GetUniformLocation("beginTransform");
			Shader advancedSh("res/shaders/verAdvanced.txt", "res/shaders/fragShadow.txt");
			advancedSh.Bind();
			advancedSh.GetUniformLocation("camera");
			advancedSh.GetUniformLocation("transform");
			advancedSh.GetUniformLocation("scale");
			advancedSh.GetUniformLocation("rotation");
			advancedSh.GetUniformLocation("shadow");
			Shader backgroundSh("res/shaders/verBasic.txt", "res/shaders/fragBackground.txt");
			backgroundSh.Bind();
			backgroundSh.GetUniformLocation("camera");
			backgroundSh.GetUniformLocation("transform");
			backgroundSh.GetUniformLocation("blendFactor");
			Shader particlesSh("res/shaders/verParticles.txt", "res/shaders/fragParticles.txt");
			particlesSh.Bind();
			particlesSh.GetUniformLocation("camera");
			particlesSh.GetUniformLocation("transform");
			particlesSh.GetUniformLocation("scale");
			particlesSh.GetUniformLocation("rotation");
			particlesSh.GetUniformLocation("color");
			Shader structureSh("res/shaders/verBasic.txt", "res/shaders/fragStructures.txt");
			structureSh.Bind();
			structureSh.GetUniformLocation("camera");
			structureSh.GetUniformLocation("transform");
			structureSh.GetUniformLocation("shadow");
			structureSh.GetUniformLocation("lookAt");

			
			CreateScale(1, 1, scale);
			CreateRotation(0, rotation);
			ChangeTransform(PLAYERHANDOFFSETX, PLAYERHANDOFFSETY, transform);
			ChangeCamera(0, Window::width, 0, Window::height, camera);
			advancedSh.Bind();
			advancedSh.SetUniform1i(advancedSize + ShadowLocation, 0);
			advancedSh.SetUniformMat4(advancedRotation, rotation);
			handSh.Bind();
			handSh.SetUniformMat4(handBeginTransform, transform);
			treeSh.Bind();
			treeSh.SetUniformMat4(treeRotation, rotation);
			HUDSh.Bind();
			HUDSh.SetUniformMat4(HUDCamera, camera);
			numberSh.Bind();
			numberSh.SetUniformMat4(numberCamera, camera);
			ChangeCamera(-Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform, camera);


			unsigned int enemiesTex1[enemySize];
			unsigned int enemiesTex2[enemySize];
			unsigned int enemiesDD1[enemySize];
			unsigned int enemiesDD2[enemySize];
			unsigned int blockTextures[21];//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			unsigned int treeTextures[3];
			unsigned int CutTextures[4];
			unsigned int structuresTextures[8];
			unsigned int DoorTextures[2] = { CreateTextureRGBA("res/textures/CloseDoor.png"), CreateTextureRGBA("res/textures/openDoor.png") };
			unsigned int trapDoorTextures[2] = { CreateTextureRGBA("res/textures/CloseTrapDoor.png"), CreateTextureRGBA("res/textures/OpenTrapDoor.png") };
			unsigned int openChestTex = CreateTextureRGBA("res/textures/OpenChest.png");
			unsigned int damageTexture[2] = { CreateTextureRGBA("res/textures/DamageBlock.png"), CreateTextureRGBA("res/textures/lightDamageBlock.png") };
			CutTextures[0] = CreateTextureRGBA("res/textures/cut4.png");
			CutTextures[1] = CreateTextureRGBA("res/textures/cut3.png");
			CutTextures[2] = CreateTextureRGBA("res/textures/cut2.png");
			CutTextures[3] = CreateTextureRGBA("res/textures/cut1.png");
			structuresTextures[s_Sapling] = CreateTextureRGBA("res/textures/sapling.png");
			structuresTextures[s_Chest] = CreateTextureRGBA("res/textures/Chest.png");
			structuresTextures[s_CraftingTable] = CreateTextureRGBA("res/textures/bench.png");
			structuresTextures[s_Forge] = CreateTextureRGBA("res/textures/forge.png");
			structuresTextures[s_Anvil] = CreateTextureRGBA("res/textures/anvil.png");
			structuresTextures[s_Door] = DoorTextures[0];
			structuresTextures[s_TrapDoor] = trapDoorTextures[0];
			structuresTextures[s_Gate] = CreateTextureRGBA("res/textures/CloseGate.png");
			treeTextures[part_Crown] = CreateTextureRGBA("res/textures/forestBush.png");
			treeTextures[part_SmallCrown] = CreateTextureRGBA("res/textures/forestSmallBush.png");
			treeTextures[part_Log] = CreateTextureRGBA("res/textures/woodLog.png");
			unsigned int treeDD[3];
			unsigned int structuresDD[8];

			structuresDD[s_Sapling] = CreateDrawData(eob, 1.5f, -0.5f, 0.5f, -0.5f);
			structuresDD[s_Chest] = CreateDrawData(eob, 1.5f, -0.5f, 1.5f, -0.5f);
			structuresDD[s_CraftingTable] = CreateDrawData(eob, 0.5f, -0.5f, 1.5f, -0.5f);
			structuresDD[s_Forge] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -0.5f);
			structuresDD[s_Anvil] = structuresDD[s_CraftingTable];
			structuresDD[s_Door] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -1.5f);
			structuresDD[s_TrapDoor] = CreateDrawData(eob, 1.5f, -1.5f, 1.5f, -0.5f);
			structuresDD[s_Gate] = CreateDrawData(eob, 3.5f, -0.5f, 0.5f, -0.5f);
			treeDD[part_Log] = CreateDrawData(eob, 0.5f, -0.5f, 0.5f, -0.5f);
			treeDD[part_Crown] = CreateDrawData(eob, 4.5f, -0.5f, 3.5f, -3.5f);
			treeDD[part_SmallCrown] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -1.5f);

			CreateAllBlockTextures(blockTextures);
			
			unsigned int blocksDrawData = CreateDrawData(eob, 0.5f, -0.5, -0.5f, 0.5f, 1, 0, 0, 1);
			unsigned int particlesDD = CreateDrawData(eob, 0.125f, -0.125f, 0.125f, -0.125f);
			unsigned int itemDD = CreateDrawData(eob, 0.4f, -0.4f, 0.4f, -0.4f);


			Player player(eob, blockTextures, structuresTextures);



			std::vector<DroppedItem> dropItems;


			std::vector<std::vector<Block>> blocks;
			std::vector<DamagedBlock> damagedBlocks;
			std::vector<std::vector<wall>> walls;
			std::vector<DamagedBlock> damagedWalls;
			std::vector<tree> trees;
			std::vector<damagedWood> damagedTrees;
			std::vector<seedling> seedlings;
			std::vector<int> isSandOnX;
			std::vector<Enemy> enemies;
			std::vector<BoomParticle> boomParticles;
			std::vector<Projectile> projectiles;
			std::vector<CraftStation> craftStations;
			std::vector<Chest> chests;
			std::vector<Door> doors;


			LoadMapBlocksAndWalls("res/save/mapWalls.txt", "res/save/mapBlocks.txt", walls, blocks, isSandOnX, 0, 1080, -500, 360, blockTextures);
			Background background(eob, backgroundSh);


			while (!glfwWindowShouldClose(window) && gameState == stateInGame)
			{
				glClear(GL_COLOR_BUFFER_BIT);

				deltaTime = glfwGetTime() - pastTime;
				pastTime = glfwGetTime();




				float CameraCoordinates[2];
				CameraCoordinates[0] = CameraHitboxX(player.m_Transform[0]);
				CameraCoordinates[1] = CameraHitboxY(player.m_Transform[1]);
				EnemySpawnManager(deltaTime, spawnTimer, eob, enemiesTex1, enemiesTex2, enemiesDD1, enemiesDD2, CameraCoordinates, blocks, enemies);

				for (int i = 0; i < seedlings.size(); i++)
				{
					if (seedlings.at(i).everyFrame(deltaTime, treeTextures, treeDD, blocks, seedlings, trees))
					{
						seedlings.erase(seedlings.begin() + i);
					}

				}
				CheckFloorCraftStations(craftStations, blocks, dropItems);
				for (int i = 0; i < doors.size(); i++)
				{
					if (doors.at(i).CheckFloorAndCeil(doors, blocks, dropItems))
					{
						Door d = doors.at(i);
						dropItems.emplace_back(d.m_Transform[0], d.m_Transform[1], 0, GetItemIDByStructure(d.m_Type), 1, true);
						doors.at(i).DestroyDoor(blocks, walls, isSandOnX);
						doors.erase(i + doors.begin());
					}
				}
				for (int i = 0; i < enemies.size(); i++)
				{
					int damage = enemies.at(i).EnemyEveryFrame(deltaTime, blocks, player.m_Transform);
					if (damage)
					{
						if (enemies.at(i).m_IsBurning)
						{
							player.m_IsBurning = true;
							player.m_BurningTimer = 0;
						}
						player.DamagePlayer(enemies.at(i).m_Transform, damage);
					}
				}
				player.EveryFrame(deltaTime, blocks, walls, enemies, isSandOnX, craftStations, damagedTrees, damagedBlocks, damagedWalls, CameraCoordinates, blocksDrawData, blockTextures, structuresTextures, trees, seedlings, dropItems, projectiles, doors, chests);
				SandEveryFrame(isSandOnX, projectiles, blocks, walls, blockTextures[t_Sand], blocksDrawData);

				for (int i = 0; i < projectiles.size(); i++)
				{
					if (projectiles.at(i).EveryFrame(deltaTime, enemies, blocks, walls, craftStations, seedlings, trees, dropItems, boomParticles, doors, chests, isSandOnX, blockTextures))
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
						if (trees.at(i).m_Transform[0] == damagedTrees.at(j).m_Transform[0] && trees.at(i).m_Transform[1] == damagedTrees.at(j).m_Transform[1])
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
				advancedSh.Bind();
				advancedSh.SetUniformMat4(advancedCamera, camera);
				particlesSh.Bind();
				particlesSh.SetUniformMat4(particlesCamera, camera);
				backgroundSh.Bind();
				backgroundSh.SetUniformMat4(basicCamera, camera);
				background.DrawBackground(backgroundSh, basicSh, transform, CameraCoordinates);
				structureSh.Bind();
				structureSh.SetUniformMat4(basicCamera, camera);

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
				treeSh.SetUniformMat4(treeCamera, camera);
				for (int i = 0; i < trees.size(); i++)
				{
					trees.at(i).drawTree(treeSh, CameraCoordinates, transform, rotation);
				}
				ErrorGL(glBindVertexArray(blocksDrawData));
				for (int i = 0; i < damagedTrees.size(); i++)
				{
					damagedTrees.at(i).DrawCut(treeSh, rotation, transform, CutTextures);
				}
				DrawDoors(doors, advancedSh, structuresDD, structuresTextures, DoorTextures, trapDoorTextures, transform, scale, rotation);
				DrawCraftStations(craftStations, structureSh, transform, structuresDD, structuresTextures);
				DrawChests(chests, structureSh, transform, openChestTex, structuresDD, structuresTextures);
				shadowSh.Bind();
				shadowSh.SetUniform1i(basicSize + ShadowLocation, 0);
				ErrorGL(glBindVertexArray(itemDD));
				for (int i = 0; i < dropItems.size(); i++)
				{
					if (dropItems.at(i).m_Item >= i_WallDirt && dropItems.at(i).m_Item <= i_WallIce)
					{
						shadowSh.SetUniform1i(basicSize + ShadowLocation, 1);
					}
					dropItems.at(i).DrawItem(player.m_AllItemTextures, shadowSh, basicTransform, transform);
					if (dropItems.at(i).m_Item >= i_WallDirt && dropItems.at(i).m_Item <= i_WallIce)
					{
						shadowSh.SetUniform1i(basicSize + ShadowLocation, 0);
					}
				}
				particlesSh.Bind();
				ErrorGL(glBindVertexArray(particlesDD));
				for (int i = 0; i < boomParticles.size(); i++)
				{
					if (boomParticles.at(i).DrawParticles(particlesSh, deltaTime, transform, scale, rotation))
					{
						boomParticles.erase(boomParticles.begin() + i);
						i--;
					}
				}
				ChangeRotation(0, rotation);
				particlesSh.SetUniformMat4(particlesRotation, rotation);
				ChangeScale(1, 1, scale);
				particlesSh.SetUniformMat4(particlesScale, scale);

				animSh.Bind();
				for (int i = 0; i < enemies.size(); i++)
				{
					enemies.at(i).DrawEnemy(animSh, transform, scale);
				}
				particlesSh.Bind();
				ErrorGL(glBindVertexArray(particlesDD));
				for (int i = 0; i < enemies.size(); i++)
				{

					if (enemies.at(i).m_IsBurning)
					{
						enemies.at(i).m_BurningTimer += deltaTime;
						if (enemies.at(i).m_BurningTimer < TIMEONFIRE)
						{
							if (enemies.at(i).m_OnFire.DrawParticles(particlesSh, deltaTime, true, enemies.at(i).m_Transform, transform))
							{
								enemies.at(i).m_BurnDamageNextTime++;
								if (enemies.at(i).m_BurnDamageNextTime >= 4)
								{
									enemies.at(i).m_BurnDamageNextTime = 0;
									if (enemies.at(i).DamageEnemy(1, NULL))
									{
										enemies.erase(enemies.begin() + i);
										i--;
									}
								}
							}
						}
						else
						{
							enemies.at(i).m_BurnDamageNextTime = 0;
							enemies.at(i).m_BurningTimer = 0;
							enemies.at(i).m_IsBurning = false;
						}
					}
					else
					{
						enemies.at(i).m_OnFire.DrawParticles(particlesSh, deltaTime, false, enemies.at(i).m_Transform, transform);
					}

				}
				advancedSh.Bind();
				for (int i = 0; i < projectiles.size(); i++)
				{
					projectiles.at(i).Draw(advancedSh, transform, scale, rotation);
				}
				player.DrawPlayer(deltaTime, basicSh, HUDSh, numberSh, animSh, handSh, particlesSh, chests, transform, scale, rotation, fontDrawData, particlesDD, numberTexture);
				numberSh.Bind();
				ErrorGL(glBindVertexArray(fontDrawData));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));
				printFPSTimer += deltaTime;
				if (printFPSTimer > 0.1f)
				{
					printFPSTimer = 0;
					oldDeltaTime = deltaTime;
				}
				drawFloat(0, 0, 1.0f / oldDeltaTime, numberTexture, fontDrawData, dotTex, dotDD, scale, transform, numberSh);

				DrawCursor(cursorTextures, structuresTextures, structuresDD, cursorDD, blocksDrawData, shadowSh, structureSh, numberSh, transform, camera, scale, fontDrawData, numberTexture, player, CameraCoordinates);


				Input::EndOfLoop();
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
			break;
		}
		}
	}
	glfwTerminate();
	std::cin.get();
	return 0;

}