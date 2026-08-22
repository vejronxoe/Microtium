#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<vector>
#include<ctime>
#include<iostream>
#include<string>
#include <algorithm>

#include"opengl/ErrorSystem.h"
#include"opengl/Shader.h"
#include"opengl/DrawData.h"
#include"opengl/Texture.h"
#include"math/matrix.h"
#include"libraries/stb_image.h"
#include"glfw/Window.h"
#include"glfw/cursor.h"
#include"glfw/input.h"
#include"Player.h"
#include"Collision.h"
#include"BlocksAndWalls.h"
#include"NumberRender.h"
#include"DroppedItems.h"
#include"flora.h"
#include"background.h"
#include"particles.h"
#include"structures.h"
#include"glfw/Font.h"
#include"math/VectorOperation.h"
#include"Editor.h"
#include"SaveAndLoad.h"
#include"glfw/input.h"
#include"light.h"

struct Menu
{
	Slider sliders[2];
	CheckBox checkBoxs[3];
	Text description[5];
	Text values[2];
	Text saveText[3];
	Text loadText[3];
	Text backText;
};
void CreateMenu(bool first
	, std::vector<Letter>& letters
	, Menu& menu
	, unsigned int& sliderDD
	, unsigned int& sliderVBO
	, unsigned int* checkBoxTex
	, unsigned int sliderTex
	, unsigned int trailTex
	, unsigned int eob)
{
	Format basicform(14, 5, 0, 0, 0, 1);
	if (!first)
	{
		ErrorGL(glDeleteBuffers(1, &sliderVBO));
		ErrorGL(glDeleteVertexArrays(1, &sliderDD));
	}
	float winSideLength = 0;
	if (Window::height > Window::width)
	{
		winSideLength = Window::width;
	}
	else
	{
		winSideLength = Window::height;
	}
	float sideLength = DistanceOnUI(0.05f);
	sliderDD = CreateDrawData(eob, sideLength, -sideLength, sideLength, -sideLength, sliderVBO);
	std::string texts[5] = { "smooth shadows\n(please reset the game in order\nto see differnt shadows)", "vsync", "full screne", "game zoom", "volume" };
	menu.description[4].CreateText(texts[4], std::vector<Format>{basicform}, letters, eob, leftTop, 0.02f * Window::width, 0.98f * Window::height);
	for (int i = 3; i > 0; i--)
	{
		menu.description[i].CreateText(texts[i], std::vector<Format>{{basicform}}, letters, eob, leftTop, 0.02f * Window::width, menu.description[i + 1].m_TextVertices[3] + menu.description[i + 1].m_Transform[1] - Window::height * 0.02f);
	}
	menu.description[0].CreateText(texts[0], std::vector<Format>{basicform, {80, 2, 1, 0.5f, 0.5f, 1 }}, letters, eob, leftTop, 0.02f * Window::width, menu.description[1].m_TextVertices[3] + menu.description[ 1].m_Transform[1] - Window::height * 0.02f);
	for (int i = 0 ; i < 3; i++)
	{
		menu.checkBoxs[i].Create(checkBoxTex, eob, leftTop, (menu.description[0].m_TextVertices[2] + menu.description[0].m_Transform[0] ) / winSideLength, (menu.description[1].m_TextVertices[3] + menu.description[i].m_Transform[1] + winSideLength - Window::height) / winSideLength, (menu.description[0].m_TextVertices[2] - menu.description[1].m_TextVertices[3] + menu.description[0].m_Transform[0] ) / winSideLength, (menu.description[i].m_TextVertices[1] + menu.description[i].m_Transform[1] + winSideLength - Window::height) / winSideLength);
	}
	for (int i  = 0; i < 2; i++)
	{
		menu.sliders[i].CreateSlider(sliderTex, trailTex, sliderDD, eob, leftTop, 0.50f,(menu.description[3+i].m_TextVertices[3]+ menu.description[3 + i].m_Transform[1] + winSideLength - Window::height)/winSideLength,0.87f,(menu.description[3 + i].m_TextVertices[1] + menu.description[3 + i].m_Transform[1] + winSideLength - Window::height) / winSideLength);
	}
	int values[2] = {static_cast<int>(Window::gameZoom*100), static_cast<int>(Window::volume*100)};
	for (int i = 0; i < 2 ; i++)
	{
		menu.values[i].CreateText("   " + std::to_string(values[i]), std::vector<Format>{{15, 3, 0, 0, 0, 1}}, letters, eob, leftTop, menu.sliders[i].m_Vertices[2], menu.sliders[i].m_Vertices[1]);
	}
	menu.backText.CreateText("Back", std::vector<Format>{basicform}, letters, eob, middleBottom, 0.5f * Window::width, 0.02f *Window::height);
	for (int i = 0; i < 3;i++)
	{
		menu.loadText[i].CreateText("Load" + std::to_string(i), std::vector<Format>{{15, 12, 0, 0, 0, 1}}, letters, eob, middleTop, 0.5f * Window::width, (1.0f - (i + 1) * (1 / 6.0f))*Window::height);
		menu.saveText[i].CreateText("Save"+std::to_string(i), std::vector<Format>{{15, 12, 0, 0, 0, 1}}, letters, eob, middleTop, 0.5f * Window::width, (1.0f-(i+1)*(1/6.0f))* Window::height);
	}

}
void optionsUpdate(Menu& menu
	, Shader& fontSh
	, Shader& basicSh
	, GLFWwindow* window
	, std::vector<Letter>& letters
	, unsigned int eob
	, unsigned int fontTex
	, unsigned int TextBackGroundTex
	, float* transform
	, float& blockSize
	, int& cursorState
	,unsigned int& menuState)
{
	for (int i = 0; i < 5; i++)
	{
		menu.description[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, false);
	}
	for (int i = 0; i < 2; i++)
	{
		menu.values[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, false);
	}
	float Textvertices[4] =
	{
		menu.backText.m_TextVertices[0] + menu.backText.m_Transform[0]
		, menu.backText.m_TextVertices[1] + menu.backText.m_Transform[1]
		, menu.backText.m_TextVertices[2] + menu.backText.m_Transform[0]
		, menu.backText.m_TextVertices[3] + menu.backText.m_Transform[1]
	};
	if (IsInArea(Textvertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
	{
		cursorState = canClickOnIt;
		if (Input::LeftMousePress)
		{
			Window::SaveSetting("res/settings.dat");
			menuState = stateDefault;
		}
	}
	menu.backText.Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, false);
	basicSh.Bind();
	bool howeringOver = false;
	float value = menu.sliders[0].Update(Window::gameZoom, howeringOver);
	if (Window::gameZoom != value)
	{
		Window::gameZoom = value;
		blockSize = DistanceOnUI(BlockSize);
		Window::halfHeightOfGameTransform = (Window::height / blockSize) / 2.0f;
		Window::halfWidthOfGameTransform = (Window::width / blockSize) / 2.0f;
		menu.values[0].CreateText("   " + std::to_string(int(Window::gameZoom*100)), std::vector<Format>{ {15, 3, 0, 0, 0, 1} }, letters, eob, leftTop, menu.sliders[0].m_Vertices[2], menu.sliders[0].m_Vertices[1]);

	}
	if (howeringOver){cursorState = canSlideIt;}
	menu.sliders[0].Draw(basicSh, Window::gameZoom , transform);

	value = menu.sliders[1].Update(Window::volume, howeringOver);
	if (Window::volume != value)
	{
		Window::volume = value;
		menu.values[1].CreateText("   " + std::to_string(int(Window::volume * 100)), std::vector<Format>{ {15, 3, 0, 0, 0, 1} }, letters, eob, leftTop, menu.sliders[1].m_Vertices[2], menu.sliders[1].m_Vertices[1]);
	}
	if (howeringOver){cursorState = canSlideIt;}
	menu.sliders[1].Draw(basicSh, Window::volume, transform);




	if (menu.checkBoxs[2].Update(Input::LeftMousePress, Window::fullScreen))
	{
		if (Input::LeftMousePress)
		{
			if (Window::fullScreen)
			{
				GLFWmonitor* primary = glfwGetPrimaryMonitor();

				const GLFWvidmode* mode = glfwGetVideoMode(primary);

				int maxHeight = mode->height;
				int maxWidth = mode->width;
				glfwSetWindowMonitor(window, primary, 0, 0, maxWidth, maxHeight, GLFW_DONT_CARE);
			}
			else
			{
				glfwSetWindowMonitor(window, NULL, 0, 100, Window::windowWidth, Window::windowHeight, GLFW_DONT_CARE);
			}
		}
		cursorState = canClickOnIt;
	}
	if (menu.checkBoxs[1].Update(Input::LeftMousePress, Window::VSync))
	{
		if (Input::LeftMousePress)
		{
			if (Window::VSync)
			{
				glfwSwapInterval(1);
			}
			else
			{
				glfwSwapInterval(0);
			}
		}
		cursorState = canClickOnIt;
	}

	if (menu.checkBoxs[0].Update(Input::LeftMousePress, Window::smoothShadows))
	{
		cursorState = canClickOnIt;
	}

	basicSh.Bind();
	bool table[3] = {Window::smoothShadows,Window::VSync,Window::fullScreen};
	for (int i = 0; i < 3; i++)
	{
		menu.checkBoxs[i].Draw(basicSh,table[i], transform);
	}

}
int main()
{
	Blocks::xMax = 1080;
	Blocks::xMin = 0;
	Blocks::yMax = 360;
	Blocks::yMin = -500;
	unsigned int gameState = stateMainMenu;
	srand(time(0));
	if (!glfwInit())
	{
		std::cout << "GLFW DONT WORK" << std::endl;
		std::cin.get();
		return -1;
	}


	if (!Window::GetInfoForWindow("res/settings.dat"))
	{
		std::cout << "settings error dont read all informations" << std::endl;
		std::cin.get();
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window;

	window = glfwCreateWindow(Window::width, Window::height, "Mikrotium", (Window::fullScreen ? glfwGetPrimaryMonitor() : NULL), NULL);


	if (!window)
	{
		std::cout << "CAN NOT CREATE WINDOW" << std::endl;
		glfwTerminate();
		std::cin.get();
		return -1;
	}

	glfwSetWindowSizeLimits(window, 400, 400, GLFW_DONT_CARE, GLFW_DONT_CARE);

	glfwGetWindowSize(window, &Window::width, &Window::height);
	float blockSize = (DistanceOnUI(BlockSize));

	Window::halfHeightOfGameTransform = (Window::height / blockSize) / 2.0f;
	Window::halfWidthOfGameTransform = (Window::width / blockSize) / 2.0f;



	glfwSetCursorPosCallback(window, Input::CursorPositionCallback);
	glfwSetKeyCallback(window, Input::KeyCallback);
	glfwSetMouseButtonCallback(window, Input::MouseButtonCallback);
	glfwSetScrollCallback(window, Input::ScrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress)))
	{
		std::cout << "GLAD DONT WORK" << std::endl;
		return -1;
	}

	if (Window::VSync)
	{
		glfwSwapInterval(1);

	}
	else
	{
		glfwSwapInterval(0);
	}

	std::vector<Letter> letters;
	LoadFont(letters, Window::lineHeight, "res/font/atlas.json");
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
	Shader numberSh("res/shaders/verNumber.txt", "res/shaders/fragHUD.txt");
	numberSh.Bind();
	numberSh.GetUniformLocation("fontCamera");
	numberSh.GetUniformLocation("fontTransform");
	numberSh.GetUniformLocation("fontScale");
	numberSh.GetUniformLocation("fontLetter");
	numberSh.GetUniformLocation("shadow");
	numberSh.GetUniformLocation("craftingY");
	numberSh.SetUniform1f(numberSize + HUDCraftingY, 0);
	unsigned int numberTexture;
	unsigned int dotTex;
	unsigned int dotDD;
	unsigned int fontDrawData = CreateDrawDataNumbers(eob, numberTexture, dotTex, dotDD);
	unsigned int blocksDrawData = CreateDrawData(eob, 0.5f, -0.5, -0.5f, 0.5f, 1, 0, 0, 1);
	unsigned int particlesDD = CreateDrawData(eob, 0.125f, -0.125f, 0.125f, -0.125f);
	unsigned int itemDD = CreateDrawData(eob, 0.4f, -0.4f, 0.4f, -0.4f);


	Shader fontSh("res/shaders/verFont.txt", "res/shaders/fragFont.txt");
	fontSh.Bind();
	fontSh.GetUniformLocation("camera");
	fontSh.GetUniformLocation("transform");
	Shader basicSh("res/shaders/verBasic.txt", "res/shaders/fragBasic.txt");
	basicSh.Bind();
	basicSh.GetUniformLocation("camera");
	basicSh.GetUniformLocation("transform");
	unsigned int cursorTextures[sizeOfCursors];
	unsigned int cursorDD = CreateCursorDrawData(cursorTextures, eob);

	unsigned int checkBoxTex[2] = { CreateTextureRGBA("res/textures/UncheckBox.png"), CreateTextureRGBA("res/textures/CheckBox.png") };
	unsigned int sliderTex = CreateTextureRGBA("res/textures/Slider.png");
	unsigned int trailTex = CreateTextureRGBA("res/textures/Trail.png");
	unsigned int TextBackGroundTex = CreateTextureRGBA("res/textures/blue.png");
	std::string pathToSave = "res/save";
	Menu menu;
	unsigned int sliderVBO = -1;
	unsigned int sliderDD = -1;
	CreateMenu(true,letters,menu,sliderDD,sliderVBO,checkBoxTex,sliderTex,trailTex,eob);
     

	float camera[16];
	float scale[16];
	float transform[16];
	float rotation[16];
	CreateTransform(0, 0, transform);
	CreateRotation(0, rotation);
	CreateScale(1, 1, scale);
	CreateCamera(0, Window::width, 0, Window::height, camera);
	numberSh.Bind();
	numberSh.SetUniformMat4(numberCamera, camera);
	basicSh.Bind();
	basicSh.SetUniformMat4(basicCamera, camera);		
	fontSh.Bind();
	fontSh.SetUniformMat4(fontCamera, camera);
	
	float deltaTime = 0;
	float printFPSTimer = 1;
	float oldDeltaTime = 0;
	float spawnTimer =0;
	unsigned int menuState = stateDefault;

	/*/////////////////////////////////////////////
	gameState = stateEditor;

	pathToSave += "0/";



	//////////*////////////////////////////////////////////
	while (!glfwWindowShouldClose(window))
	{
		switch (gameState)
		{	
		case stateMainMenu:
		{

			unsigned int backGroundVBO;
			unsigned int backGroundDD = CreateDrawData(eob, 1, 0, 1, 0, backGroundVBO);
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
			Text menuDefault[4];
			
		
			
			{
				std::string Texts[4] = { "Start", "Editor", "Options", "Exit" };
				for (int i = 0; i < 4; i++)
				{
					menuDefault[i].CreateText(Texts[i], std::vector<Format>{ Format(6, 13, 0, 0, 0, 1)}, letters, eob, middleBottom, Window::width / 2.0f, Window::height / 6.0f * (4 - i));
				}
			}
		
			unsigned int loadGameState = stateInGame;
			while (!glfwWindowShouldClose(window) && gameState == stateMainMenu)
			{
				int newWidth, newHeight;
				glfwGetWindowSize(window, &newWidth, &newHeight);
				if(newHeight != Window::height || newWidth != Window::width)
				{ 
					Window::height = newHeight;
					Window::width = newWidth;
					ChangeScreenSize(newWidth, newHeight);
					{
						std::string Texts[4] = { "Start", "Editor", "Options", "Exit" };
						for (int i = 0; i < 4; i++)
						{
							menuDefault[i].CreateText(Texts[i], std::vector<Format>{ Format(6, 13, 0, 0, 0, 1)}, letters, eob, middleBottom, Window::width / 2.0f, Window::height / 6.0f * (4 - i));
						}
					}
					CreateMenu(false, letters, menu, sliderDD, sliderVBO, checkBoxTex, sliderTex, trailTex, eob);
					ChangeCamera(0,Window::width,0, Window::height, camera);
					numberSh.Bind();
					numberSh.SetUniformMat4(numberCamera, camera);
					basicSh.Bind();
					basicSh.SetUniformMat4(basicCamera, camera);
					fontSh.Bind();
					fontSh.SetUniformMat4(fontCamera, camera);


				}


				glClear(GL_COLOR_BUFFER_BIT);
				deltaTime = glfwGetTime() - pastTime;
				pastTime = glfwGetTime();
				int aimingAt = -1;
				int cursorState = canNotDoIt;

				basicSh.Bind();
				ChangeTransform(0, 0, transform);
				ChangeCamera(0, 1, 0, 1, camera);
				basicSh.SetUniformMat4(basicCamera, camera);
				basicSh.SetUniformMat4(basicTransform, transform);
				ErrorGL(glBindVertexArray(backGroundDD));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, backGroundTex));
				ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				ChangeCamera(0, Window::width, 0, Window::height, camera);
				basicSh.SetUniformMat4(basicCamera, camera);

				fontSh.Bind();
				ErrorGL(glBindTexture(GL_TEXTURE_2D, fontTex));

				switch (menuState)
				{
				case stateDefault:
				{
					for (int i = 0; i < 4; i++)
					{
						float vertices[4] =
						{
							menuDefault[i].m_TextVertices[0] + menuDefault[i].m_Transform[0]
							, menuDefault[i].m_TextVertices[1] + menuDefault[i].m_Transform[1]
							, menuDefault[i].m_TextVertices[2] + menuDefault[i].m_Transform[0]
							, menuDefault[i].m_TextVertices[3] + menuDefault[i].m_Transform[1]
						};
						if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
						{
							aimingAt = i;
							cursorState = canClickOnIt;
							break;
						}
					}
					if (Input::LeftMousePress)
					{
						switch (aimingAt)
						{
						case 0:

							menuState = stateLoad;
							loadGameState = stateInGame;
							break;
						case 1:
							menuState = stateLoad;
							loadGameState = stateEditor;
							break;
						case 2:
							menuState = stateOptions;
							break;
						case 3:
							glfwSetWindowShouldClose(window, true);
							break;

						default:
							break;
						}
					}
					for (int i = 0; i < 4; i++)
					{
						menuDefault[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, i == aimingAt);
					}
					break;
				}
				case stateOptions:
				{
					optionsUpdate(menu, fontSh, basicSh, window, letters, eob, fontTex, TextBackGroundTex, transform, blockSize, cursorState, menuState);
					break;
				}
				case stateLoad:
				{
					for (int i = 0; i < 3; i++)
					{
						float vertices[4] =
						{
							menu.loadText[i].m_TextVertices[0] + menu.loadText[i].m_Transform[0]
							, menu.loadText[i].m_TextVertices[1] + menu.loadText[i].m_Transform[1]
							, menu.loadText[i].m_TextVertices[2] + menu.loadText[i].m_Transform[0]
							, menu.loadText[i].m_TextVertices[3] + menu.loadText[i].m_Transform[1]
						};
						if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
						{
							aimingAt = i;
							break;
						}
					}
					float vertices[4] =
					{
						menu.backText.m_TextVertices[0] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[1] + menu.backText.m_Transform[1]
						, menu.backText.m_TextVertices[2] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[3] + menu.backText.m_Transform[1]
					};
					if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
					{
						aimingAt = 3;
					}
					if (Input::LeftMousePress)
					{
						switch (aimingAt)
						{
						case 0:
						case 2:
						case 1:
							pathToSave += std::to_string(aimingAt) + "/";
							gameState = loadGameState;
							break;
						case 3:
							menuState = stateDefault;
							break;
						}
					}
					for (int i = 0; i < 3; i++)
					{
						menu.loadText[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, i == aimingAt);
					}
					menu.backText.Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, aimingAt == 3);
					break;
				}
				}


				printFPSTimer += deltaTime;
				if (printFPSTimer > 0.1f)
				{
					printFPSTimer = 0;
					oldDeltaTime = deltaTime;
				}
				numberSh.Bind();
				ErrorGL(glBindVertexArray(fontDrawData));
				ErrorGL(glBindTexture(GL_TEXTURE_2D, numberTexture));
				drawFloat(0, 0, 1.0f / oldDeltaTime, numberTexture, fontDrawData, dotTex, dotDD, scale, transform, numberSh);

				basicSh.Bind();
				DrawCursor(cursorTextures, cursorState, cursorDD, basicSh, transform, camera);
				Input::EndOfLoop();
				glfwSwapBuffers(window);
				glfwPollEvents();
			}

			for (int i = 0; i < 4; i++)
			{
				menuDefault[i].deleteText();
			}
		
			ErrorGL(glDeleteTextures(1, &backGroundTex));

			ErrorGL(glDeleteBuffers(1, &backGroundVBO));

			ErrorGL(glDeleteVertexArrays(1, &backGroundDD));

			
			break;
		}
		case stateInGame:
		{
			
			Shader HUDSh("res/shaders/verHUD.txt", "res/shaders/fragHUD.txt");
			HUDSh.Bind();
			HUDSh.GetUniformLocation("HUDCamera");
			HUDSh.GetUniformLocation("HUDTransform");
			HUDSh.GetUniformLocation("HUDScale");
			HUDSh.GetUniformLocation("HUDBasicLocation");
			HUDSh.GetUniformLocation("shadow");
			HUDSh.GetUniformLocation("craftingY");
			HUDSh.SetUniform1f(HUDSize + HUDCraftingY, 0);
			Shader backgroundSh("res/shaders/verBasic.txt", "res/shaders/fragBackground.txt");
			backgroundSh.Bind();
			backgroundSh.GetUniformLocation("camera");
			backgroundSh.GetUniformLocation("transform");
			backgroundSh.GetUniformLocation("blendFactor");
			Shader shadowSh("res/shaders/verBasic.txt", "res/shaders/fragShadow.txt");
			shadowSh.Bind();
			shadowSh.GetUniformLocation("camera");
			shadowSh.GetUniformLocation("transform");
			shadowSh.GetUniformLocation("shadow");
			Shader CrownSh("res/shaders/verTree.txt", "res/shaders/fragBasic.txt");
			CrownSh.Bind();
			CrownSh.GetUniformLocation("CrownCamera");
			CrownSh.GetUniformLocation("CrownTransform");
			CrownSh.GetUniformLocation("CrownRotation");
			Shader animSh("res/shaders/verAnimation.txt", "res/shaders/fragHUD.txt");
			animSh.Bind();
			animSh.GetUniformLocation("animCamera");
			animSh.GetUniformLocation("animTransform");
			animSh.GetUniformLocation("animScale");
			animSh.GetUniformLocation("animNumber");
			animSh.GetUniformLocation("animLeangth");
			animSh.GetUniformLocation("shadow");
			animSh.GetUniformLocation("craftingY");
			animSh.SetUniform1f(animSize + HUDCraftingY, 0);
			animSh.SetUniform1i(animSize + HUDShadow, 0);
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
			menuState = stateNone;
			ChangeTransform(-0.5f, -0.5f, transform);
			Shader lightMapSh("res/shaders/verBasic.txt", "res/shaders/fragLightMap.txt");
			lightMapSh.Bind();
			lightMapSh.GetUniformLocation("camera");
			lightMapSh.GetUniformLocation("transform");
			lightMapSh.SetUniformMat4(basicTransform,transform);
			CreateScale(1, 1, scale);
			CreateRotation(0, rotation);
			ChangeTransform(HANDOFFSETX, HANDOFFSETY, transform);
			ChangeCamera(0, Window::width, 0, Window::height, camera);
			advancedSh.Bind();
			advancedSh.SetUniform1i(advancedSize + ShadowLocation, 0);
			advancedSh.SetUniformMat4(advancedRotation, rotation);
			handSh.Bind();
			handSh.SetUniformMat4(handBeginTransform, transform);
			CrownSh.Bind();
			CrownSh.SetUniformMat4(CrownRotation, rotation);
			HUDSh.Bind();
			HUDSh.SetUniformMat4(HUDCamera, camera);
			numberSh.Bind();
			numberSh.SetUniformMat4(numberCamera, camera);
			ChangeCamera(-Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform, camera);

			unsigned int enemiesTexs[enemySize];
			unsigned int enemiesDDs[enemySize];

			enemiesTexs[en_Ghost] = CreateTextureRGBA("res/textures/ghost.png");
			enemiesDDs[en_Ghost] = CreateDrawData(eob, 1.5f, -1.5f, -1.5f, 1.5f, 1, 0, 0, 1.0f / 2.0f);


			enemiesTexs[en_Zombie] = CreateTextureRGBA("res/textures/zombieAnim.png");
			enemiesDDs[en_Zombie] = CreateDrawData(eob, 1.5f, -1.5f, -1, 1, 1, 0, 0, 1.0f / 5.0f);

			enemiesTexs[en_Mummy] = CreateTextureRGBA("res/textures/mummyAnim.png");
			enemiesDDs[en_Mummy] = enemiesDDs[en_Zombie];

			enemiesTexs[en_Slime] = CreateTextureRGBA("res/textures/slimeAnim.png");
			enemiesDDs[en_Slime] = CreateDrawData(eob, 1, -1, -1, 1, 1, 0, 0, 1.0f / 2.0f);
			
			enemiesTexs[en_FrostSlime] = CreateTextureRGBA("res/textures/FrostSlimeAnim.png");
			enemiesDDs[en_FrostSlime] = enemiesDDs[en_Slime];

			enemiesTexs[en_SandSlime] = CreateTextureRGBA("res/textures/sandSlimeAnim.png");
			enemiesDDs[en_SandSlime] = enemiesDDs[en_Slime];


			enemiesDDs[en_Skeleton] = enemiesDDs[en_Zombie];
			enemiesTexs[en_Skeleton] = CreateTextureRGBA("res/textures/skeletonAnim.png");

			enemiesTexs[en_Imp] = CreateTextureRGBA("res/textures/ImpAnim.png");
			enemiesDDs[en_Imp] = CreateDrawData(eob, 1.25f, -1.25f, -1, 1, 1, 0, 0, 1.0f / 5.0f);

			enemiesTexs[en_Imp] = CreateTextureRGBA("res/textures/ImpAnim.png");
			enemiesDDs[en_Imp] = CreateDrawData(eob, 1.25f, -1.25f, -1, 1, 1, 0, 0, 1.0f / 5.0f);


			enemiesTexs[en_Ghost] = CreateTextureRGBA("res/textures/ImpAnim.png");
			enemiesDDs[en_Ghost] = enemiesDDs[en_Slime];


			unsigned int blockTextures[t_BlocksSize];
			unsigned int projectilesTex[p_Size];
			unsigned int projectilesDD[p_Size];

			for (int i = 0; i < p_Size; i++)
			{
				projectilesDD[i] = blocksDrawData;
			}
			projectilesDD[p_BasicBullet] = CreateDrawData(eob, 0.3f, -0.3f, 0.2f, -0.2f);
			projectilesDD[p_FireBullet] = projectilesDD[p_BasicBullet];
			projectilesDD[p_BouncingBullet] = projectilesDD[p_BasicBullet];
			projectilesDD[p_PierceBullet] = projectilesDD[p_BasicBullet];



			projectilesTex[p_FrostSpike] = CreateTextureRGBA("res/textures/frostSpike.png");
			
			projectilesTex[p_BoneArrow] = CreateTextureRGBA("res/textures/boneArrow.png");

			projectilesTex[p_FireBall] = CreateTextureRGBA("res/textures/fireBall.png");





			unsigned int skeletonHandTex = CreateTextureRGBA("res/textures/skeletonHand.png");
			unsigned int skeletonHandDD = CreateDrawData(eob, 1.8f, 0, 0.9f, -0.9f);



			unsigned int impHandTex = CreateTextureRGBA("res/textures/impHand.png");
			unsigned int impHandDD = CreateDrawData(eob, 1.25f, 0, 0.15f, -0.15f);


			unsigned int CrownTextures[4];
			unsigned int CutTextures[4];
			unsigned int structuresTextures[s_StructureSize];
			unsigned int DoorTextures[2] = { CreateTextureRGBA("res/textures/CloseDoor.png"), CreateTextureRGBA("res/textures/openDoor.png") };
			unsigned int trapDoorTextures[2] = { CreateTextureRGBA("res/textures/CloseTrapDoor.png"), CreateTextureRGBA("res/textures/OpenTrapDoor.png") };
			unsigned int openChestTex = CreateTextureRGBA("res/textures/OpenChest.png");
			unsigned int damageTexture[2] = { CreateTextureRGBA("res/textures/DamageBlock.png"), CreateTextureRGBA("res/textures/lightDamageBlock.png") };
			

			CutTextures[0] = CreateTextureRGBA("res/textures/cut4.png");
			CutTextures[1] = CreateTextureRGBA("res/textures/cut3.png");
			CutTextures[2] = CreateTextureRGBA("res/textures/cut2.png");
			CutTextures[3] = CreateTextureRGBA("res/textures/cut1.png");

			structuresTextures[s_ForestSapling] = CreateTextureRGBA("res/textures/sapling.png");
			structuresTextures[s_SnowSapling] = CreateTextureRGBA("res/textures/saplingSnow.png");
			structuresTextures[s_CactusSapling] = CreateTextureRGBA("res/textures/saplingCactus.png");
			structuresTextures[s_Chest] = CreateTextureRGBA("res/textures/Chest.png");
			structuresTextures[s_CraftingTable] = CreateTextureRGBA("res/textures/bench.png");
			structuresTextures[s_Forge] = CreateTextureRGBA("res/textures/forge.png");
			structuresTextures[s_Anvil] = CreateTextureRGBA("res/textures/anvil.png");
			structuresTextures[s_Lathe] = CreateTextureRGBA("res/textures/lathe.png");
			structuresTextures[s_WorkBench] = CreateTextureRGBA("res/textures/workBench.png");
			structuresTextures[s_AlchemyTable] = CreateTextureRGBA("res/textures/alchemyTable.png");
			structuresTextures[s_Door] = DoorTextures[0];
			structuresTextures[s_TrapDoor] = trapDoorTextures[0];
			structuresTextures[s_Gate] = CreateTextureRGBA("res/textures/CloseGate.png");
			CrownTextures[crown_Forest] = CreateTextureRGBA("res/textures/forestBush.png");
			CrownTextures[crown_ForestSmall] = CreateTextureRGBA("res/textures/forestSmallBush.png");
			CrownTextures[crown_Snow] = CreateTextureRGBA("res/textures/snowBush.png");
			CrownTextures[crown_SnowSmall] = CreateTextureRGBA("res/textures/snowSmallBush.png");
			unsigned int CrownDD[4];
			unsigned int structuresDD[s_StructureSize];
			unsigned int alternativeBlockDD = CreateDrawData(eob, 0.5f, -0.5f, 0.5f, -0.5f, 1, 0, TEXSLOTDISTANCE, 0);

			structuresDD[s_ForestSapling] = CreateDrawData(eob, 1.5f, -0.5f, 0.5f, -0.5f);
			structuresDD[s_CactusSapling] = structuresDD[s_ForestSapling];
			structuresDD[s_SnowSapling]  = structuresDD[s_ForestSapling];
			structuresDD[s_Chest] = CreateDrawData(eob, 1.5f, -0.5f, 1.5f, -0.5f);
			structuresDD[s_AlchemyTable] = structuresDD[s_Chest];
			structuresDD[s_CraftingTable] = CreateDrawData(eob, 0.5f, -0.5f, 1.5f, -0.5f);
			structuresDD[s_Forge] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -0.5f);
			structuresDD[s_Lathe] = CreateDrawData(eob, 1.5f, -0.5f, 2.5f, -0.5f);
			structuresDD[s_WorkBench] = structuresDD[s_Lathe];	
			structuresDD[s_Anvil] = structuresDD[s_CraftingTable];
			structuresDD[s_Door] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -1.5f);
			structuresDD[s_TrapDoor] = CreateDrawData(eob, 1.5f, -1.5f, 1.5f, -0.5f);
			structuresDD[s_Gate] = CreateDrawData(eob, 3.5f, -0.5f, 0.5f, -0.5f);
			CrownDD[crown_Forest] = CreateDrawData(eob, 4.5f, -0.5f, 3.5f, -3.5f);
			CrownDD[crown_ForestSmall] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -1.5f);
			CrownDD[crown_Snow] = CrownDD[crown_Forest];
			CrownDD[crown_SnowSmall] = CrownDD[crown_ForestSmall];



			CreateAllBlockTextures(blockTextures);
			

			Text menuTexts[6];
			unsigned int menuBackgroundVBO;
			unsigned int menuBackgroundDD = CreateDrawData(eob,Window::height,0,Window::width,0,menuBackgroundVBO,1,0,TEXSLOTDISTANCE, 0);

			{
				std::string Texts[5] = { "Exit" ,"Options", "Save" , "Load",  "Resume" };
				for (int i = 0; i < 5; i++)
				{
					menuTexts[i].CreateText(Texts[i], std::vector<Format>{ Format(10, 8, 0, 0, 0, 1) }, letters, eob, middleBottom, Window::width / 2.0f, Window::height / 7.0f * (i + 1));
				}
				menuTexts[5].CreateText("Menu", std::vector<Format>{ Format(6, 4, 0, 0, 0, 1) }, letters, eob, rightBottom, Window::width, 0);

			}

			std::vector<DroppedItem> dropItems;


			std::vector<std::vector<Block>> blocks;
			std::vector<DamagedBlock> damagedBlocks;
			std::vector<std::vector<uint8_t>> Walls;
			std::vector<DamagedBlock> damagedWalls;
			std::vector<Crown> Crowns;
			std::vector<damagedWood> damagedCrowns;
			std::vector<seedling> seedlings;
			std::vector<int> isSandOnX;
			std::vector<int> chunksToRebuildBlock;
			std::vector < ChunkDD> blockChunks;
			{
				ChunkDD chunk;
				blockChunks.assign(2322, chunk);
			}
			std::vector<int> chunksToRebuildWall;
			std::vector < ChunkDD> wallChunks;
			{
				ChunkDD chunk;
				wallChunks.assign(2322, chunk);
			}
			std::vector<int> chunkToRebuildLightMap;
			std::vector<std::vector<float>> staticLightMap;
		
			std::vector<Enemy> enemies;
			std::vector<BoomParticle> boomParticles;
			std::vector<Projectile> projectiles;
			std::vector<CraftStation> craftStations;
			std::vector<Chest> chests;
			std::vector<Door> doors;
			Player player(eob, chests, letters, blockTextures, structuresTextures);

			if(!Load(pathToSave, blocks,isSandOnX))
			{
				std::cout << "error can not load blocks" << std::endl;
			}
			if (!Load(pathToSave, Walls))
			{
				std::cout << "error can not load walls" << std::endl;

			}
			if (!Load(pathToSave, blocks, craftStations, chests, doors, Crowns, seedlings, structuresTextures, CrownTextures, CrownDD))
			{
				std::cout << "error can not load structs" << std::endl;

			}
			if (!Load(pathToSave,player,damagedCrowns,damagedBlocks,damagedWalls,projectiles,enemies,dropItems, letters, eob))
			{
				std::cout << "error can not load player" << std::endl;

			}
			Background background(eob, backgroundSh);
			{
				std::vector<float> fill;				
				fill.assign(Blocks::yMax - Blocks::yMin+1, 0);
				staticLightMap.assign(Blocks::xMax, fill);
			}
			projectilesTex[p_Sand] = player.m_AllItemTextures[i_Sand];
			projectilesTex[p_FireArrow] = player.m_AllItemTextures[i_FireArrow];
			projectilesTex[p_PierceArrow] = player.m_AllItemTextures[i_PierceArrow];
			projectilesTex[p_BasicArrow] = player.m_AllItemTextures[i_BasicArrow];
			projectilesTex[p_BouncingArrow] = player.m_AllItemTextures[i_BouncingArrow];
			projectilesTex[p_BasicCannonBall] = player.m_AllItemTextures[i_BasicCannonBall];
			projectilesTex[p_PierceCannonBall] = player.m_AllItemTextures[i_PierceCannonBall];
			projectilesTex[p_BouncingCannonBall] = player.m_AllItemTextures[i_BouncingCannonBall];
			projectilesTex[p_FireCannonBall] = player.m_AllItemTextures[i_FireCannonBall];
			projectilesTex[p_BasicBullet] = player.m_AllItemTextures[i_BasicBullet];
			projectilesTex[p_PierceBullet] = player.m_AllItemTextures[i_PierceBullet];
			projectilesTex[p_BouncingBullet] = player.m_AllItemTextures[i_BouncingBullet];
			projectilesTex[p_FireBullet] = player.m_AllItemTextures[i_FireBullet];
	
			int lightMapSize[2] = {static_cast<int>(2 * ceil(Window::halfWidthOfGameTransform + 1)),static_cast<int>(2 * ceil(Window::halfHeightOfGameTransform + 1)) };
			unsigned int lightMapVBO = 0;
			unsigned int lightMapDD = CreateDrawData(eob, ceil(Window::halfHeightOfGameTransform + 1), -ceil(Window::halfHeightOfGameTransform + 1), ceil(Window::halfWidthOfGameTransform + 1), -ceil(Window::halfWidthOfGameTransform+1), lightMapVBO);
			CalculateLightMap( blocks, Walls, staticLightMap);
			unsigned int lightMap;
			ErrorGL(glGenTextures(1, &lightMap));
			ErrorGL(glBindTexture(GL_TEXTURE_2D, lightMap));

			ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			if (Window::smoothShadows)
			{
				ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
				ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
			}
			else
			{
				ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
				ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

			}
			CreateChunks(blockChunks, blocks);
			CreateChunks(wallChunks,Walls);
			Input::OffAllButtons();

			while (!glfwWindowShouldClose(window) && gameState == stateInGame)
			{

				glClear(GL_COLOR_BUFFER_BIT);

				deltaTime = Clamp(glfwGetTime() - pastTime,0,1);
				pastTime = glfwGetTime();
				assert(!GLCheakError(__FILE__,__LINE__));

				
				int newWidth, newHeight;
				glfwGetWindowSize(window, &newWidth, &newHeight);
				if (newHeight != Window::height || newWidth != Window::width)
				{
					Window::height = newHeight;
					Window::width = newWidth;
					ChangeScreenSize(newWidth, newHeight);
					{
						std::string Texts[5] = { "Exit" ,"Options", "Save" , "Load",  "Resume" };
						for (int i = 0; i < 5; i++)
						{
							menuTexts[i].CreateText(Texts[i], std::vector<Format>{ Format(10, 8, 0, 0, 0, 1) }, letters, eob, middleBottom, Window::width / 2.0f, Window::height / 7.0f * (i + 1));
						}
						menuTexts[5].CreateText("Menu", std::vector<Format>{ Format(6, 4, 0, 0, 0, 1) }, letters, eob, rightBottom, Window::width, 0);

					}
					ErrorGL(glDeleteBuffers(1, &menuBackgroundVBO));
					ErrorGL(glDeleteVertexArrays(1, &menuBackgroundDD));
					menuBackgroundDD = CreateDrawData(eob, Window::height, 0, Window::width, 0, menuBackgroundVBO, 1, 0, TEXSLOTDISTANCE, 0);

					CreateMenu(false, letters, menu, sliderDD, sliderVBO, checkBoxTex, sliderTex, trailTex, eob);
					ChangeCamera(0, Window::width, 0, Window::height, camera);
					HUDSh.Bind();
					HUDSh.SetUniformMat4(HUDCamera, camera);
					numberSh.Bind();
					numberSh.SetUniformMat4(numberCamera, camera);
					fontSh.Bind();
					fontSh.SetUniformMat4(fontCamera, camera);
					player.ResizeHUD(eob, chests, letters);

				}
				Input::XMousePos = Clamp(Input::XMousePos, -Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform);
				Input::YMousePos = Clamp(Input::YMousePos, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform);

				float CameraCoordinates[2];
				CameraCoordinates[0] = CameraHitboxX(player.m_Transform[0]);
				CameraCoordinates[1] = CameraHitboxY(player.m_Transform[1]);

				EnemySpawnManager(deltaTime, spawnTimer, eob, CameraCoordinates, blocks, enemies);

				for (int i = 0; i < seedlings.size(); i++)
				{
					if (seedlings.at(i).everyFrame(deltaTime,chunksToRebuildBlock, blocks, seedlings, Crowns))
					{
						seedlings.erase(seedlings.begin() + i);
					}

				}

				for (int i = 0; i < enemies.size(); i++)
				{
					int damage = enemies.at(i).EnemyEveryFrame(deltaTime, projectiles, blocks, player.m_Transform,player.m_Velocity);
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
				bool escapePressed = Input::EscapePress;
				bool leftMousePressed = Input::LeftMousePress;
				bool leftMouseReleased = Input::LeftMouseRelease;
				bool leftMousehold = Input::LeftMouseHold;
				if (menuState != stateNone)
				{
					Input::OffAllButtons();

				}


				player.EveryFrame(deltaTime, chunksToRebuildBlock, blocks, chunksToRebuildWall, Walls, enemies, isSandOnX, craftStations, damagedCrowns, damagedBlocks, damagedWalls, letters, CameraCoordinates, blocksDrawData, eob, blockTextures, structuresTextures, Crowns, seedlings, dropItems, projectiles, doors, chests);

				{
					int damageToplayer = ProjectileUpdate(deltaTime, projectiles, enemies, blocks, Walls, craftStations, seedlings, Crowns, dropItems, boomParticles, doors, chests, isSandOnX, chunksToRebuildBlock,player.m_Transform, blockTextures);
					if(damageToplayer)
					{
						player.DamagePlayer(NULL, damageToplayer);
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
				else if (damagedCrowns.size() > 20)
				{
					damagedCrowns.erase(damagedCrowns.begin());
				}
				for (int i = 0; i < damagedBlocks.size(); i++)
				{
					if (blocks.at(damagedBlocks.at(i).m_Transform[0]).at( damagedBlocks.at(i).m_Transform[1] - Blocks::yMin).m_Type == t_Air)
					{
						damagedBlocks.erase(damagedBlocks.begin() + i);
					}
				}
				for (int j = 0; j < damagedCrowns.size(); j++)
				{
					bool ExistenceOfWood = false;
				
				
					if (blocks.at(damagedCrowns.at(j).m_Transform[0]).at(damagedCrowns.at(j).m_Transform[1]- Blocks::yMin).m_Type != t_Air)
					{
						ExistenceOfWood = true;
					}

					if (!ExistenceOfWood)
					{
						damagedCrowns.erase(damagedCrowns.begin() + j);
					}
				}

				if (menuState != stateNone)
				{
					Input::EscapePress = escapePressed;
					Input::LeftMousePress = leftMousePressed;
					Input::LeftMouseHold = leftMousehold;
					Input::LeftMouseRelease = leftMouseReleased;
				}
				CameraCoordinates[0] = CameraHitboxX(player.m_Transform[0]);
				CameraCoordinates[1] = CameraHitboxY(player.m_Transform[1]);
				{
					float pixelPerUnit =  Window::width/ (Window::halfWidthOfGameTransform * 2);
					CameraCoordinates[0] = floor(CameraCoordinates[0] * pixelPerUnit) / pixelPerUnit;
					CameraCoordinates[1] = floor(CameraCoordinates[1] * pixelPerUnit) / pixelPerUnit;

				}
				ChangeCamera(-Window::halfWidthOfGameTransform + CameraCoordinates[0], Window::halfWidthOfGameTransform + CameraCoordinates[0], -Window::halfHeightOfGameTransform + CameraCoordinates[1], Window::halfHeightOfGameTransform + CameraCoordinates[1], camera);

				
				animSh.Bind();
				animSh.SetUniformMat4(animCamera, camera);
				basicSh.Bind();
				basicSh.SetUniformMat4(basicCamera, camera);
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
				lightMapSh.Bind();
				lightMapSh.SetUniformMat4(basicCamera, camera);
				shadowSh.Bind();
				shadowSh.SetUniformMat4(basicCamera, camera);
				DrawChunks(shadowSh, blockTextures, transform, CameraCoordinates, blockChunks, wallChunks);
				basicSh.Bind();
				ErrorGL(glBindVertexArray(blocksDrawData));
				for (int i = 0; i < damagedBlocks.size(); i++)
				{
					damagedBlocks.at(i).DrawDamage(basicSh, transform, damageTexture);
				}
				ErrorGL(glBindVertexArray(structuresDD[s_ForestSapling]));
				for (int i = 0; i < seedlings.size(); i++)
				{
					seedlings.at(i).drawSeedling(basicSh, transform);
				}
				CrownSh.Bind();
				CrownSh.SetUniformMat4(CrownCamera, camera);
				
				DrawCrowns(CrownSh,Crowns,CrownDD,CrownTextures, CameraCoordinates, transform, rotation);
				
				ErrorGL(glBindVertexArray(blocksDrawData));
				for (int i = 0; i < damagedCrowns.size(); i++)
				{
					damagedCrowns.at(i).DrawCut(CrownSh, rotation, transform, CutTextures);
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
					enemies.at(i).DrawEnemy(animSh,handSh,enemiesTexs,enemiesDDs,skeletonHandTex,skeletonHandDD,impHandTex,impHandDD,player.m_Transform, transform, scale, rotation);
				}

				particlesSh.Bind();
				ErrorGL(glBindVertexArray(particlesDD));
				for (int i = 0; i < enemies.size(); i++)
				{
					if (enemies.at(i).m_TypeOfEnemy == en_SandSlime)
					{
						enemies.at(i).m_OnFire.DrawParticles(particlesSh, deltaTime, true, enemies.at(i).m_Transform, transform);
					}
					else
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

				}

				advancedSh.Bind();
				for (int i = 0; i < projectiles.size(); i++)
				{
					projectiles.at(i).Draw(advancedSh,projectilesDD,projectilesTex, transform, scale, rotation);
				}

				player.DrawPlayer(deltaTime, basicSh, animSh, handSh, particlesSh, transform, scale, rotation, camera, particlesDD);
				if (Window::height)
				{
					lightMapSh.Bind();
					if (lightMapSize[0] != 2 * ceil(Window::halfWidthOfGameTransform + 1) || lightMapSize[1] != 2 * ceil(Window::halfHeightOfGameTransform + 1))
					{

						ErrorGL(glDeleteBuffers(1, &lightMapVBO));

						ErrorGL(glDeleteVertexArrays(1, &lightMapDD));

						lightMapSize[0] = 2.0f * ceil(Window::halfWidthOfGameTransform + 1);
						lightMapSize[1] = 2.0f * ceil(Window::halfHeightOfGameTransform + 1);
						lightMapDD = CreateDrawData(eob, ceil(Window::halfHeightOfGameTransform + 1), -ceil(Window::halfHeightOfGameTransform + 1), ceil(Window::halfWidthOfGameTransform + 1), -ceil(Window::halfWidthOfGameTransform + 1), lightMapVBO);

					}

					CreateLightMap(staticLightMap, blocks,projectiles, player.m_Transform, player.m_PlayerSlots[0], round(CameraCoordinates[0]) - ceil(Window::halfWidthOfGameTransform + 1), round(CameraCoordinates[1]) - ceil(Window::halfHeightOfGameTransform + 1), 2 * ceil(Window::halfWidthOfGameTransform + 1), 2 * ceil(Window::halfHeightOfGameTransform + 1), lightMap);
					ChangeTransform(round(CameraCoordinates[0]) - 0.5f, round(CameraCoordinates[1]) - 0.5f, transform);
					lightMapSh.SetUniformMat4(basicTransform, transform);
					ErrorGL(glBindTexture(GL_TEXTURE_2D, lightMap));
					ErrorGL(glBindVertexArray(lightMapDD));
					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
				}
				player.DrawPlayerHUD(basicSh, HUDSh, numberSh, fontSh, animSh, chests, transform, scale, rotation, camera, fontTex, fontDrawData, numberTexture);
				if (player.m_IsInventoryOpen && menuState != stateNone)
				{
					animSh.Bind();
					ChangeTransform(0, 0, transform);
					animSh.SetUniformMat4(animTransform, transform);
					animSh.SetUniform1f(animSize + HUDCraftingY, 2);
					animSh.SetUniform1i(animNumber, 0);
					ErrorGL(glBindVertexArray(menuBackgroundDD));
					ErrorGL(glBindTexture(GL_TEXTURE_2D, player.m_SlotTextures));

					ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));



				}

				basicSh.Bind();
				ChangeCamera(0, Window::width, 0, Window::height, camera);
				basicSh.SetUniformMat4(basicCamera, camera);

				fontSh.Bind();
				ErrorGL(glBindTexture(GL_TEXTURE_2D, fontTex));
				int cursorState = canNotDoIt;

				switch (menuState)
				{
				case stateNone:
				{
					if (player.m_IsInventoryOpen)
					{
						float vertices[4] =
						{
							menuTexts[5].m_TextVertices[0] + menuTexts[5].m_Transform[0]
							, menuTexts[5].m_TextVertices[1] + menuTexts[5].m_Transform[1]
							, menuTexts[5].m_TextVertices[2] + menuTexts[5].m_Transform[0]
							, menuTexts[5].m_TextVertices[3] + menuTexts[5].m_Transform[1]
						};

						if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
						{

							menuTexts[5].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, true);
							if (Input::LeftMousePress)
							{
								menuState = stateDefault;
							}
						}
						else
						{
							menuTexts[5].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, false);
						}
					}
					break;
				}
				case stateDefault:
				{
					if (Input::EscapePress)
					{
						menuState = stateNone;
						break;
					}
					for (int i = 0; i < 5; i++)
					{
						float vertices[4] =
						{
							menuTexts[i].m_TextVertices[0] + menuTexts[i].m_Transform[0]
							, menuTexts[i].m_TextVertices[1] + menuTexts[i].m_Transform[1]
							, menuTexts[i].m_TextVertices[2] + menuTexts[i].m_Transform[0]
							, menuTexts[i].m_TextVertices[3] + menuTexts[i].m_Transform[1]
						};

						if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
						{
							menuTexts[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, true);
							if (Input::LeftMousePress)
							{
								switch (i)
								{

								case 0:
									glfwSetWindowShouldClose(window, true);
									break;
								case 1:
									menuState = stateOptions;
									break;
								case 2:
									menuState = stateSave;
									break;
								case 3:
									menuState = stateLoad;
									break;
								case 4:
									menuState = stateNone;
									break;
								}
							}
						}
						else
						{
							menuTexts[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, false);
						}
					}

					break;
				}
				case stateOptions:
				{
					if (Input::EscapePress)
					{
						menuState = stateNone;
						break;
					}
					optionsUpdate(menu, fontSh, basicSh, window, letters, eob, fontTex, TextBackGroundTex, transform, blockSize, cursorState, menuState);
					break;
				}
				case stateLoad:
				{
					int aimingAt = -1;
					for (int i = 0; i < 3; i++)
					{
						float vertices[4] =
						{
							menu.loadText[i].m_TextVertices[0] + menu.loadText[i].m_Transform[0]
							, menu.loadText[i].m_TextVertices[1] + menu.loadText[i].m_Transform[1]
							, menu.loadText[i].m_TextVertices[2] + menu.loadText[i].m_Transform[0]
							, menu.loadText[i].m_TextVertices[3] + menu.loadText[i].m_Transform[1]
						};
						if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
						{
							aimingAt = i;
							cursorState = canClickOnIt;
							break;
						}
					}
					float vertices[4] =
					{
						menu.backText.m_TextVertices[0] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[1] + menu.backText.m_Transform[1]
						, menu.backText.m_TextVertices[2] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[3] + menu.backText.m_Transform[1]
					};
					if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
					{
						cursorState = canClickOnIt;
						aimingAt = 3;
					}
					if (Input::LeftMousePress)
					{
						switch (aimingAt)
						{
						case 0:
						case 1:
						case 2:
							pathToSave = "res/save" + std::to_string(aimingAt) + "/";
							if (!Load(pathToSave, blocks, isSandOnX))
							{
								std::cout << "error can not load (blocks)" << std::endl;
							}
							if (!Load(pathToSave, Walls))
							{
								std::cout << "error can not load (walls)" << std::endl;
							}
							if (!Load(pathToSave, blocks, craftStations, chests, doors, Crowns, seedlings, structuresTextures, CrownTextures, CrownDD))
							{
								std::cout << "error can not load (struct)" << std::endl;
							}
							if (!Load(pathToSave, player, damagedCrowns, damagedBlocks, damagedWalls, projectiles, enemies, dropItems, letters, eob))
							{
								std::cout << "error can not load player" << std::endl;

							}
							CreateChunks(blockChunks, blocks);
							CreateChunks(wallChunks, Walls);
							CalculateLightMap( blocks, Walls, staticLightMap);
							break;
						case 3:
							menuState = stateDefault;
							break;
						}
					}
					if (Input::EscapePress)
					{
						menuState = stateNone;
						break;
					}
					fontSh.Bind();
					ErrorGL(glBindTexture(GL_TEXTURE_2D, fontTex));

					for (int i = 0; i < 3; i++)
					{
						menu.loadText[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, aimingAt == i);
					}
					menu.backText.Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, aimingAt == 3);
					break;

				}
				case stateSave:
				{
					int aimingAt = -1;
					for (int i = 0; i < 3; i++)
					{
						float vertices[4] =
						{
							menu.saveText[i].m_TextVertices[0] + menu.saveText[i].m_Transform[0]
							, menu.saveText[i].m_TextVertices[1] + menu.saveText[i].m_Transform[1]
							, menu.saveText[i].m_TextVertices[2] + menu.saveText[i].m_Transform[0]
							, menu.saveText[i].m_TextVertices[3] + menu.saveText[i].m_Transform[1]
						};
						if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
						{
							aimingAt = i;
							cursorState = canClickOnIt;
							break;
						}
					}
					float vertices[4] =
					{
						menu.backText.m_TextVertices[0] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[1] + menu.backText.m_Transform[1]
						, menu.backText.m_TextVertices[2] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[3] + menu.backText.m_Transform[1]
					};
					if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
					{
						cursorState = canClickOnIt;
						aimingAt = 3;
					}
					if (Input::LeftMousePress)
					{
						switch (aimingAt)
						{
						case 0:
						case 1:
						case 2:
							pathToSave = "res/save" + std::to_string(aimingAt) + "/";
							if (!Save(pathToSave, blocks))
							{
								std::cout << "error can not make save (blocks)" << std::endl;
							}
							if (!Save(pathToSave, Walls))
							{
								std::cout << "error can not make save (walls)" << std::endl;
							}
							if (!Save(pathToSave, craftStations, chests, doors, Crowns, seedlings))
							{
								std::cout << "error can not make save (struct)" << std::endl;
							}
							if (!Save(pathToSave, player))
							{
								std::cout << "error can not make save (player)" << std::endl;
							}
							break;
						case 3:
							menuState = stateDefault;
							break;
						}
					}
					if (Input::EscapePress)
					{
						menuState = stateNone;
						break;
					}
					fontSh.Bind();
					ErrorGL(glBindTexture(GL_TEXTURE_2D, fontTex));

					for (int i = 0; i < 3; i++)
					{
						menu.saveText[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, aimingAt == i);
					}
					menu.backText.Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, aimingAt == 3);
					break;
					break;
				}
				}
				if (Input::F5Press)
				{
					if (!Save(pathToSave, blocks))
					{
						std::cout << "error can not make save (blocks)" << std::endl;
					}
					if (!Save(pathToSave, Walls))
					{
						std::cout << "error can not make save (walls)" << std::endl;
					}
					if (!Save(pathToSave, craftStations, chests, doors, Crowns, seedlings))
					{
						std::cout << "error can not make save (struct)" << std::endl;
					}
					if (!Save(pathToSave, player))
					{
						std::cout << "error can not make save (struct)" << std::endl;
					}
				}
				else if (Input::F8Press)
				{
					if (!Load(pathToSave, blocks, isSandOnX))
					{
						std::cout << "error can not load (blocks)" << std::endl;
					}
					if (!Load(pathToSave, Walls))
					{
						std::cout << "error can not load (walls)" << std::endl;
					}
					if (!Load(pathToSave, blocks, craftStations, chests, doors, Crowns, seedlings, structuresTextures, CrownTextures, CrownDD))
					{
						std::cout << "error can not load (struct)" << std::endl;
					}
					if (!Load(pathToSave, player, damagedCrowns, damagedBlocks, damagedWalls, projectiles, enemies, dropItems, letters, eob))
					{
						std::cout << "error can not load player" << std::endl;

					}
					CreateChunks(blockChunks, blocks);
					CreateChunks(wallChunks, Walls);
					CalculateLightMap( blocks, Walls, staticLightMap);
				}

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
				if (menuState == stateNone)
				{
					animSh.Bind();
					ChangeCamera(-Window::halfWidthOfGameTransform + CameraCoordinates[0], Window::halfWidthOfGameTransform + CameraCoordinates[0], -Window::halfHeightOfGameTransform + CameraCoordinates[1], Window::halfHeightOfGameTransform + CameraCoordinates[1], camera);
					animSh.SetUniformMat4(animCamera, camera);
					DrawCursor(cursorTextures, structuresTextures, structuresDD, cursorDD, alternativeBlockDD, shadowSh, structureSh, numberSh, animSh, transform, camera, scale, fontDrawData, numberTexture, player, CameraCoordinates);
				}
				else
				{

					basicSh.Bind();
					DrawCursor(cursorTextures, cursorState, cursorDD, basicSh, transform, camera);
				}
				if (chunksToRebuildBlock.size())
				{
					SandEveryFrame(isSandOnX, projectiles, blocks, Walls, chunksToRebuildBlock);
					CheckFloorCraftStations(craftStations, blocks, dropItems);
					for (int i = 0; i < doors.size(); i++)
					{
						if (doors.at(i).CheckFloorAndCeil(doors, blocks, dropItems))
						{
							dropItems.emplace_back(doors[i].m_Transform[0], doors[i].m_Transform[1], 0, GetItemIDByStructure(doors[i].m_Type), 1, true);
							doors.at(i).DestroyDoor(blocks, Walls, isSandOnX);
							doors.erase(i + doors.begin());
						}
					}
				}

				for (int i = 0; i < chunksToRebuildBlock.size();i++)
				{
					bool alreadyThere = false;
					for (int j = 0; j < chunkToRebuildLightMap.size(); j++)
					{
						if (chunksToRebuildBlock.at(i) == chunkToRebuildLightMap.at(j))
						{
							alreadyThere = true;
						}
					}
					if (!alreadyThere)
					{
						chunkToRebuildLightMap.emplace_back(chunksToRebuildBlock.at(i));
					}
				}
				for (int i = 0; i < chunksToRebuildWall.size();i++)
				{
					bool alreadyThere = false;
					for (int j = 0; j < chunkToRebuildLightMap.size(); j++)
					{
						if (chunksToRebuildWall.at(i) == chunkToRebuildLightMap.at(j))
						{
							alreadyThere = true;
						}
					}
					if (!alreadyThere)
					{
						chunkToRebuildLightMap.emplace_back(chunksToRebuildWall.at(i));
					}
				}
				for (int i = 0; i < chunkToRebuildLightMap.size();i++)
				{
					CalculateLightMap(chunkToRebuildLightMap.at(i), blocks, Walls, staticLightMap);
				}
				chunkToRebuildLightMap.clear();


				CreateChunks(chunksToRebuildBlock, blockChunks, blocks);
				CreateChunks(chunksToRebuildWall, wallChunks, Walls);


				Input::EndOfLoop();
				glfwSwapBuffers(window);
				glfwPollEvents();

			}
			break;
		}
		case stateEditor:
		{
			Shader backgroundSh("res/shaders/verBasic.txt", "res/shaders/fragBackground.txt");
			backgroundSh.Bind();
			backgroundSh.GetUniformLocation("camera");
			backgroundSh.GetUniformLocation("transform");
			backgroundSh.GetUniformLocation("blendFactor");
			Shader shadowSh("res/shaders/verBasic.txt", "res/shaders/fragShadow.txt");
			shadowSh.Bind();
			shadowSh.GetUniformLocation("camera");
			shadowSh.GetUniformLocation("transform");
			shadowSh.GetUniformLocation("shadow");
			Shader CrownSh("res/shaders/verTree.txt", "res/shaders/fragBasic.txt");
			CrownSh.Bind();
			CrownSh.GetUniformLocation("CrownCamera");
			CrownSh.GetUniformLocation("CrownTransform");
			CrownSh.GetUniformLocation("CrownRotation");
			Shader structureSh("res/shaders/verBasic.txt", "res/shaders/fragStructures.txt");
			structureSh.Bind();
			structureSh.GetUniformLocation("camera");
			structureSh.GetUniformLocation("transform");
			structureSh.GetUniformLocation("shadow");
			structureSh.GetUniformLocation("lookAt");
			Shader advancedSh("res/shaders/verAdvanced.txt", "res/shaders/fragShadow.txt");
			advancedSh.Bind();
			advancedSh.GetUniformLocation("camera");
			advancedSh.GetUniformLocation("transform");
			advancedSh.GetUniformLocation("scale");
			advancedSh.GetUniformLocation("rotation");
			advancedSh.GetUniformLocation("shadow");
			Shader animSh("res/shaders/verAnimation.txt", "res/shaders/fragHUD.txt");
			animSh.Bind();
			animSh.GetUniformLocation("animCamera");
			animSh.GetUniformLocation("animTransform");
			animSh.GetUniformLocation("animScale");
			animSh.GetUniformLocation("animNumber");
			animSh.GetUniformLocation("animLeangth");
			animSh.GetUniformLocation("shadow");
			animSh.GetUniformLocation("craftingY");
			animSh.SetUniform1f(animSize + HUDCraftingY, 0);
			animSh.SetUniform1i(animSize + HUDShadow, 0);

			menuState = stateNone;
			Editor editor(eob);
			EditorHUD editorHUD( eob, TextBackGroundTex);

			CreateScale(1, 1, scale);
			CreateRotation(0, rotation);
			ChangeTransform(HANDOFFSETX, HANDOFFSETY, transform);
			ChangeCamera(0, Window::width, 0, Window::height, camera);
			advancedSh.Bind();
			advancedSh.SetUniform1i(advancedSize + ShadowLocation, 0);
			advancedSh.SetUniformMat4(advancedRotation, rotation);
			CrownSh.Bind();
			CrownSh.SetUniformMat4(CrownRotation, rotation);
			numberSh.Bind();
			numberSh.SetUniformMat4(numberCamera, camera);
			ChangeCamera(-Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform, camera);
			unsigned int enemiesTex1[enemySize];
			unsigned int enemiesTex2[enemySize];
			unsigned int enemiesDD1[enemySize];
			unsigned int enemiesDD2[enemySize];
			unsigned int blockTextures[t_BlocksSize];
			unsigned int CrownTextures[4];
			unsigned int CutTextures[4];
			unsigned int structuresTextures[s_StructureSize];
			unsigned int DoorTextures[2] = { CreateTextureRGBA("res/textures/CloseDoor.png"), CreateTextureRGBA("res/textures/openDoor.png") };
			unsigned int trapDoorTextures[2] = { CreateTextureRGBA("res/textures/CloseTrapDoor.png"), CreateTextureRGBA("res/textures/OpenTrapDoor.png") };
			unsigned int openChestTex = CreateTextureRGBA("res/textures/OpenChest.png");
			unsigned int damageTexture[2] = { CreateTextureRGBA("res/textures/DamageBlock.png"), CreateTextureRGBA("res/textures/lightDamageBlock.png") };
			CutTextures[0] = CreateTextureRGBA("res/textures/cut4.png");
			CutTextures[1] = CreateTextureRGBA("res/textures/cut3.png");
			CutTextures[2] = CreateTextureRGBA("res/textures/cut2.png");
			CutTextures[3] = CreateTextureRGBA("res/textures/cut1.png");

			structuresTextures[s_ForestSapling] = CreateTextureRGBA("res/textures/sapling.png");
			structuresTextures[s_SnowSapling] = CreateTextureRGBA("res/textures/saplingSnow.png");
			structuresTextures[s_CactusSapling] = CreateTextureRGBA("res/textures/saplingCactus.png");
			structuresTextures[s_Chest] = CreateTextureRGBA("res/textures/Chest.png");
			structuresTextures[s_CraftingTable] = CreateTextureRGBA("res/textures/bench.png");
			structuresTextures[s_Forge] = CreateTextureRGBA("res/textures/forge.png");
			structuresTextures[s_Anvil] = CreateTextureRGBA("res/textures/anvil.png");
			structuresTextures[s_Lathe] = CreateTextureRGBA("res/textures/lathe.png");
			structuresTextures[s_WorkBench] = CreateTextureRGBA("res/textures/workBench.png");
			structuresTextures[s_AlchemyTable] = CreateTextureRGBA("res/textures/alchemyTable.png");
			structuresTextures[s_Door] = DoorTextures[0];
			structuresTextures[s_TrapDoor] = trapDoorTextures[0];
			structuresTextures[s_Gate] = CreateTextureRGBA("res/textures/CloseGate.png");
			CrownTextures[crown_Forest] = CreateTextureRGBA("res/textures/forestBush.png");
			CrownTextures[crown_ForestSmall] = CreateTextureRGBA("res/textures/forestSmallBush.png");
			CrownTextures[crown_Snow] = CreateTextureRGBA("res/textures/snowBush.png");
			CrownTextures[crown_SnowSmall] = CreateTextureRGBA("res/textures/snowSmallBush.png");
			unsigned int CrownDD[4];
			unsigned int structuresDD[s_StructureSize];
			unsigned int alternativeBlockDD = CreateDrawData(eob, 0.5f, -0.5f, 0.5f, -0.5f, 1, 0, TEXSLOTDISTANCE, 0);

			structuresDD[s_ForestSapling] = CreateDrawData(eob, 1.5f, -0.5f, 0.5f, -0.5f);
			structuresDD[s_CactusSapling] = structuresDD[s_ForestSapling];
			structuresDD[s_SnowSapling] = structuresDD[s_ForestSapling];
			structuresDD[s_Chest] = CreateDrawData(eob, 1.5f, -0.5f, 1.5f, -0.5f);
			structuresDD[s_AlchemyTable] = structuresDD[s_Chest];
			structuresDD[s_CraftingTable] = CreateDrawData(eob, 0.5f, -0.5f, 1.5f, -0.5f);
			structuresDD[s_Forge] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -0.5f);
			structuresDD[s_Lathe] = CreateDrawData(eob, 1.5f, -0.5f, 2.5f, -0.5f);
			structuresDD[s_WorkBench] = structuresDD[s_Lathe];
			structuresDD[s_Anvil] = structuresDD[s_CraftingTable];
			structuresDD[s_Door] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -1.5f);
			structuresDD[s_TrapDoor] = CreateDrawData(eob, 1.5f, -1.5f, 1.5f, -0.5f);
			structuresDD[s_Gate] = CreateDrawData(eob, 3.5f, -0.5f, 0.5f, -0.5f);
			CrownDD[crown_Forest] = CreateDrawData(eob, 4.5f, -0.5f, 3.5f, -3.5f);
			CrownDD[crown_ForestSmall] = CreateDrawData(eob, 2.5f, -0.5f, 1.5f, -1.5f);
			CrownDD[crown_Snow] = CrownDD[crown_Forest];
			CrownDD[crown_SnowSmall] = CrownDD[crown_ForestSmall];


			CreateAllBlockTextures(blockTextures);


			unsigned int ItemTex[i_ItemSize];
			CreateAllItemTextures(ItemTex, blockTextures, structuresTextures);
			Text menuTexts[5];
			unsigned int menuBackgroundVBO;
			unsigned int menuBackgroundDD = CreateDrawData(eob, Window::height, 0, Window::width, 0, menuBackgroundVBO, 1, 0, TEXSLOTDISTANCE, 0);

			{
				std::string Texts[5] = { "Exit" ,"Options", "Save" , "Load",  "Resume" };
				for (int i = 0; i < 5; i++)
				{
					menuTexts[i].CreateText(Texts[i], std::vector<Format>{ Format(10, 8, 0, 0, 0, 1) }, letters, eob, middleBottom, Window::width / 2.0f, Window::height / 7.0f * (i + 1));
				}

			}

		
			std::vector<std::vector<Block>> blocks;
		
			std::vector<std::vector<uint8_t>> Walls;
		
			std::vector<Crown> Crowns;
		
			std::vector<seedling> seedlings;
			std::vector<int> isSandOnX;
			std::vector<int> chunksToRebuildBlock;
			std::vector < ChunkDD> blockChunks;
			{
				ChunkDD chunk;
				blockChunks.assign(2322, chunk);
			}
			std::vector<int> chunksToRebuildWall;
			std::vector < ChunkDD> wallChunks;
			{
				ChunkDD chunk;
				wallChunks.assign(2322, chunk);
			}
			std::vector<CraftStation> craftStations;
			std::vector<Chest> chests;
			std::vector<Door> doors;
			if (!Load(pathToSave, blocks, isSandOnX))
			{
				std::cout << "error loading (blocks)" << std::endl;
			}
			if (!Load(pathToSave, Walls))
			{
				std::cout << "error can not load walls" << std::endl;

			}
			if (!Load(pathToSave,blocks,craftStations,chests,doors,Crowns,seedlings,structuresTextures,CrownTextures,CrownDD))
			{
				std::cout << "error can not load structs" << std::endl;

			}
			
			Background background(eob, backgroundSh);
			CreateChunks(blockChunks, blocks);
			CreateChunks(wallChunks, Walls);

			while (!glfwWindowShouldClose(window) && gameState == stateEditor)
			{
				glClear(GL_COLOR_BUFFER_BIT);
				deltaTime = glfwGetTime() - pastTime;
				pastTime = glfwGetTime();
				int cursorState = canNotDoIt;
				assert(!GLCheakError(__FILE__,__LINE__));

				int newWidth, newHeight;
				glfwGetWindowSize(window, &newWidth, &newHeight);
				if (newHeight != Window::height || newWidth != Window::width)
				{
					Window::height = newHeight;
					Window::width = newWidth;
					ChangeScreenSize(newWidth, newHeight);
					{
						std::string Texts[5] = { "Exit" ,"Options", "Save" , "Load",  "Resume" };
						for (int i = 0; i < 5; i++)
						{
							menuTexts[i].CreateText(Texts[i], std::vector<Format>{ Format(10, 8, 0, 0, 0, 1) }, letters, eob, middleBottom, Window::width / 2.0f, Window::height / 7.0f * (i + 1));
						}

					}
					ErrorGL(glDeleteBuffers(1, &menuBackgroundVBO));
					ErrorGL(glDeleteVertexArrays(1, &menuBackgroundDD));
					menuBackgroundDD = CreateDrawData(eob, Window::height, 0, Window::width, 0, menuBackgroundVBO, 1, 0, TEXSLOTDISTANCE, 0);
					editorHUD.Create(eob,chests,letters,false);
					CreateMenu(false, letters, menu, sliderDD, sliderVBO, checkBoxTex, sliderTex, trailTex, eob);
					ChangeCamera(0, Window::width, 0, Window::height, camera);	
					numberSh.Bind();
					numberSh.SetUniformMat4(numberCamera, camera);
					fontSh.Bind();
					fontSh.SetUniformMat4(fontCamera, camera);

				}
				Input::XMousePos = Clamp(Input::XMousePos, -Window::halfWidthOfGameTransform, Window::halfWidthOfGameTransform);
				Input::YMousePos = Clamp(Input::YMousePos, -Window::halfHeightOfGameTransform, Window::halfHeightOfGameTransform);
				bool escapePressed = Input::EscapePress;
				bool leftMousePressed = Input::LeftMousePress;
				bool leftMousehold = Input::LeftMouseHold;
				bool leftMouseReleased = Input::LeftMouseRelease;

				if(menuState != stateNone)
				{ 
					Input::OffAllButtons();

				}
				cursorState = editorHUD.Update(deltaTime,eob,blockSize,letters,chests, editor);
				editor.Update(deltaTime, cursorState, blockTextures, structuresTextures,chunksToRebuildBlock, blocks,chunksToRebuildWall,Walls,seedlings,craftStations,chests,doors,isSandOnX);
				if (menuState != stateNone)
				{
					cursorState = canNotDoIt;
					Input::EscapePress   = escapePressed;
					Input::LeftMousePress = leftMousePressed;
					Input::LeftMouseRelease = leftMouseReleased;
					Input::LeftMouseHold = leftMousehold;
				}
				editor.m_Transform[0] = CameraHitboxX(editor.m_Transform[0]);
				editor.m_Transform[1] = CameraHitboxY(editor.m_Transform[1]);
				{
					float CameraCoordinates[2];
					float pixelPerUnit = Window::width / (Window::halfWidthOfGameTransform * 2);
					CameraCoordinates[0] = floor( editor.m_Transform[0] * pixelPerUnit) / pixelPerUnit;
					CameraCoordinates[1] = floor( editor.m_Transform[1] * pixelPerUnit) / pixelPerUnit;
			
					ChangeCamera(-Window::halfWidthOfGameTransform + CameraCoordinates[0], Window::halfWidthOfGameTransform + CameraCoordinates[0], -Window::halfHeightOfGameTransform + CameraCoordinates[1], Window::halfHeightOfGameTransform + CameraCoordinates[1], camera);

				}

				animSh.Bind();
				animSh.SetUniformMat4(animCamera, camera);
				basicSh.Bind();
				basicSh.SetUniformMat4(basicCamera, camera);
				
				advancedSh.Bind();
				advancedSh.SetUniformMat4(advancedCamera, camera);
				shadowSh.Bind();
				shadowSh.SetUniformMat4(basicCamera, camera);

				backgroundSh.Bind();
				backgroundSh.SetUniformMat4(basicCamera, camera);
				background.DrawBackground(backgroundSh, basicSh, transform, editor.m_Transform);
				structureSh.Bind();
				structureSh.SetUniformMat4(basicCamera, camera);



				shadowSh.Bind();
			
				DrawChunks(shadowSh, blockTextures, transform, editor.m_Transform, blockChunks,wallChunks);

				basicSh.Bind();
				ErrorGL(glBindVertexArray(structuresDD[s_ForestSapling]));
				for (int i = 0; i < seedlings.size(); i++)
				{
					seedlings.at(i).drawSeedling(basicSh, transform);
				}
				CrownSh.Bind();
				CrownSh.SetUniformMat4(CrownCamera, camera);
				
				DrawCrowns(CrownSh,Crowns,CrownDD,CrownTextures, editor.m_Transform, transform, rotation);
				
				ErrorGL(glBindVertexArray(blocksDrawData));
			
				DrawDoors(doors, advancedSh, structuresDD, structuresTextures, DoorTextures, trapDoorTextures, transform, scale, rotation);
				DrawCraftStations(craftStations, structureSh, transform, structuresDD, structuresTextures);
				DrawChests(chests, structureSh, transform, openChestTex, structuresDD, structuresTextures);
		
				editor.Draw(animSh, transform, scale);

				ChangeCamera(0, Window::width, 0, Window::height, camera);
				basicSh.Bind();
				basicSh.SetUniformMat4(basicCamera, camera);
				shadowSh.Bind();
				shadowSh.SetUniformMat4(basicCamera, camera);
				shadowSh.SetUniform1i(basicSize + ShadowLocation, 0);

				editorHUD.Draw(shadowSh,fontSh, editor,chests,fontTex, ItemTex, blockTextures, transform);
				fontSh.Bind();
				ErrorGL(glBindTexture(GL_TEXTURE_2D, fontTex));

				switch (menuState)
				{
				case stateNone:
				{
					if (Input::EscapePress)
					{
						menuState = stateDefault;
					}

					break;
				}
				case stateDefault:
				{
					if (Input::EscapePress)
					{
						menuState = stateNone;
						break;
					}
					for (int i = 0; i < 5; i++)
					{
						float vertices[4] =
						{
							menuTexts[i].m_TextVertices[0] + menuTexts[i].m_Transform[0]
							, menuTexts[i].m_TextVertices[1] + menuTexts[i].m_Transform[1]
							, menuTexts[i].m_TextVertices[2] + menuTexts[i].m_Transform[0]
							, menuTexts[i].m_TextVertices[3] + menuTexts[i].m_Transform[1]
						};

						if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
						{
							menuTexts[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, true);
							if (Input::LeftMousePress)
							{
								switch (i)
								{

								case 0:
									glfwSetWindowShouldClose(window, true);
									break;
								case 1:
									menuState = stateOptions;
									break;
								case 2:
									menuState = stateSave;
									break;
								case 3:
									menuState = stateLoad;
									break;
								case 4:
									menuState = stateNone;
									break;
								}
							}
						}
						else
						{
							menuTexts[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, false);
						}
					}

					break;
				}
				case stateOptions:
				{
					if (Input::EscapePress)
					{
						menuState = stateNone;
						break;
					}
					optionsUpdate(menu, fontSh, basicSh, window, letters, eob, fontTex, TextBackGroundTex, transform, blockSize, cursorState, menuState);
					break;
				}
				case stateLoad:
				{
					int aimingAt = -1;
					for (int i = 0; i < 3; i++)
					{
						float vertices[4] =
						{
							menu.loadText[i].m_TextVertices[0] + menu.loadText[i].m_Transform[0]
							, menu.loadText[i].m_TextVertices[1] + menu.loadText[i].m_Transform[1]
							, menu.loadText[i].m_TextVertices[2] + menu.loadText[i].m_Transform[0]
							, menu.loadText[i].m_TextVertices[3] + menu.loadText[i].m_Transform[1]
						};
						if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
						{
							aimingAt = i;
							cursorState = canClickOnIt;
							break;
						}
					}
					float vertices[4] =
					{
						menu.backText.m_TextVertices[0] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[1] + menu.backText.m_Transform[1]
						, menu.backText.m_TextVertices[2] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[3] + menu.backText.m_Transform[1]
					};
					if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
					{
						cursorState = canClickOnIt;
						aimingAt = 3;
					}
					if (Input::LeftMousePress)
					{
						switch (aimingAt)
						{
						case 0:
						case 1:
						case 2:
							pathToSave = "res/save" + std::to_string(aimingAt) + "/";
							if (!Load(pathToSave, blocks, isSandOnX))
							{
								std::cout << "error can not load (blocks)" << std::endl;
							}
							if (!Load(pathToSave, Walls))
							{
								std::cout << "error can not load (walls)" << std::endl;
							}
							if (!Load(pathToSave, blocks, craftStations, chests, doors, Crowns, seedlings, structuresTextures, CrownTextures, CrownDD))
							{
								std::cout << "error can not load (struct)" << std::endl;
							}
							CreateChunks(blockChunks, blocks);
							CreateChunks(wallChunks, Walls);
							break;
						case 3:
							menuState = stateDefault;
							break;
						}
					}
					if (Input::EscapePress)
					{
						menuState = stateNone;
						break;
					}
					fontSh.Bind();
					ErrorGL(glBindTexture(GL_TEXTURE_2D, fontTex));

					for (int i = 0; i < 3; i++)
					{
						menu.loadText[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, aimingAt == i);
					}
					menu.backText.Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, aimingAt == 3);
					break;

				}
				case stateSave:
				{
					int aimingAt = -1;
					for (int i = 0; i < 3; i++)
					{
						float vertices[4] =
						{
							menu.saveText[i].m_TextVertices[0] + menu.saveText[i].m_Transform[0]
							, menu.saveText[i].m_TextVertices[1] + menu.saveText[i].m_Transform[1]
							, menu.saveText[i].m_TextVertices[2] + menu.saveText[i].m_Transform[0]
							, menu.saveText[i].m_TextVertices[3] + menu.saveText[i].m_Transform[1]
						};
						if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
						{
							aimingAt = i;
							cursorState = canClickOnIt;
							break;
						}
					}
					float vertices[4] =
					{
						menu.backText.m_TextVertices[0] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[1] + menu.backText.m_Transform[1]
						, menu.backText.m_TextVertices[2] + menu.backText.m_Transform[0]
						, menu.backText.m_TextVertices[3] + menu.backText.m_Transform[1]
					};
					if (IsInArea(vertices, Input::XRawMousePos, Window::height - Input::YRawMousePos))
					{
						cursorState = canClickOnIt;
						aimingAt = 3;
					}
					if (Input::LeftMousePress)
					{
						switch (aimingAt)
						{
						case 0:
						case 1:
						case 2:
							pathToSave = "res/save" +std::to_string(aimingAt) +"/";
							if (!Save(pathToSave, blocks))
							{
								std::cout << "error can not make save (blocks)" << std::endl;
							}
							if (!Save(pathToSave, Walls))
							{
								std::cout << "error can not make save (walls)" << std::endl;
							}
							if (!Save(pathToSave,craftStations,chests,doors,Crowns,seedlings))
							{
								std::cout << "error can not make save (struct)" << std::endl;
							}
							break;
						case 3:
							menuState = stateDefault;
							break;
						}
					}
					if (Input::EscapePress)
					{
						menuState = stateNone;
						break;
					}
					fontSh.Bind();
					ErrorGL(glBindTexture(GL_TEXTURE_2D, fontTex));

					for (int i = 0; i < 3; i++)
					{
						menu.saveText[i].Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, aimingAt == i);
					}
					menu.backText.Draw(fontSh, basicSh, transform, fontTex, TextBackGroundTex, aimingAt == 3);
					break;
				}
				}
				if (Input::F5Press)
				{
					if (!Save(pathToSave, blocks))
					{
						std::cout << "error can not make save (blocks)" << std::endl;
					}
					if (!Save(pathToSave, Walls))
					{
						std::cout << "error can not make save (walls)" << std::endl;
					}
					if (!Save(pathToSave, craftStations, chests, doors, Crowns, seedlings))
					{
						std::cout << "error can not make save (struct)" << std::endl;
					}
				}
				else if (Input::F8Press)
				{
					if (!Load(pathToSave, blocks, isSandOnX))
					{
						std::cout << "error can not load (blocks)" << std::endl;
					}
					if (!Load(pathToSave, Walls))
					{
						std::cout << "error can not load (walls)" << std::endl;
					}
					if (!Load(pathToSave, blocks, craftStations, chests, doors, Crowns, seedlings, structuresTextures, CrownTextures, CrownDD))
					{
						std::cout << "error can not load (struct)" << std::endl;
					}
					CreateChunks(blockChunks, blocks);
					CreateChunks(wallChunks, Walls);

				}
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
				

				basicSh.Bind();
				DrawCursor(cursorTextures, cursorState, cursorDD, basicSh, transform, camera);
			

				CreateChunks(chunksToRebuildBlock, blockChunks, blocks);
				CreateChunks(chunksToRebuildWall, wallChunks, Walls);

				Input::EndOfLoop();
				glfwSwapBuffers(window);
				glfwPollEvents();
			}
			break;
		}
		}
	}
	glfwTerminate();
	return 0;

	
}

