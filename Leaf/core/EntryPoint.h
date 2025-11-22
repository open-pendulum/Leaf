#pragma once

#include "utils/Logger.h"

extern Leaf::Application *Leaf::CreateApplication();

int main(int argc, char **argv) {
    Leaf::Logger::Init();
    LEAF_ERROR("app log from macro");

    LEAF_CORE_INFO("core log from macro");

    //    LEAF_CORE_FATAL("errrr");

    auto app = Leaf::CreateApplication();
    app->Run();
    delete app;
    return 0;
}