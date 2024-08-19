//
//  Test.h
//  glhf
//
//  Created by lulz on 11/08/2024.
//

#ifndef Test_h
#define Test_h

namespace tests {
class Test {
public:
    Test() {};
    virtual ~Test() {};

    virtual void OnUpdate(float deltaTime) {}
    virtual void OnImGuiRender() {}
    virtual void OnRender() {}
};
}


#endif /* Test_h */