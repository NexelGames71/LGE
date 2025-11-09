#pragma once

#include "LGE/rendering/Camera.h"
#include <cstdint>

namespace LGE {

class CameraController {
public:
    CameraController(Camera* camera);
    ~CameraController() = default;

    void OnUpdate(float deltaTime);
    void OnWindowResize(uint32_t width, uint32_t height);

    void SetMovementSpeed(float speed) { m_MovementSpeed = speed; }
    void SetPanSpeed(float speed) { m_PanSpeed = speed; }
    void SetZoomSpeed(float speed) { m_ZoomSpeed = speed; }
    void SetMouseSensitivity(float sensitivity) { m_MouseSensitivity = sensitivity; }
    void SetZoomLimits(float minDistance, float maxDistance) { m_MinZoomDistance = minDistance; m_MaxZoomDistance = maxDistance; }

    float GetMovementSpeed() const { return m_MovementSpeed; }
    float GetPanSpeed() const { return m_PanSpeed; }
    float GetZoomSpeed() const { return m_ZoomSpeed; }

private:
    void ProcessKeyboard(float deltaTime);
    void ProcessMouse(float deltaTime);
    void ProcessScroll(float deltaTime);

    Camera* m_Camera;
    
    float m_MovementSpeed;
    float m_PanSpeed;
    float m_ZoomSpeed;
    float m_MouseSensitivity;
    
    // Zoom limits
    float m_MinZoomDistance;
    float m_MaxZoomDistance;
    
    // Mouse state
    double m_LastMouseX;
    double m_LastMouseY;
    bool m_FirstMouse;
    
    // Camera rotation
    float m_Yaw;
    float m_Pitch;
};

} // namespace LGE

