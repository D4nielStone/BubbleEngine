#include "delta_time.hpp"
#include "GLFW/glfw3.h"

double st = 0.0;
double Bubble::Tempo::delta_time = 0.0;
void Bubble::Tempo::iniDT()
{
    st = glfwGetTime();
}
void Bubble::Tempo::endDT()
{
    // Calcular delta time
    delta_time = glfwGetTime() - st;
}
