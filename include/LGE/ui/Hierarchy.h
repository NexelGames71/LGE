#pragma once

#include <vector>
#include <memory>

namespace LGE {

class GameObject;

class Hierarchy {
public:
    Hierarchy();
    ~Hierarchy();

    void OnUIRender();
    
    void SetGameObjects(const std::vector<std::shared_ptr<GameObject>>& objects) { m_GameObjects = objects; }
    void SetSelectedObject(GameObject* obj) { m_SelectedObject = obj; }
    GameObject* GetSelectedObject() const { return m_SelectedObject; }

private:
    std::vector<std::shared_ptr<GameObject>> m_GameObjects;
    GameObject* m_SelectedObject;
};

} // namespace LGE


