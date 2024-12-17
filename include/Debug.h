#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#define ASSERT(x)                                                              \
  if (!(x))                                                                    \
  __builtin_trap()
// __debugbreak(); alternative in linux is __builtin_trap()
#define GLCall(x)                                                              \
  GLClearError();                                                              \
  x;                                                                           \
  ASSERT(GLLogCall(#x, __FILE__,                                               \
                   __LINE__)); // Werap a function with an error boundary

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
#else
#define GLCall(x) x
#endif // DEBUG

void glfw_error_callback(int error, const char* description);

#endif // DEBUG_H
