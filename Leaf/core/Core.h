#pragma once

#ifdef LEAF_WINDOWS
#ifdef BUILD_LEAF_SHARED
#ifdef BUILD_LEAF
#define LEAF_API __declspec(dllexport)
#else
#define LEAF_API __declspec(dllimport)
#endif
#else
#define LEAF_API
#endif
#elif LEAF_APPLE || LEAF_UNIX

#ifdef BUILD_LEAF
#define LEAF_API __attribute__((visibility("default")))
#else
#define LEAF_API
#endif
#endif

#ifdef LEAF_ENABLE_ASSERT
#define LEAF_ASSERT(x, ...)                                                    \
    {                                                                          \
        if (!(x)) {                                                            \
            LEAF_ERROR("Assertion Failed: {}", __VA_ARGS__);                   \
            __debugbreak();                                                    \
        }                                                                      \
    }
#define LEAF_CORE_ASSERT(x, ...)                                               \
    {                                                                          \
        if (!(x)) {                                                            \
            LEAF_CORE_ERROR("Assertion Failed: {}", __VA_ARGS__);              \
            __debugbreak();                                                    \
        }                                                                      \
    }
#else
#define LEAF_ASSERT(x, ...)
#define LEAF_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define LEAF_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)