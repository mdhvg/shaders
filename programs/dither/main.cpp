#include "Core.h"
#include "Debug.h"
#include "Shader.h"
#include "Utils.h"
#include "Renderer.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize2.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <functional>

/*
NOTE:
To use NVIDIA GPU, run the following command:
export __GLX_VENDOR_LIBRARY_NAME=nvidia
before running the program.
*/

const int width = 1080;
const int height = 1080;

std::function <void(const char*)> fileDropCallback;

void fileDropCallbackWrapper(GLFWwindow*, int path_count, const char* paths[]) {
	if (path_count != 1) {
		fmt::println("Only one file can be dropped at a time.");
		return;
	}
	fileDropCallback(paths[0]);
}



int main() {
	GLFWwindow* window = createWindow(width, height, "Dither effect");
	// Disable window decoration
	glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
	glfwSetDropCallback(window, fileDropCallbackWrapper);
	MyASSERT(window != nullptr);

	// Setup V-Sync
	glfwSwapInterval(1);

	printOpenGLInfo();
	printComputeShaderLimits();

	std::vector<float> vertices = {
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 0.0f
	};

	std::vector<int> widths = { 3, 2 };

	std::vector<int> indices = {
		0, 1, 2,
		1, 2, 3
	};

	RenderBuffer renderBuffers = generateBuffers(vertices, indices, widths);

	unsigned int vertexShader = compileShader(ROOT_DIR "/rsc/vert.txt", GL_VERTEX_SHADER);
	unsigned int fragmentShader = compileShader(ROOT_DIR "/rsc/frag.txt", GL_FRAGMENT_SHADER);

	unsigned int renderProgram = createRenderProgram(vertexShader, fragmentShader);

	GLCall(glDeleteShader(vertexShader));
	GLCall(glDeleteShader(fragmentShader));

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Nearest filtering for pixelated look
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int textureScaleBase = 2;
	int textureScaleExponent = 1;
	int textureScale = 1;
	int lastTextureScale = textureScale;
	std::string imagePath = std::string(ROOT_DIR "/rsc/img.jpg");

	// Initial texture
	int initialWidth, initialHeight, initialChannels;
	unsigned char* imageData = stbi_load(imagePath.c_str(), &initialWidth, &initialHeight, &initialChannels, 4);
	unsigned char* scaledImageData = new unsigned char[width * height * 4];

	fileDropCallback = [&imagePath, &initialWidth, &initialHeight, &initialChannels, &imageData, &scaledImageData, &textureScale, &texture](const char* path) {
		fmt::println("Reading file: {}", path);
		imagePath = std::string(path);
		if (imageData != nullptr) {
			stbi_image_free(imageData);
			imageData = nullptr;
		}
		if (scaledImageData != nullptr) {
			stbi_image_free(scaledImageData);
			scaledImageData = nullptr;
		}
		imageData = stbi_load(imagePath.c_str(), &initialWidth, &initialHeight, &initialChannels, 4);
		if (imageData == nullptr) {
			fmt::println("Failed to load image: {}", path);
			return;
		}
		scaledImageData = new unsigned char[width * height * 4];
		stbir_resize(imageData,
			initialWidth, initialHeight,
			0,
			scaledImageData,
			width / textureScale, height / textureScale,
			0,
			STBIR_RGBA,
			STBIR_TYPE_UINT8,
			STBIR_EDGE_ZERO,
			STBIR_FILTER_POINT_SAMPLE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width / textureScale, height / textureScale, 0, GL_RGBA, GL_UNSIGNED_BYTE, scaledImageData);
		};

	float startTime = glfwGetTime();
	int ditherSize = 4;
	int paletteSize[3] = { 2, 2, 2 };
	float spread = 0.5f;
	bool grayscale = false;
	bool imageWritten = false;
	std::string savePath;

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		GLCall(glClearColor(0.1f, 0.4f, 0.5f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		/*
		Render Loop here
		*/
		if (textureScale != lastTextureScale) {
			stbir_resize(imageData,
				initialWidth, initialHeight,
				0,
				scaledImageData,
				width / textureScale, height / textureScale,
				0,
				STBIR_RGBA,
				STBIR_TYPE_UINT8,
				STBIR_EDGE_ZERO,
				STBIR_FILTER_POINT_SAMPLE);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width / textureScale, height / textureScale, 0, GL_RGBA, GL_UNSIGNED_BYTE, scaledImageData);
		}
		glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
		GLCall(glUseProgram(renderProgram));
		glUniform2f(glGetUniformLocation(renderProgram, "resolution"), width, height);
		glUniform1i(glGetUniformLocation(renderProgram, "ditherSize"), ditherSize);
		glUniform1i(glGetUniformLocation(renderProgram, "grayscale"), grayscale);
		glUniform3i(glGetUniformLocation(renderProgram, "paletteSize"), paletteSize[0], paletteSize[1], paletteSize[2]);
		glUniform1f(glGetUniformLocation(renderProgram, "spread"), spread);
		GLCall(glBindVertexArray(renderBuffers.vao));
		GLCall(glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr));

		lastTextureScale = textureScale;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGui::ShowDemoWindow();

		{
			ImGui::Begin("Controls");
			ImGui::Text("Press Q or Esc to quit.");
			float frameTime = glfwGetTime();
			ImGui::Text("FPS: %f", 1.0f / (frameTime - startTime));
			ImGui::Text("Frametime: %f", frameTime - startTime);
			startTime = frameTime;
			ImGui::SetTooltip("Drop an image file here to load it.");
			ImGui::DragInt("Texture Scale (^ 2)", &textureScaleExponent, 0.1f, 0, 5);
			textureScale = textureScaleBase << textureScaleExponent;
			ImGui::DragFloat("Spread", &spread, 0.01f, 0.1f, 2.0f, "%.2f");
			ImGui::Checkbox("Grayscale", &grayscale);
			if (grayscale) {
				ImGui::DragInt("Palette Size [Gray]", paletteSize, 0.01f, 2, 32);
			}
			else {
				ImGui::DragInt3("Palette Size [RGB]", paletteSize, 0.01f, 2, 32);
			}
			ImGui::Text("Dither Size");
			ImGui::RadioButton("2x2", &ditherSize, 2);
			ImGui::RadioButton("4x4", &ditherSize, 4);
			ImGui::RadioButton("8x8", &ditherSize, 8);
			if (ImGui::Button("Save Image")) {
				unsigned char* pixels = new unsigned char[width * height * 4];
				glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
				// save to {imagepath (without extension)}.dither.png
				savePath = imagePath.substr(0, imagePath.find_last_of('.'));
				savePath += ".dither.png";
				stbi_flip_vertically_on_write(true);
				stbi_write_png(savePath.c_str(), width, height, 4, pixels, width * 4);
				imageWritten = true;
				delete[] pixels;
			}
			if (imageWritten) {
				ImGui::Text("Image saved to %s", savePath.c_str());
			}
			ImGui::End();
		}

		ImGui::Render();

		if (ImGui::GetIO().WantCaptureKeyboard) {
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
		}

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		GLCall(glfwSwapBuffers(window));
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	if (imageData != nullptr) {
		stbi_image_free(imageData);
		imageData = nullptr;
	}
	if (scaledImageData != nullptr) {
		stbi_image_free(scaledImageData);
		scaledImageData = nullptr;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
