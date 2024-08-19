//
//  TestClearColor.cpp
//  glhf
//
//  Created by lulz on 11/08/2024.
//

#include "TestClearColor.hpp"
#include "Renderer.hpp"
#include "imgui.h"

namespace tests
{

    TestClearColor::TestClearColor()
        : m_Color{0.2f, 0.3f, 0.8f, 1.0f}
    {
    }

    TestClearColor::~TestClearColor()
    {
    }

    void TestClearColor::OnUpdate(float deltaTime)
    {
    }

    void TestClearColor::OnRender()
    {
        GLCall(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
    }

    void TestClearColor::OnImGuiRender()
    {
        ImGui::ColorPicker4("Clear Color", m_Color);
    }
}