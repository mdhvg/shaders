#ifndef DEBUG_H
#define DEBUG_H

#include <fmt/core.h>

#define GLASSERT(x)   \
  if (!(x))           \
    __builtin_trap()
// __debugbreak(); alternative in linux is __builtin_trap()

#define MyASSERT(x)   \
  if (!(x)) { \
    fmt::print("Assertion failed in {} in {} at line {}\n", \
    #x, __FILE__, __LINE__); \
    __builtin_trap(); \
  }
// __debugbreak(); alternative in linux is __builtin_trap()

#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  GLASSERT(GLLogCall(#x, __FILE__,                                               \
                   __LINE__)); // Werap a function with an error boundary

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

void glfw_error_callback(int error, const char* description);

#endif // DEBUG_H
