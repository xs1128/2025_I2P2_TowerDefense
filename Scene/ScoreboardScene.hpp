#ifndef SCOREBOARDSCENE_HPP
#define SCOREBOARDSCENE_HPP
#include "Engine/IScene.hpp"
#include "UI/Component/Label.hpp"
#include <allegro5/allegro_audio.h>

class ScoreboardScene final : public Engine::IScene {
  private:
    float ticks;
    ALLEGRO_SAMPLE_ID bgmId;
    int page;
    int page_cap;
    std::vector<std::pair<std::string, int>> info;

    int spacing;

  public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void UpdatePage(int dir);
    void Draw() const override;
    void BackOnClick(int stage);
};

#endif // SCOREBOARDSCENE_HPP
