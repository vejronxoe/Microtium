#include "XoeEngine.h"
#include"ErrorSystem.h"

bool SetupEngine(int wideOfWindow, int heightOfWindow,const char* nameOfGame)
{
       GLFWwindow* window;
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(wideOfWindow, heightOfWindow, nameOfGame, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
        return -1;

    ErrorGL(glEnable(GL_BLEND));
    ErrorGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    return 0;
}


void ClearScrean()
{
    ErrorGL(glClear(GL_COLOR_BUFFER_BIT));
}
void EndProgram()
{
    glfwTerminate();
}