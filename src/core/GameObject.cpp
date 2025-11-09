#include "LGE/core/GameObject.h"
#include <cmath>

namespace LGE {

uint32_t GameObject::s_NextID = 1;

GameObject::GameObject(const std::string& name)
    : m_Name(name)
    , m_ID(s_NextID++)
    , m_Position(0.0f, 0.0f, 0.0f)
    , m_Rotation(0.0f, 0.0f, 0.0f)
    , m_Scale(1.0f, 1.0f, 1.0f)
    , m_TransformDirty(true)
    , m_Selected(false)
{
}

GameObject::~GameObject() {
}

void GameObject::UpdateTransformMatrix() {
    if (!m_TransformDirty) return;
    
    // Build transform matrix: T * R * S
    Math::Matrix4 translation = Math::Matrix4::Translate(m_Position);
    
    // Rotation as Euler angles (in degrees)
    float rx = m_Rotation.x * 3.14159f / 180.0f;
    float ry = m_Rotation.y * 3.14159f / 180.0f;
    float rz = m_Rotation.z * 3.14159f / 180.0f;
    
    Math::Matrix4 rotX = Math::Matrix4::Rotate(rx, Math::Vector3(1.0f, 0.0f, 0.0f));
    Math::Matrix4 rotY = Math::Matrix4::Rotate(ry, Math::Vector3(0.0f, 1.0f, 0.0f));
    Math::Matrix4 rotZ = Math::Matrix4::Rotate(rz, Math::Vector3(0.0f, 0.0f, 1.0f));
    Math::Matrix4 rotation = rotZ * rotY * rotX;
    
    Math::Matrix4 scale = Math::Matrix4::Scale(m_Scale);
    
    m_TransformMatrix = translation * rotation * scale;
    m_TransformDirty = false;
}

const Math::Matrix4& GameObject::GetTransformMatrix() {
    UpdateTransformMatrix();
    return m_TransformMatrix;
}

} // namespace LGE

