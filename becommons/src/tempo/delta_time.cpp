#include "delta_time.hpp"
#include "GLFW/glfw3.h"

int Bubble::Tempo::s_passados = 0;
double Bubble::Tempo::delta_time = 0.0;
void Bubble::Tempo::iniDT()
{
    s_passados = glfwGetTime();
}
void Bubble::Tempo::endDT()
{
    // Calcular delta time
    delta_time = glfwGetTime() - s_passados;
}
