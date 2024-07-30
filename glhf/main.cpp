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

#define ASSERT(x) if(!(x)) __builtin_trap()

#define GLCall(x)\
    GLClearError();\
    x;\
    ASSERT(!GLHasError(#x, __FILE__, __LINE__))

static void GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}

static bool GLHasError(std::string method, std::string filename, int line) {
    if (GLenum error = glGetError() != GL_NO_ERROR)
    {
        printf("Error %d: %s at %s:%d\n", error, method.c_str(), filename.c_str(), line);
        return true;
    }
    
    return false;
}


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
            ASSERT((int)mode != -1);
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
    
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    
    // Deleting the intemediate files. Kind of like deleting the intermediate .obj files in C++
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

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
    printf("Hello, %s.\n", glGetString(GL_VERSION));
    
     unsigned int vao;
     GLCall(glGenVertexArrays(1, &vao));
     GLCall(glBindVertexArray(vao));
    
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
    unsigned int ibo; // Index Buffer Object
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    
    ShaderProgramSource shader = loadShader("res/shaders/basic.shader");

    unsigned int program = createShader(shader.vertexShader, shader.fragmentShader);
    GLCall(glUseProgram(program));
    
    // uniforms need to be defined after `glUseProgram` so they can be applied to the shader.
    GLCall(unsigned int uColor = glGetUniformLocation(program, "u_Color"));
    GLCall(glUniform4f(uColor, 1.0f, 0.0f, 0.0f, 1.0f));
    
    float rgb[3] = {0.2f, 0.6f, 0.9f};
    float step = 0.01;
    
    /* Loop until the user closes the window */
    while (true)
    {
        GLCall(if (glfwWindowShouldClose(window)) break;);
        
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        GLCall(glDrawElements(
                       GL_TRIANGLES,
                       6, // number of indices not vertices
                       GL_UNSIGNED_INT,
                       nullptr // no need to specify it here since we bound the ibo to the GL_ELEMENT_ARRAY_BUFFER slot
        ));
        
        
        GLCall(glUniform4f(uColor, rgb[0], rgb[1], rgb[2], 1.0));
        for (int i = 0; i < 3; i++) {
            if (rgb[i] > 1 || rgb[i] < 0) {
                step *= -1;
            }
            
            rgb[i] += step;
        }

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
    
    GLCall(glDisableVertexAttribArray(0));
    GLCall(glDeleteProgram(program));
    GLCall(glDeleteVertexArrays(1, &vao));
    GLCall(glfwTerminate());
    
    return 0;
}
