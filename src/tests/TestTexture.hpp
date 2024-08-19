
//
//  TestTexture.hpp
//  glhf
//
//  Created by lulz on 19/08/2024.
//

#ifndef TestTexture_hpp
#define TestTexture_hpp

#include <stdio.h>
#include "IndexBuffer.hpp"
#include "Shader.hpp"
#include "Test.h"
#include "VertexArray.hpp"
#include "vendor/glm/fwd.hpp"
#include "VertexBuffer.hpp"
#include "Texture.hpp"

namespace tests
{
    class TestTexture : public Test
    {
    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<Texture> m_Texture;

        glm::mat4 m_Proj, m_View;
        glm::vec3 m_TranslationA, m_TranslationB;
    public:
        TestTexture();
        ~TestTexture();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}

#endif /* TestTexture_hpp */
