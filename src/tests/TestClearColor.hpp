//
//  TestClearColor.hpp
//  glhf
//
//  Created by lulz on 11/08/2024.
//

#ifndef TestClearColor_hpp
#define TestClearColor_hpp

#include <stdio.h>
#include "test.h"

namespace tests
{
    class TestClearColor : public Test
    {
    private:
        float m_Color[4];

    public:
        TestClearColor();
        ~TestClearColor();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    };
}

#endif /* TestClearColor_hpp */