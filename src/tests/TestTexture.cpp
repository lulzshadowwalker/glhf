//
//  TestTexture.cpp
//  glhf
//
//  Created by lulz on 19/08/2024.
//

#include "TestTexture.hpp"
#include "Renderer.hpp"
#include "VertexBufferLayout.hpp"
#include "imgui.h"
#include "vendor/glm/ext/matrix_transform.hpp"
#include "vendor/glm/ext/matrix_clip_space.hpp"

namespace tests
{

    TestTexture::TestTexture()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        m_TranslationA(200, 200, 0),
        m_TranslationB(400, 200, 0)
    {
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 1.0f, 0.0f, 0.0f,
             50.0f,  50.0f, 1.0f, 1.0f, 0.0f,
            -50.0f,  50.0f, 0.0f, 1.0f, 0.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 5 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(3);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);

        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("/Users/lulz/dev/glhf/res/shaders/basic.shader");
        m_Shader->Bind();
        m_Texture = std::make_unique<Texture>("/Users/lulz/dev/glhf/res/texture.png");
        m_Shader->SetUniform1i("u_Texture", 0);
    }

    TestTexture::~TestTexture()
    {
        //
    }

    void TestTexture::OnUpdate(float deltaTime)
    {
    }

    void TestTexture::OnRender()
    {
        Renderer renderer;

        m_Texture->Bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
    }

    void TestTexture::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
