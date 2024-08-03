//
//  Texture.hpp
//  glhf
//
//  Created by lulz on 03/08/2024.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <string>

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_Filepath;
    int  m_Width, m_Height, m_BPP;
    unsigned char* m_LocalBuffer;
public:
    Texture(const std::string& filepath);
    ~Texture();
    
    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
};
#endif /* Texture_hpp */
