#include "BubbleManager.h"
#include "Transform.h"
#include "Shader.h"

GLuint pgId;
Shader phong;

int main() {
    BubbleManager bm;

    if (bm.bubbleInitGlfw()) {

    ObjectFile obj("C:/Users/Daniel/Desktop/test_monkey/scene.gltf");
    GameObject go(obj);
    
    for (auto& c : go.Components) {
        c->load();
    }
                                      
        bm.renderOpengl(&go);
    }
    return 0;
}

//@Initialize GLFW and GLAD
bool BubbleManager::bubbleInitGlfw() {
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
    phong.compile("phong.vert", "phong.frag");
    phong.use();

    glEnable(GL_DEPTH_TEST);

    return true;
}
//@Render OpenGL (virtual function)
void BubbleManager::renderOpengl(GameObject* go) {
    Transform* transform = go->getTransform();
    std::vector<Renderer*> rendererList;
    for (auto& r : go->getComponents("Renderer")) {
        rendererList.push_back(dynamic_cast<Renderer*>(r));
        dynamic_cast<Renderer*>(r)->setShader(phong);
    }
    transform->setShader(phong);
    while (!glfwWindowShouldClose(glfwWindow))
    {
        /* Render here */
        float st = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (Renderer* renderer : rendererList) {
            renderer->update();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(glfwWindow);

        /* Poll for and process events */
        glfwPollEvents();

        transform->Rotate(glm::vec3(0, 0.5f * (glfwGetTime() - st), 0));
        transform->update();
    }
}
