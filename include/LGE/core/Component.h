#pragma once

#include <string>

namespace LGE {

class GameObject;

// Base component class
class Component {
public:
    Component();
    virtual ~Component();

    // Component name/type
    virtual const std::string& GetTypeName() const = 0;
    
    // Owner GameObject
    void SetOwner(GameObject* owner) { m_Owner = owner; }
    GameObject* GetOwner() const { return m_Owner; }
    
    // Enable/disable
    void SetEnabled(bool enabled) { m_Enabled = enabled; }
    bool IsEnabled() const { return m_Enabled; }

protected:
    GameObject* m_Owner;
    bool m_Enabled;
};

} // namespace LGE

