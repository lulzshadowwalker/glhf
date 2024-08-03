//
//  Shader.hpp
//  glhf
//
//  Created by lulz on 02/08/2024.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <stdio.h>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string vertexShader;
    std::string fragmentShader;
};

class Shader
{
private:
    unsigned int m_RendererID;
    const std::string& m_Filepath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
    
public:
    Shader(const std::string& m_Filepath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) const;
    void SetUniform1i(const std::string& name, int value) const;
private:
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    ShaderProgramSource LoadShader(std::string filepath);
    unsigned int GetUniformLocation(const std::string& name) const;
};

#endif /* Shader_hpp */
