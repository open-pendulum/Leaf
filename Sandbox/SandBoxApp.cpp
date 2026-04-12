#include <Core/EntryPoint.h>
#include "ExampleLayer.h"
#include "SandBox2D.h"

class Sandbox : public Leaf::Application {
public:
    Sandbox() {
        PushLayer(new Sandbox2D());
    };
    ~Sandbox() override = default;
};

Leaf::Application *Leaf::CreateApplication() {
    return new Sandbox();
}