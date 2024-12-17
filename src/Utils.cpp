#include "Utils.h"
#include "Debug.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


template <class T, class P>
T max(T a, P b) {
    return a > b ? a : b;
}

template <class T, class P, class... Args>
T max(T a, P b, Args... args) {
    return max(max(a, b), args...);
}

template <class T, class P>
T min(T a, P b) {
    return a < b ? a : b;
}

template <class T, class P, class... Args>
T min(T a, P b, Args... args) {
    return min(min(a, b), args...);
}

template int max<int, double>(int, double);
template double max<double, int>(double, int);
template float max<float, int>(float, int);

template int min<int, double>(int, double);
template double min<double, int>(double, int);
template float min<float, int>(float, int);

void printComputeShaderLimits() {
    GLint maxComputeWorkGroupCount[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &maxComputeWorkGroupCount[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &maxComputeWorkGroupCount[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &maxComputeWorkGroupCount[2]);

    GLint maxComputeWorkGroupSize[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &maxComputeWorkGroupSize[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &maxComputeWorkGroupSize[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &maxComputeWorkGroupSize[2]);

    GLint maxComputeWorkGroupInvocations;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &maxComputeWorkGroupInvocations);

    printf("Compute Shader Limits:\n");
    printf("  Max Compute Work Group Count: [%d, %d, %d]\n", maxComputeWorkGroupCount[0], maxComputeWorkGroupCount[1], maxComputeWorkGroupCount[2]);
    printf("  Max Compute Work Group Size: [%d, %d, %d]\n", maxComputeWorkGroupSize[0], maxComputeWorkGroupSize[1], maxComputeWorkGroupSize[2]);
    printf("  Max Compute Work Group Invocations: %d\n", maxComputeWorkGroupInvocations);
}

void printOpenGLInfo() {
    // Get the renderer and vendor information
    const char* renderer = (const char*)glGetString(GL_RENDERER); // Returns the renderer
    const char* vendor = (const char*)glGetString(GL_VENDOR); // Returns the vendor
    const char* version = (const char*)glGetString(GL_VERSION); // Returns the OpenGL version
    const char* shadingLanguageVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION); // Returns the GLSL version

    // Print the information
    printf("GPU Vendor: %s\n", vendor);
    printf("GPU Renderer: %s\n", renderer);
    printf("OpenGL Version: %s\n", version);
    printf("GLSL Version: %s\n", shadingLanguageVersion);
}

GLFWwindow* createWindow(int width, int height, const char* title) {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        return nullptr;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (window == nullptr) {
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    const char* glsl_version = "#version 460 core";

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    gladLoadGL();

    return window;
}

