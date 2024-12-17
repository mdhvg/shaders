#include <vector>

struct RenderBuffer {
    unsigned int vao, vbo, ebo;
};

RenderBuffer generateBuffers(
    std::vector<float> vertices,
    std::vector<int> indices,
    std::vector<int> widths
);