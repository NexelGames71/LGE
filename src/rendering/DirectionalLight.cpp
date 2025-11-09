#include "LGE/rendering/DirectionalLight.h"

namespace LGE {

DirectionalLight::DirectionalLight()
    : m_Direction(0.0f, -1.0f, 0.0f)  // Pointing down by default
    , m_Color(1.0f, 1.0f, 1.0f)       // White light
    , m_Intensity(1.0f)
{
}

DirectionalLight::DirectionalLight(const Math::Vector3& direction, const Math::Vector3& color, float intensity)
    : m_Direction(direction)
    , m_Color(color)
    , m_Intensity(intensity)
{
}

} // namespace LGE

