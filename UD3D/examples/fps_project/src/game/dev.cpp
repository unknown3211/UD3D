#include "dev.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <cstdio>

void Dev::Show(bool show) 
{
    s_visible = show;
}

bool Dev::IsVisible() 
{
    return s_visible;
}

void Dev::EnableGui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
}

void Dev::DrawCrosshairEditor()
{
    if (!s_visible) return;

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::Begin("Crosshair Editor", &s_visible,
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_AlwaysAutoResize
    );

    ImGui::End();
}

void Dev::DrawViewmodelTuner(float& offsetX, float& offsetY, float& offsetZ, float& rotX, float& rotY, float& rotZ, float& scale)
{
    if (!s_visible) return;

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
    ImGui::Begin("Viewmodel Tuner", &s_visible,
        ImGuiWindowFlags_NoCollapse | 
        ImGuiWindowFlags_AlwaysAutoResize
    );

    ImGui::SeparatorText("Position");
    ImGui::SliderFloat("Offset X (right)",   &offsetX, -1.0f,   1.0f);
    ImGui::SliderFloat("Offset Y (up)",      &offsetY, -1.0f,   1.0f);
    ImGui::SliderFloat("Offset Z (forward)", &offsetZ,  0.05f,  2.0f);

    ImGui::SeparatorText("Rotation (deg)");
    ImGui::SliderFloat("Rot X (pitch)", &rotX, -180.0f, 180.0f);
    ImGui::SliderFloat("Rot Y (yaw)",   &rotY, -180.0f, 360.0f);
    ImGui::SliderFloat("Rot Z (roll)",  &rotZ, -180.0f, 180.0f);

    ImGui::SeparatorText("Scale");
    ImGui::SliderFloat("Scale", &scale, 0.01f, 0.5f);

    ImGui::Spacing();
    ImGui::SeparatorText("Copy Values");
    char buf[512];
    snprintf(buf, sizeof(buf),
        "offsetX=%.4f offsetY=%.4f offsetZ=%.4f\n"
        "rotX=%.2f rotY=%.2f rotZ=%.2f scale=%.4f",
        offsetX, offsetY, offsetZ, rotX, rotY, rotZ, scale);
    ImGui::InputTextMultiline("##vals", buf, sizeof(buf),
        ImVec2(-1, 52), ImGuiInputTextFlags_ReadOnly);

    ImGui::End();
}
