#pragma once

#include "LGE/rendering/Shader.h"
#include "LGE/rendering/VertexBuffer.h"
#include "LGE/rendering/VertexArray.h"
#include "LGE/rendering/Texture.h"
#include "LGE/math/Vector.h"
#include "LGE/math/Matrix.h"
#include <memory>
#include <string>

namespace LGE {

class Skybox {
public:
    Skybox();
    ~Skybox();

    void Init();
    bool LoadHDR(const std::string& filepath);
    void Render(const class Camera& camera, float cloudOffset = 0.0f);
    void SetTopColor(const Math::Vector3& color) { m_TopColor = color; }
    void SetBottomColor(const Math::Vector3& color) { m_BottomColor = color; }

private:
    std::unique_ptr<Shader> m_Shader;
    std::unique_ptr<VertexBuffer> m_VertexBuffer;
    std::unique_ptr<VertexArray> m_VertexArray;
    std::unique_ptr<Texture> m_Texture;
    
    Math::Vector3 m_TopColor;
    Math::Vector3 m_BottomColor;
    bool m_Initialized;
    bool m_HasTexture;
};

} // namespace LGE

