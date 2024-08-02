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
#include "src/VertexBufferLayout.hpp"

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
    window = glfwCreateWindow(640, 480, "Hello lulzie.", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    printf("Hello, %s.\n", glGetString(GL_VERSION));
    
    static const float positions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
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
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);
    
    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    
    // uniforms need to be defined after `glUseProgram` so they can be applied to the shader.
    shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
    
    float rgb[3] = {0.2f, 0.6f, 0.9f};
    float step = 0.01;
    
    Renderer renderer;
    
    /* Loop until the user closes the window */
    while (true)
    {
        GLCall(if (glfwWindowShouldClose(window)) break;);
        
        renderer.Clear();
        
        shader.SetUniform4f("u_Color", rgb[0], rgb[1], rgb[2], 1.0f);
        for (int i = 0; i < 3; i++) {
            if (rgb[i] > 1 || rgb[i] < 0) {
                step *= -1;
            }
            
            rgb[i] += step;
        }
        
        renderer.Draw(va, ib, shader);

        /* Drawing a Triangle in Legacy OpenGL */
        // glBegin(GL_TRIANGLES);
        // glVertex2d(-0.5f, -0.5f);
        // glVertex2d(0.5f, -0.5f);
        // glVertex2d(0.0f, 0.5f);
        // glEnd();
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));
        
        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    
    // GLCall(glDisableVertexAttribArray(0)); TODO: Cleanup in either the VertexArray or IndexBuffer ?
    GLCall(glfwTerminate());
    
    return 0;
}
