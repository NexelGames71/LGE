#pragma once

namespace LGE {

class UITheme {
public:
    static void ApplyModernTheme();
    
private:
    static void SetupColors();
    static void SetupStyle();
};

} // namespace LGE

