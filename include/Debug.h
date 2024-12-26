#ifndef DEBUG_H
#define DEBUG_H

#include <fmt/core.h>

#if defined(_WIN32) || defined(_WIN64)
#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#else
#define DEBUG_BREAK() __builtin_trap()
#endif
#else
#define DEBUG_BREAK() __builtin_trap()
#endif

#define GLASSERT(x)                                                 \
    if (!(x))                                                       \
	DEBUG_BREAK();

#define MyASSERT(x)                                                 \
    if (!(x)) {                                                     \
	fmt::print("Assertion failed: {} at {}:{}\n", #x, __FILE__,     \
		   __LINE__);                                               \
	DEBUG_BREAK();                                                  \
    }

#define GLCall(x)                                                   \
    GLClearError();                                                 \
    x;                                                              \
    GLASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

void glfw_error_callback(int error, const char* description);

#endif // DEBUG_H
