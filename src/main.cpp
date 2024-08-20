#include <GLFW/glfw3.h>
#include "Renderer.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "tests/TestClearColor.hpp"
#include "tests/TestTexture.hpp"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    printf("Hello, Apple.\n");
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 480, "Hello lulzie.", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    printf("Hello, %s.\n", glGetString(GL_VERSION));

    glfwSwapInterval(1);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
                                                          // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

                                                          // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    glm::vec3 translation(200.0f, 200.0f, 0.0f);

    tests::Test *currentTest = nullptr;
    tests::TestMenu *testMenu = new tests::TestMenu(currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<tests::TestClearColor>("Color Picker");
    testMenu->RegisterTest<tests::TestTexture>("Texture");

    Renderer renderer;

    /* Loop until the user closes the window */
    while (true)
    {
        GLCall(if (glfwWindowShouldClose(window)) break;);

        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        renderer.Clear();

        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("< Back"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
            }

            ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        GLCall(glfwSwapBuffers(window));

        GLCall(glfwPollEvents());

        if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)
        {
            break;
        }
    }

    //  NOTE: What is faster, deleting testMenu any way or using an if condition.
    delete currentTest;
    if (currentTest != testMenu)
        delete testMenu;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // GLCall(glDisableVertexAttribArray(0));  TODO: Cleanup in either the VertexArray or IndexBuffer ?
    GLCall(glfwTerminate());

    return 0;
}
