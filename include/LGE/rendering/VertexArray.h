#pragma once

#include <cstdint>

namespace LGE {

class VertexBuffer;

class VertexArray {
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddVertexBuffer(const VertexBuffer& vertexBuffer);

    uint32_t GetRendererID() const { return m_RendererID; }

private:
    uint32_t m_RendererID;
    uint32_t m_VertexBufferIndex;
};

} // namespace LGE

