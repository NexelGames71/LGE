#include "LGE/rendering/Shader.h"
#include "LGE/core/Log.h"
#include <glad/glad.h>
#include <sstream>
#include <fstream>

namespace LGE {

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    m_RendererID = CreateProgram(vertexSrc, fragmentSrc);
}

Shader::~Shader() {
    glDeleteProgram(m_RendererID);
}

void Shader::Bind() const {
    glUseProgram(m_RendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value) {
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value) {
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2) {
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4(const std::string& name, const float* matrix) {
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, matrix);
}

uint32_t Shader::CompileShader(uint32_t type, const std::string& source) {
    uint32_t shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        Log::Error("Shader compilation failed: " + std::string(infoLog));
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

uint32_t Shader::CreateProgram(const std::string& vertexSrc, const std::string& fragmentSrc) {
    uint32_t program = glCreateProgram();
    uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexSrc);
    uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

    if (vs == 0 || fs == 0) {
        return 0;
    }

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        Log::Error("Shader program linking failed: " + std::string(infoLog));
        glDeleteProgram(program);
        glDeleteShader(vs);
        glDeleteShader(fs);
        return 0;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        Log::Warn("Uniform '" + name + "' not found in shader!");
    }
    m_UniformLocationCache[name] = location;
    return location;
}

} // namespace LGE

