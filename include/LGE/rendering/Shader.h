#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>

namespace LGE {

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform3f(const std::string& name, float v0, float v1, float v2);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4(const std::string& name, const float* matrix);

    uint32_t GetRendererID() const { return m_RendererID; }

private:
    uint32_t CompileShader(uint32_t type, const std::string& source);
    uint32_t CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc);
    int GetUniformLocation(const std::string& name);

    uint32_t m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
};

} // namespace LGE

