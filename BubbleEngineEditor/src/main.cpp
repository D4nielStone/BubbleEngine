#include "src/nucleo/gerenciador.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

float SCR_WIDTH = 640, SCR_HEIGHT = 480;

int main() {
    Bubble::Nucleo::Gerenciador gerenciador;

    if (!gerenciador.inicializacao())
        return -1;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;

    const char* path = "C:/Users/Daniel/Desktop";
    gerenciador.criarProjeto(path, "ProjetoBubble");

    // ImGui initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(gerenciador.glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 130"); // Use the appropriate GLSL version

    // Framebuffer configuration
    GLuint FBO, textureColorbuffer, rbo;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create a texture to attach to the framebuffer
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    // Create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Main loop
    while (!gerenciador.pararloop()) {
        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Create ImGui window(s) here
        ImGui::Begin("Editor");
        ImGui::Image((void*)(intptr_t)textureColorbuffer, ImVec2(SCR_WIDTH, SCR_HEIGHT), ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();

        SCR_WIDTH = ImGui::GetWindowWidth();
        SCR_HEIGHT = ImGui::GetWindowHeight();
        // Bind to the framebuffer and render the scene
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gerenciador.gerenciadorDeCenas.atualizarCenaAtual(Modo::Editor, 1, SCR_WIDTH/SCR_HEIGHT); // Render your scene here
        gerenciador.gerenciadorDeCenas.camera_do_editor.transformacao->definirPosicao(glm::vec3(0, 0, -5)); // Render your scene here
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(gerenciador.glfwWindow);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    gerenciador.limpar();
    return 0;
}
