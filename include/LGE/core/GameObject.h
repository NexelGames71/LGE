#pragma once

#include "LGE/math/Vector.h"
#include "LGE/math/Matrix.h"
#include <string>
#include <cstdint>

namespace LGE {

class GameObject {
public:
    GameObject(const std::string& name = "GameObject");
    ~GameObject();

    // Transform
    void SetPosition(const Math::Vector3& position) { m_Position = position; m_TransformDirty = true; }
    void SetRotation(const Math::Vector3& rotation) { m_Rotation = rotation; m_TransformDirty = true; }
    void SetScale(const Math::Vector3& scale) { m_Scale = scale; m_TransformDirty = true; }
    
    Math::Vector3 GetPosition() const { return m_Position; }
    Math::Vector3 GetRotation() const { return m_Rotation; }
    Math::Vector3 GetScale() const { return m_Scale; }
    
    const Math::Matrix4& GetTransformMatrix();
    
    // Selection
    void SetSelected(bool selected) { m_Selected = selected; }
    bool IsSelected() const { return m_Selected; }
    
    // Name
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }
    
    // ID for picking
    uint32_t GetID() const { return m_ID; }

private:
    void UpdateTransformMatrix();
    
    std::string m_Name;
    uint32_t m_ID;
    
    Math::Vector3 m_Position;
    Math::Vector3 m_Rotation; // Euler angles in degrees
    Math::Vector3 m_Scale;
    
    Math::Matrix4 m_TransformMatrix;
    bool m_TransformDirty;
    bool m_Selected;
    
    static uint32_t s_NextID;
};

} // namespace LGE

