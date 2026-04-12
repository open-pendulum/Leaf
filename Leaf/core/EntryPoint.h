#pragma once

#include "Application.h"
#include "debug/Instrumentor.h"
#include "utils/Logger.h"

extern Leaf::Application *Leaf::CreateApplication();

int main(int argc, char **argv) {
    Leaf::Logger::Init();

    LEAF_PROFILE_BEGIN_SESSION("startup", "leaf-profile-startup.json");

    auto app = Leaf::CreateApplication();
    LEAF_PROFILE_END_SESSION();

    LEAF_PROFILE_BEGIN_SESSION("runtime", "leaf-profile-runtime.json");
    app->Run();
    LEAF_PROFILE_END_SESSION();
    LEAF_PROFILE_BEGIN_SESSION("shutdown", "leaf-profile-shutdown.json");
    delete app;
    LEAF_PROFILE_END_SESSION();
    return 0;
}