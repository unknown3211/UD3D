#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "globals/global_structs.h"

struct WindowDetails
{
	std::string title;
	uint32_t s_width;
	uint32_t s_height;
	bool vSync;
	bool dev;
};

class Window {
public:
	GLFWwindow* GetGLFWwindow() const { return window; }
	GLFWmonitor* GetGLFWmonitor() const { return monitor; }
	const GLFWvidmode* GetGLFWVideoMode() const { return videoMode; }
	void Start(void(*start)());
	void CreateWindow(const WindowDetails& details);
	void Update(void(*update)());
	void Shutdown(void(*shutdown)());
	void SetBackgroundColor(const Color& color);
	void ResizeWindow(GLFWwindow* window);
	double GetWindowUpTime();

	WindowDetails GetDetails() const { return wd; }
	Color GetBackgroundColor() { return bgColor; }
private:
	WindowDetails wd;
	Color bgColor;
	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;
	const GLFWvidmode* videoMode = nullptr;
};