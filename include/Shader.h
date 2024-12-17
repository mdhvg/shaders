#include <string>

std::string readFile(const std::string& filename);

unsigned int compileShader(const std::string& filename, unsigned int type);
unsigned int createRenderProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int createComputeProgram(unsigned int computeShader);