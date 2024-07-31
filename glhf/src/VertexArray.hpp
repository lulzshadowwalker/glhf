//
//  VertexArray.hpp
//  glhf
//
//  Created by lulz on 31/07/2024.
//

#ifndef VertexArray_hpp
#define VertexArray_hpp

#include <stdio.h>
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();
    
    void Bind() const;
    void Unbind() const;
    void AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);
};
#endif /* VertexArray_hpp */
