#pragma once

namespace LGE {

class GameObject;

class Inspector {
public:
    Inspector();
    ~Inspector();

    void OnUIRender();
    
    void SetSelectedObject(GameObject* obj) { m_SelectedObject = obj; }
    GameObject* GetSelectedObject() const { return m_SelectedObject; }

private:
    GameObject* m_SelectedObject;
};

} // namespace LGE


