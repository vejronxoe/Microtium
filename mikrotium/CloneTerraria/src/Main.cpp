
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

struct block{
	Texture* te;
	unsigned int vao;
	float transform[2];
	float vertices[20];
};

bool k_WHold = false;
bool k_WPress = false;
bool k_AHold = false;
bool k_APress = false;
bool k_SHold = false;
bool k_SPress = false;
bool k_DHold = false;
bool k_DPress = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		k_WHold = true;
		k_WPress = true;
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		k_WHold = false;
	}



	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		k_APress = true;
		k_AHold = true;
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		k_AHold = false;
	}



	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		k_SPress = true;
		k_SHold = true;
	}
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		k_SHold = false;
	}



	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		k_DPress = true;
		k_DHold = true;
	}
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		k_DHold = false;
	}
}




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



	window = glfwCreateWindow(1920, 1080, "Mikrotium", NULL, NULL);
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
	glfwSetKeyCallback(window, key_callback);

	stbi_set_flip_vertically_on_load(true);
	ErrorGL(glEnable(GL_BLEND));
	ErrorGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	

	unsigned int vao;
	glGenVertexArrays(1, &vao);

	Shader sh("res/shaders/vertexPlayer.txt", "res/shaders/fragmentShaderBasic.txt");

	float vertices[20] =
	{
		
		-1.0f, 3.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 3.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f, 0.0f, 0.0f
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

	Texture tex("res/textures/player0.png", true);

	

	float posicionCamera[16];
	Camera(-56.0f, 56.0f, -31.5f, 31.5f, posicionCamera);
	sh.SetUniformMat4("camera", posicionCamera);
	




	Texture grass("res/textures/grass.jpg", false);
	Shader blockShader("res/shaders/vertexBlock.txt", "res/shaders/fragmentShaderBasic.txt");
	blockShader.SetUniformMat4("camera", posicionCamera);
	std::string line;
	std::vector<block> blocks;
	std::ifstream map("res/save/map.txt");
	if (!map)
	{
		std::cout << "can not open map file"<< std::endl;
	}
	{

		float y = 0.0f;
		while (std::getline(map, line))
		{
			float x = -56;
			for (int j = 0; j < line.length(); j++)
			{
				if (line[j] != ' ')
				{

					block blockT;
					blockT.te = &grass;
					blockT.transform[0] = x;
					blockT.transform[1] = y;
					blockT.vertices[0] = x - 0.5f; blockT.vertices[1] = y + 0.5f; blockT.vertices[2] = 0.1f; blockT.vertices[3] = 0.0f; blockT.vertices[4] = 1.0f;
					blockT.vertices[5] = x + 0.5f; blockT.vertices[6] = y + 0.5f; blockT.vertices[7] = 0.1f; blockT.vertices[8] = 1.0f; blockT.vertices[9] = 1.0f;
					blockT.vertices[10] = x + 0.5f; blockT.vertices[11] = y - 0.5f; blockT.vertices[12] = 0.1f; blockT.vertices[13] = 1.0f; blockT.vertices[14] = 0.0f;
					blockT.vertices[15] = x - 0.5f; blockT.vertices[16] = y - 0.5f; blockT.vertices[17] = 0.1f; blockT.vertices[18] = 0.0f; blockT.vertices[19] = 0.0f;
					glGenVertexArrays(1, &blockT.vao);
					ErrorGL(glBindVertexArray(blockT.vao));
					unsigned int VB;
					ErrorGL(glGenBuffers(1, &VB));
					ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, VB));
					ErrorGL(glBufferData(GL_ARRAY_BUFFER, 20  * sizeof(float), blockT.vertices, GL_STATIC_DRAW));

					ErrorGL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
					ErrorGL(glEnableVertexAttribArray(0));
					ErrorGL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
					ErrorGL(glEnableVertexAttribArray(1));

					ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob));

					ErrorGL(glBindVertexArray(0));

					blocks.push_back(blockT);
				}				
				x++;
			}
			y--;
		}
	}

	sh.Bind();
	tex.Bind();
	ErrorGL(glBindVertexArray(vao));

	float deltaTime;
	float transform[16];
	float x = 0.0f;
	float yp = 0.0f;





	while (!glfwWindowShouldClose(window))
	{
		sh.Bind();
		tex.Bind();
		ErrorGL(glBindVertexArray(vao));
		glClear(GL_COLOR_BUFFER_BIT);
		deltaTime = glfwGetTime() - pastTime;
		pastTime = glfwGetTime();
		if (k_WHold)
			yp += 10.0f * deltaTime; 
		if (k_SHold)
			yp += -10.0f * deltaTime; 
		if (k_DHold)
			x += 10.0f * deltaTime; 
		if (k_AHold)
			x += -10.0f * deltaTime; 
		Transform(x, yp, transform);
		sh.SetUniformMat4("transform", transform);
		ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));

	
		blockShader.Bind();
		for (int i = 0; i < blocks.size(); i++)
		{
			
			blocks.at(i).te->Bind();
			ErrorGL(glBindVertexArray(blocks.at(i).vao));
			ErrorGL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}