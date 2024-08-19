//
//  Test.h
//  glhf
//
//  Created by lulz on 11/08/2024.
//

#ifndef Test_h
#define Test_h

#include <vector>
#include <string>
#include <functional>

namespace tests {
    class Test {
        public:
            Test() {};
            virtual ~Test() {};

            virtual void OnUpdate(float deltaTime) {}
            virtual void OnImGuiRender() {}
            virtual void OnRender() {}
    };

    class TestMenu : public Test {
        public:
            TestMenu(Test*& current);

            void OnImGuiRender();

            template<typename T>
            void RegisterTest(const std::string& name) {
                m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
            }
        private:
            Test*& m_CurrentTest;
            std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
    };
}


#endif /* Test_h */
