#pragma once

#include <memory>

#include "scenes/Scene.h"

class SceneManager {
public:
    void changeScene(std::unique_ptr<Scene> new_scene) { pending_scene = std::move(new_scene); }
    void applyPendingScene() {
        if (pending_scene)
        {
            active_scene = std::move(pending_scene);
            active_scene->start();
        }
    }

    Scene* getScene() { return active_scene.get(); }
private:
    std::unique_ptr<Scene> active_scene;
    std::unique_ptr<Scene> pending_scene;
};