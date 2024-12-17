#include "Core.h"
#include "Debug.h"
#include "Shader.h"
#include "Utils.h"
#include "Renderer.h"

/*
NOTE:
To use NVIDIA GPU, run the following command:
export __GLX_VENDOR_LIBRARY_NAME=nvidia
before running the program.
*/

const int width = 640;
const int height = 640;

int main() {
    GLFWwindow* window = createWindow(width, height, "Raytracer");
    MyASSERT(window != nullptr);

    // Setup V-Sync
    glfwSwapInterval(1);

    printOpenGLInfo();
    printComputeShaderLimits();

    std::vector<float> vertices = {
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f
    };

    std::vector<int> widths = { 3, 2 };

    std::vector<int> indices = {
        0, 1, 2,
        1, 2, 3
    };

    RenderBuffer renderBuffers = generateBuffers(vertices, indices, widths);

    unsigned int vertexShader = compileShader(ROOT_DIR "/rsc/vert.txt", GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(ROOT_DIR "/rsc/frag.txt", GL_FRAGMENT_SHADER);
    unsigned int computeShader = compileShader(ROOT_DIR "/rsc/raytrace.txt", GL_COMPUTE_SHADER);

    unsigned int renderProgram = createRenderProgram(vertexShader, fragmentShader);
    unsigned int computeProgram = createComputeProgram(computeShader);

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));
    GLCall(glDeleteShader(computeShader));

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);

    float startTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        GLCall(glClearColor(0.1f, 0.4f, 0.5f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));

        /*
        Render Loop here
        */
        GLCall(glUseProgram(computeProgram));
        glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glDispatchCompute(width / 8, height / 8, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        GLCall(glUseProgram(renderProgram));
        GLCall(glBindVertexArray(renderBuffers.vao));
        GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr));

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("FPS");
            float frameTime = glfwGetTime();
            ImGui::Text("FPS: %f", 1.0f / (frameTime - startTime));
            ImGui::Text("Frametime: %f", frameTime - startTime);
            startTime = frameTime;

            ImGui::End();
        }

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        GLCall(glfwSwapBuffers(window));
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
