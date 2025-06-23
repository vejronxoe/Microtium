#include"ErrorSystem.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}
bool GLCheakError(const char* filePath, int line)
{
	int error = glGetError();
	bool errors = false;
	while (error != GL_NO_ERROR)
	{
		std::cout << "[OpenGL Error] file: " << filePath << " line: " << line << "{ Error: " << error << " }" << std::endl;
		error = glGetError();
		errors = true;
	}
	return errors;
}