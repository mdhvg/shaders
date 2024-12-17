#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

template <class T, class P>
T max(T a, P b);

template <class T, class P, class... Args>
T max(T a, P b, Args... args);

template <class T, class P>
T min(T a, P b);

template <class T, class P, class... Args>
T min(T a, P b, Args... args);

void printComputeShaderLimits();
void printOpenGLInfo();
GLFWwindow* createWindow(int width, int height, const char* title);

#endif // UTILS_H