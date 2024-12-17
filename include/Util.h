#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

#endif // UTIL_H