#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#define NORMALIZE_COLOR(value) (static_cast<float>(value) / 255.0f)

struct ShaderProgramSource
{
    std::string vertexShader;
    std::string fragmentShader;
};

static ShaderProgramSource loadShader(std::string filepath)
{
    std::ifstream stream(filepath);
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1,
    };
    
    std::string line;
    std::stringstream ss[2];
    ShaderType mode = ShaderType::NONE;
    
    while(getline(stream, line))
    {
        if (line.find("#SHADER") != std::string::npos) {
            if (line.find("VERTEX") != std::string::npos) {
                mode = ShaderType::VERTEX;
            } else if (line.find("FRAGMENT")) {
                mode = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)mode] << line << "\n";
        }
    }
    
    return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) alloca(sizeof(char) * length);
        glGetShaderInfoLog(id, length, &length, message);
        
        printf("Failed to compile %s program\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        printf("%s\n", message);
        
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // A program in OpenGL is a collection of shaders that are used together for rendering.
    unsigned int program = glCreateProgram();
    
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    // Deleting the intemediate files. Kind of like deleting the intermediate .obj files in C++
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

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
    
     unsigned int vao;
     glGenVertexArrays(1, &vao);
     glBindVertexArray(vao);
    
    static const float positions[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f,
    }; // Buffer, on the CPU

    unsigned int buffer; // The ID of the generated buffer
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    
    static const unsigned int indices[] = { // index buffer
        0, 1, 2,
        2, 3, 0
    };
    unsigned int ibo; // Index Buffer Object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    ShaderProgramSource shader = loadShader("res/shaders/basic.shader");

    unsigned int program = createShader(shader.vertexShader, shader.fragmentShader);
    glUseProgram(program);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glDrawElements(
                       GL_TRIANGLES,
                       6, // number of indices not vertices
                       GL_UNSIGNED_INT,
                       nullptr // no need to specify it here since we bound the ibo to the GL_ELEMENT_ARRAY_BUFFER slot
        );
        
        /* Drawing a Triangle in Legacy OpenGL */
        // glBegin(GL_TRIANGLES);
        // glVertex2d(-0.5f, -0.5f);
        // glVertex2d(0.5f, -0.5f);
        // glVertex2d(0.0f, 0.5f);
        // glEnd();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glDisableVertexAttribArray(0);
    glDeleteProgram(program);
    glDeleteVertexArrays(1, &vao);
    glfwTerminate();
    
    return 0;
}
