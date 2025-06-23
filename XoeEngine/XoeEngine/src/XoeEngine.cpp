#include "XoeEngine.h"

int SetupEngine()
{
    if (glewInit() != GLEW_OK)
        return -1;
    ErrorGL(glEnable(GL_BLEND));
    ErrorGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    return 0;
}



void EndProgram()
{
    glfwTerminate();
}