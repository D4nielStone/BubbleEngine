#include "gerenciador.h"
#include "src/componentes/transformacao/transformacao.h"
#include "src/componentes/renderizador/renderizador.h"

//@Initialize GLFW and GLAD
bool Bubble::Nucleo::Gerenciador::inicializacao() {
    if (!glfwInit())
        return false;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindow = glfwCreateWindow(640, 480, "Bubble", NULL, NULL);
    if (!glfwWindow)
    {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(glfwWindow);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    return true;
}
int Bubble::Nucleo::Gerenciador::pararloop() {
    return glfwWindowShouldClose(glfwWindow);
}
void Bubble::Nucleo::Gerenciador::atualizar(Bubble::Entidades::Entidade* ent) {
        for (auto& c : ent->obterComponentesLogicos()) {
            c->atualizar();
        }
}
//@Render OpenGL (virtual function)
void Bubble::Nucleo::Gerenciador::renderizar(Bubble::Entidades::Entidade* ent) {
        /* Render here */

        int display_w, display_h;
        glfwGetFramebufferSize(glfwWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        for (auto& r : ent->obterRenderizadores())
        {
            r->atualizar();
        }


        /* Swap front and back buffers */
        glfwSwapBuffers(glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();

   
}
void Bubble::Nucleo::Gerenciador::limpar() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}