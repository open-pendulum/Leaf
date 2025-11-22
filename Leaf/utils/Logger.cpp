//
// Created by Saber on 2025/2/28.
//

#include "Logger.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Leaf {
std::shared_ptr<spdlog::logger> Logger::sCoreLogger;
std::shared_ptr<spdlog::logger> Logger::sAppLogger;
void Logger::Init() {
    sCoreLogger =
        spdlog::stdout_color_mt("LEAF", spdlog::color_mode::automatic);
    sCoreLogger->set_level(spdlog::level::trace);
    sCoreLogger->set_pattern("%^[%T][%t][%n][%l]: %v%$");

    sAppLogger = spdlog::stdout_color_mt("APP", spdlog::color_mode::automatic);
    sAppLogger->set_level(spdlog::level::trace);
    sAppLogger->set_pattern("%^[%T][%t][%n][%l]: %v%$");
}

std::shared_ptr<spdlog::logger> Logger::GetCoreLogger() {
    return sCoreLogger;
}

std::shared_ptr<spdlog::logger> Logger::GetAppLogger() {
    return sAppLogger;
}

}  // namespace Leaf