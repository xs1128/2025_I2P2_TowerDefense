#include "IScene.hpp"
#include <allegro5/allegro.h>

namespace Engine {
    void IScene::Terminate() {
        Clear();
    }
    void IScene::Draw() const {
        // change background here
        al_clear_to_color(al_map_rgb(100, 0, 0));
        Group::Draw();
    }
}
