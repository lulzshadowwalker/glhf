//
//  VertexBufferLayout.hpp
//  glhf
//
//  Created by lulz on 31/07/2024.
//

#ifndef VertexBufferLayout_hpp
#define VertexBufferLayout_hpp

#include <stdio.h>
#include <vector>

#include "Renderer.hpp"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#endif

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    
    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
            case GL_UNSIGNED_INT: return 4;
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout()
    : m_Stride(0) {}
    
    template <typename T>
    void Push(unsigned int count)
    {
        // static_assert(false);
    }
    
    template <>
    void Push<float>(unsigned int count)
    {
        m_Elements.push_back({ GL_FLOAT, count, false });
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }
    
    template <>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_INT, count, false });
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }
    
    template <>
    void Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_BYTE, count, true });
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }
    
    inline std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};

#endif /* VertexBufferLayout_hpp */
