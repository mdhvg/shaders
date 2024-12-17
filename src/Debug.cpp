#include "Debug.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fmt/core.h>
#include <iostream>

/**
	Clear all (unrelated) previous errors.
*/
void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

/**
	Check for an error and log the error to the console.

	@param function The function where the error happend
	@param file The file where the error happend
	@param line The line number where the error happend
	@return Whether there was an error
*/
bool GLLogCall(const char* function, const char* file, int line) {
	GLenum error = glGetError();

	if (error) {
		fmt::print("[OpenGL Error] (0x{:X}) in {} at {} on line {}\n",
			error, function, file, line);
		return false;
	}

	return true;
}

void glfw_error_callback(int error, const char* description) {
	std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}