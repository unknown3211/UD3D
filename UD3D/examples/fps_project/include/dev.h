#pragma once

class Dev
{
public:
    bool s_visible = true;
    void DrawCrosshairEditor();
    void DrawViewmodelTuner(float& offsetX, float& offsetY, float& offsetZ, float& rotX, float& rotY, float& rotZ, float& scale);
    void Show(bool show);
    bool IsVisible();

    void EnableGui();
};
