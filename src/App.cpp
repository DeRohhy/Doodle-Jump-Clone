#include "App.h"

#include "singletons/GameSettings.h"
#include "scenes/MainMenu.h"

void App::run() {
    manager.changeScene(std::make_unique<MainMenu>(manager));
    manager.applyPendingScene();

    while (window.isOpen()) {
        float delta = clock.restart().asSeconds();

        Scene* active_scene = manager.getScene();
        if (!active_scene) {
            window.close();
            break;
        }
        
        active_scene->handleEvents(window);
        active_scene->update(delta);

        window.clear();
        active_scene->render(window);
        window.display();

        // safely change to pending scene after the frame
        manager.applyPendingScene();
    }
}