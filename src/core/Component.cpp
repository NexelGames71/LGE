#include "LGE/core/Component.h"

namespace LGE {

Component::Component()
    : m_Owner(nullptr)
    , m_Enabled(true)
{
}

Component::~Component() {
}

} // namespace LGE

