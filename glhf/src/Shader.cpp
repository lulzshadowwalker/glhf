//
//  Shader.cpp
//  glhf
//
//  Created by lulz on 02/08/2024.
//

#include "Shader.hpp"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Renderer.hpp"

Shader::Shader(const std::string& filepath)
: m_Filepath(filepath), m_RendererID(0)
{
    ShaderProgramSource shader = LoadShader(filepath);
    m_RendererID = CreateShader(shader.vertexShader, shader.fragmentShader);
}

Shader::~Shader()
{
    Unbind();
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform1i(const std::string& name, int value) const
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

ShaderProgramSource Shader::LoadShader(std::string filepath)
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    m_RendererID = glCreateShader(type);
    
    const char* src = source.c_str();
    glShaderSource(m_RendererID, 1, &src, nullptr);
    glCompileShader(m_RendererID);
    
    int result;
    glGetShaderiv(m_RendererID, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(m_RendererID, GL_INFO_LOG_LENGTH, &length);
        
        char* message = (char*) alloca(sizeof(char) * length);
        glGetShaderInfoLog(m_RendererID, length, &length, message);
        
        printf("Failed to compile %s program\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment");
        printf("%s\n", message);
        
        glDeleteShader(m_RendererID);
        return 0;
    }
    
    return m_RendererID;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // A program in OpenGL is a collection of shaders that are used together for rendering.
    unsigned int program = glCreateProgram();
    
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    
    // Deleting the intemediate files. Kind of like deleting the intermediate .obj files in C++
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

unsigned int Shader::GetUniformLocation(const std::string& name) const
{
   if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
   {
       return m_UniformLocationCache.at(name);
   }
    
    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        printf("Failed to get the uniform '%s' location\n", name.c_str());
    }
    
    // m_UniformLocationCache.insert({ name, location });
    return location;
}
