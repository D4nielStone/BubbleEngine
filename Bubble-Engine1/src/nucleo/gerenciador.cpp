#include "gerenciador.h"
#include "src/componentes/transformador.h"
#include "src/componentes/renderizador.h"

GLuint pgId;
Shader phong;

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
    phong.compilar("phong.vert", "phong.frag");
    phong.use();

    glEnable(GL_DEPTH_TEST);

    return true;
}
int Bubble::Nucleo::Gerenciador::pararloop() {
    return glfwWindowShouldClose(glfwWindow);
}
//@Render OpenGL (virtual function)
void Bubble::Nucleo::Gerenciador::renderizar(Bubble::Entidades::ObjetoGeral* go) {
        /* Render here */
        float st = glfwGetTime();

        int display_w, display_h;
        glfwGetFramebufferSize(glfwWindow, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto& r : go->obterComponentes("Renderizador")) {
            dynamic_cast<Bubble::Componentes::Rendererizador&>(r.get()).definirShader(phong);
            dynamic_cast<Bubble::Componentes::Rendererizador&>(r.get()).atualizar();
        }

        Bubble::Componentes::Transformador* transform = go->obterTransformador();
        transform->definirShader(phong);

        /* Swap front and back buffers */
        glfwSwapBuffers(glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();

        transform->Rotacionar(glm::vec3(0, 0.5f * (glfwGetTime() - st), 0));
        transform->atualizar();
   
}
void Bubble::Nucleo::Gerenciador::limpar() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}