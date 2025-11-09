#pragma once

#include <cstdint>

namespace LGE {

class VertexBuffer {
public:
    VertexBuffer(const void* data, uint32_t size);
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const;

    uint32_t GetRendererID() const { return m_RendererID; }

private:
    uint32_t m_RendererID;
};

} // namespace LGE

