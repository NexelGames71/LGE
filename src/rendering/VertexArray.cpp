#include "LGE/rendering/VertexArray.h"
#include "LGE/rendering/VertexBuffer.h"
#include <glad/glad.h>

namespace LGE {

VertexArray::VertexArray() : m_VertexBufferIndex(0) {
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const {
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer) {
    Bind();
    vertexBuffer.Bind();

    // Position attribute (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Color attribute (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    m_VertexBufferIndex++;
}

} // namespace LGE

