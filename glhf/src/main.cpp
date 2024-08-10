#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/ext/matrix_transform.hpp"
#include "vendor/glm/ext/matrix_clip_space.hpp"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit()) {
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
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    
    static const float positions[] = {
        100.0f, 100.0f,   0.0f, 0.0f,
        200.0f, 100.0f,   1.0f, 0.0f,
        200.0f, 200.0f,   1.0f, 1.0f,
        100.0f, 200.0f,   0.0f, 1.0f
    }; // Buffer, on the CPU
    
    
        unsigned int buffer; // The ID of the generated buffer
        GLCall(glGenBuffers(1, &buffer));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));
    
    static const unsigned int indices[] = { // index buffer
        0, 1, 2,
        2, 3, 0
    };
    
    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);
    
    glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));

    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    
    Texture texture("res/texture.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);
    
    // uniforms need to be defined after `glUseProgram` so they can be applied to the shader.
    shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
    
    float rgb[3] = {0.2f, 0.6f, 0.9f};
    float step = 0.01;
    
    Renderer renderer;
    
    glm::vec3 translation(200.0f, 200.0f, 0.0f);
    
    /* Loop until the user closes the window */
    while (true)
    {
        GLCall(if (glfwWindowShouldClose(window)) break;);
        
        renderer.Clear();
        
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        // in reverse because OpenGL expects matrices to be stored in column-major ordering
        glm::mat4 mvp = projection * view * model;
        
        shader.SetUniformMat4f("u_MVP", mvp);
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow(); // Show demo window! :)
        
        ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 960.0f);

        shader.SetUniform4f("u_Color", rgb[0], rgb[1], rgb[2], 1.0f);
        for (int i = 0; i < 3; i++) {
            if (rgb[i] > 1 || rgb[i] < 0) {
                step *= -1;
            }
            
            rgb[i] += step;
        }
        
        renderer.Draw(va, ib, shader);
        
        // Rendering
        // (Your code clears your framebuffer, renders your other stuff etc.)
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        // (Your code calls glfwSwapBuffers() etc.)
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));
        
        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    // GLCall(glDisableVertexAttribArray(0)); TODO: Cleanup in either the VertexArray or IndexBuffer ?
    GLCall(glfwTerminate());
    
    return 0;
}
