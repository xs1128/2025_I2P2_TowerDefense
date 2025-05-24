#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

class WinScene final : public Engine::IScene {
  private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    std::string name;
    int input_timer;
    double textbox_x, textbox_y, textbox_w, textbox_h;

  public:
    explicit WinScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Draw() const override;
    void BackOnClick(int stage);
    void OnKeyDown(int keyCode) override;
    void SubmitOnClick();
};

#endif // WINSCENE_HPP
