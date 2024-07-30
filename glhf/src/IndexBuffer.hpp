//
//  VertexBuffer.hpp
//  glhf
//
//  Created by lulz on 30/07/2024.
//

#ifndef VertexBuffer_hpp
#define VertexBuffer_hpp

#include <stdio.h>

#endif /* VertexBuffer_hpp */

class IndexBuffer {
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
};
