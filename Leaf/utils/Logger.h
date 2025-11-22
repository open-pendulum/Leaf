#pragma once

#define SPDLOG_USE_STD_FORMAT
#define SPDLOG_HEADER_ONLY
#include "Core.h"
#include "spdlog/logger.h"
namespace Leaf {
class LEAF_API Logger {
public:
    static void Init();

    static std::shared_ptr<spdlog::logger> GetCoreLogger();

    static std::shared_ptr<spdlog::logger> GetAppLogger();

private:
    static std::shared_ptr<spdlog::logger> sCoreLogger;
    static std::shared_ptr<spdlog::logger> sAppLogger;
};

}  // namespace Leaf

#define LEAF_CORE_ERROR(...) ::Leaf::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define LEAF_CORE_WARN(...) ::Leaf::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define LEAF_CORE_INFO(...) ::Leaf::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define LEAF_CORE_TRACE(...) ::Leaf::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define LEAF_CORE_FATAL(...) ::Leaf::Logger::GetCoreLogger()->fatal(__VA_ARGS__)

#define LEAF_ERROR(...) ::Leaf::Logger::GetAppLogger()->error(__VA_ARGS__)
#define LEAF_WARN(...) ::Leaf::Logger::GetAppLogger()->warn(__VA_ARGS__)
#define LEAF_INFO(...) ::Leaf::Logger::GetAppLogger()->info(__VA_ARGS__)
#define LEAF_TRACE(...) ::Leaf::Logger::GetAppLogger()->trace(__VA_ARGS__)
#define LEAF_FATAL(...) ::Leaf::Logger::GetAppLogger()->fatal(__VA_ARGS__)