#include"Renderer.h"
#include<sstream>
#include <fstream>
#include <string>


#include"Vendor/stb_image/stb_Image.h"
struct shaderProgramSource
{
	std::string VS;
	std::string FS;
};

shaderProgramSource ParseShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else if (type != ShaderType::NONE)
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

void squere::CountPoints()
{
	float leanght[2];
	leanght[0] = m_Scale[0] / 2.0f;
	leanght[1] = m_Scale[1] / 2.0f;

	m_Points[0] = -leanght[0]; m_Points[1] = -leanght[1]; m_Points[2] = 0.0f; m_Points[3] = 0.0f;
	m_Points[4] = leanght[0]; m_Points[5] = -leanght[1]; m_Points[6] = 1.0f; m_Points[7] = 0.0f;
	m_Points[8] = leanght[0]; m_Points[9] = leanght[1]; m_Points[10] = 1.0f; m_Points[11] = 1.0f;
	m_Points[12] = -leanght[0]; m_Points[13] = leanght[1]; m_Points[14] = 0.0f; m_Points[15] = 1.0f;

	ErrorGL(glGenBuffers(1, &m_PointsID));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_PointsID));
	ErrorGL(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), m_Points, GL_DYNAMIC_DRAW));
}
void squere::rendererObject()
{
	Bind();
	ErrorGL(glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, nullptr));
	Unbind();
}
unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	ErrorGL(glShaderSource(id, 1, &src, nullptr));
	ErrorGL(glCompileShader(id));
	int result;
	ErrorGL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		ErrorGL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		ErrorGL(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << std::endl;
		std::cout << message << std::endl;
		ErrorGL(glDeleteShader(id));
		return 0;
	}
	return id;
}
unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	ErrorGL(glAttachShader(program, vs));
	ErrorGL(glAttachShader(program, fs));
	ErrorGL(glLinkProgram(program));
	ErrorGL(glValidateProgram(program));
	ErrorGL(glDeleteShader(fs));
	ErrorGL(glDeleteShader(vs));
	return program;
}


#if _DEBUG 
squere::squere(float TransformX, float TransformY, float ScaleX, float ScaleY, std::string pathImageEXE, std::string pathImage, int TexSlot,std::string pathshaderEXE, std::string pathshader)
	:m_Points{ 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f } , m_Order{ 0,1,2,2,3,0 }, m_ImageID(0), m_PointsID(0), m_OrderID(0), m_LocalBuffer(0), m_Height(0), m_Width(0), m_BPP(0), m_Transform{ TransformX,TransformY }, m_Scale{ ScaleX,ScaleY }
{
	CountPoints();
	ErrorGL(glGenBuffers(1, &m_OrderID));
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OrderID));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), m_Order, GL_STATIC_DRAW));
	stbi_set_flip_vertically_on_load(1);


	m_LocalBuffer = stbi_load(pathImage.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	ErrorGL(glGenTextures(1, &m_ImageID));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ImageID));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, TexSlot));
	shaderProgramSource ShaderSource;
	ShaderSource = ParseShader(pathshader);
	m_ProgramID = CreateShader(ShaderSource.VS, ShaderSource.FS);
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
	int location = glGetUniformLocation(m_ProgramID, "u_Texture");
	if (location == -1)
		std::cout << "(warnig) uniform not found     name: " << "u_Texture" << std::endl;
	glUniform1i(location, TexSlot);


}
#else
squere::squere(int TransformX, int TransformY, int ScaleX, int ScaleY, std::string pathImageEXE, std::string pathImage, int TexSlot, std::string pathshaderEXE, std::string pathshader)
	:m_Points{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, m_Order{ 0,1,2,2,3,0 }, m_ImageID(0), m_PointsID(0), m_OrderID(0), m_LocalBuffer(0), m_Height(0), m_Width(0), m_BPP(0), m_Transform{ TransformX,TransformY }, m_Scale{ ScaleX,ScaleY }
{
	CountPoints();
	ErrorGL(glGenBuffers(1, &m_OrderID));
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OrderID));
	ErrorGL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), m_Order, GL_STATIC_DRAW));
	stbi_set_flip_vertically_on_load(1);


	m_LocalBuffer = stbi_load(pathImageEXE.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	ErrorGL(glGenTextures(1, &m_ImageID));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ImageID));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	ErrorGL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	ErrorGL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, TexSlot));
	shaderProgramSource ShaderSource = ParseShader(pathshaderEXE);
	m_ProgramID = CreateShader(ShaderSource.VS, ShaderSource.FS);
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
	ErrorGL(int location = glGetUniformLocation(m_ProgramID, "u_Texture"));
	if (location == -1)
		std::cout << "(warnig) uniform not found     name: " << "u_Texture" << std::endl;
	glUniform1i(location, TexSlot);


}
#endif
void squere::Unbind() const
{
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, 0));
	ErrorGL(glUseProgram(0));

}

void squere::Bind() const
{
	ErrorGL(glActiveTexture(GL_TEXTURE0));
	ErrorGL(glBindTexture(GL_TEXTURE_2D, m_ImageID));
	ErrorGL(glBindBuffer(GL_ARRAY_BUFFER, m_PointsID));
	ErrorGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_OrderID));
	glUseProgram(m_ProgramID);

}

