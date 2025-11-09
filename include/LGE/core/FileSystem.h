#pragma once

#include <string>

namespace LGE {

class FileSystem {
public:
    static std::string ReadFile(const std::string& filepath);
};

} // namespace LGE

