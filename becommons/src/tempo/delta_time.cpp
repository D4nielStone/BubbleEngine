#include "delta_time.hpp"
#include "GLFW/glfw3.h"

double st = 0.0;
double Bubble::Tempo::delta_time = 0.0;
void BECOMMONS_DLL_API Bubble::Tempo::iniDT()
{
    st = glfwGetTime();
}
void BECOMMONS_DLL_API Bubble::Tempo::endDT()
{
    // Calcular deltaTime
    delta_time = glfwGetTime() - st;
}