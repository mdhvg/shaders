#include "Renderer.h"
#include "Debug.h"
#include <glad/glad.h>

RenderBuffer generateBuffers(
    std::vector<float> vertices,
    std::vector<int> indices,
    std::vector<int> widths
) {
    RenderBuffer buffer;
    GLCall(glGenVertexArrays(1, &buffer.vao));
    GLCall(glBindVertexArray(buffer.vao));

    GLCall(glGenBuffers(1, &buffer.vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo));
    GLCall(glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(vertices[0]),
        vertices.data(),
        GL_STATIC_DRAW
    ));

    GLCall(glGenBuffers(1, &buffer.ebo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.ebo));
    GLCall(glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(indices[0]),
        indices.data(),
        GL_STATIC_DRAW
    ));

    int stride = 0;
    for (auto& width : widths) {
        stride += width;
    }
    int cur = 0;
    for (int i = 0; i < widths.size(); i++) {
        GLCall(glVertexAttribPointer(
            i,
            widths[i],
            GL_FLOAT,
            GL_FALSE,
            stride * sizeof(vertices[0]),
            (void*)(intptr_t)cur
        ));
        GLCall(glEnableVertexAttribArray(i));
        cur += widths[i] * sizeof(vertices[0]);
    }
    return buffer;
}