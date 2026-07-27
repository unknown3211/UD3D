#include "core/window.h"
#include "utils/gl_errors.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

void Window::Start(void(*start)())
{
    start();
    printf("Loaded OpenGL %s\n", glGetString(GL_VERSION));
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window* instance = (Window*)(glfwGetWindowUserPointer(window));
    if (!instance) 
        return;

    auto& inputManager = instance->getInputManager();
    if (action == GLFW_PRESS)
    {
        inputManager.setKeyPressed(key, true);
    }
    else if (action == GLFW_RELEASE)
    {
        inputManager.setKeyPressed(key, false);
    }
}

void Window::CreateWindow(const WindowDetails& details)
{
    wd = details;

    if (!glfwInit())
    {
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (details.dev)
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    }

    monitor = glfwGetPrimaryMonitor();
    videoMode = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(wd.s_width, wd.s_height, wd.title.c_str(), NULL, NULL); // do monitor instead of first NULL if want fullscreen
    if (window == NULL)
    {
        glfwTerminate();
        return;
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return;
    }

    if (details.dev)
        enableReportGlErrors();

    glViewport(0, 0, details.s_width, details.s_height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    bgColor = { 74.f, 120.f, 139.f, 255.f }; // light blue
    SetBackgroundColor(bgColor);

    glfwSwapInterval(details.vSync ? 1 : 0);
}

void Window::Update(void(*update)())
{
    while (!glfwWindowShouldClose(window))
    {
        ResizeWindow(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        update();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::Shutdown(void(*shutdown)())
{
    shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::SetBackgroundColor(const Color& color)
{
    bgColor = color;
    glClearColor(color.r / 255, color.g / 255, color.b / 255, color.a / 255);
}

void Window::ResizeWindow(GLFWwindow* window)
{
    int w = 0, h = 0;
    glfwGetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
}

double Window::GetWindowUpTime()
{
    double seconds = glfwGetTime();
    return seconds;
}