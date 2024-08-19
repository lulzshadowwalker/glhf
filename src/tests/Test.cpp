#include "Test.h"
#include "imgui.h"

namespace tests {
    TestMenu::TestMenu(Test*& current) : m_CurrentTest(current)
    {
    }

    void TestMenu::OnImGuiRender()
    {
        //  TODO: Show current test name
        // ImGui::BeginMenu("Select Test");
        for (auto& test : m_Tests) {
            if (ImGui::Button(test.first.c_str())) {
                m_CurrentTest = test.second();
            }
        }
        // ImGui::EndMenu();
    }

}
