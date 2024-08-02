//
//  Renderer.hpp
//  glhf
//
//  Created by lulz on 30/07/2024.
//
#include <GLFW/glfw3.h>
#include <string>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include "./src/IndexBuffer.hpp"
#include "./src/VertexArray.hpp"
#include "./src/Shader.hpp"

#define NORMALIZE_COLOR(value) (static_cast<float>(value) / 255.0f)

#define ASSERT(x) if(!(x)) __builtin_trap()

#define GLCall(x)\
    GLClearError();\
    x;\
    ASSERT(!GLHasError(#x, __FILE__, __LINE__))

void GLClearError();

bool GLHasError(std::string method, std::string filename, int line);

class Renderer
{
private:
    //
public:
    void Clear() const;
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

#endif /* Renderer_hpp */
