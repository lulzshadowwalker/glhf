//
//  Renderer.cpp
//  glhf
//
//  Created by lulz on 30/07/2024.
//

#include "Renderer.hpp"

void GLClearError() {
    while(glGetError() != GL_NO_ERROR);
}

bool GLHasError(std::string method, std::string filename, int line) {
    if (GLenum error = glGetError() != GL_NO_ERROR)
    {
        printf("Error %d: %s at %s:%d\n", error, method.c_str(), filename.c_str(), line);
        return true;
    }
    
    return false;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();
    
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
