#include <fstream>
#include <functional>
#include <sstream>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "ScoreboardScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"

void ScoreboardScene::Initialize()
{
    // init curr page
    page = 0;

    spacing = 75;

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // load the previous data of player score
    std::ifstream f("Resource/scoreboard.txt");
    std::string line;
    while (getline(f, line)) {
        std::string name;
        int score;
        std::stringstream ss(line);
        ss >> name >> score;
        info.push_back({name, score});
    }
    f.close();

    page_cap = info.size() / 5;

    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("stage-select/dirt.png",
                                  "stage-select/floor.png", halfW - 200,
                                  (double)halfH * 7 / 4 - 50, 400, 100);

    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 0));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW,
                                   (double)halfH * 7 / 4, 0, 0, 0, 255, 0.5,
                                   0.5));

    // prev and next button to view over one page scoreboard
    btn = new Engine::ImageButton("stage-select/dirt.png",
                                  "stage-select/floor.png", 0,
                                  (double)halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::UpdatePage, this, -1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Previous", "pirulen.ttf", 36, halfW * 0.2,
                                   (double)halfH * 3 / 2, 0, 0, 0, 255, 0.5,
                                   0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png",
                                  "stage-select/floor.png", halfW * 1.5,
                                  (double)halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::UpdatePage, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next", "pirulen.ttf", 36, halfW * 2 - 150,
                                   (double)halfH * 3 / 2, 0, 0, 0, 255, 0.5,
                                   0.5));
    UpdatePage(0);
}
void ScoreboardScene::Terminate()
{
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void ScoreboardScene::Update(float deltaTime) { ticks += deltaTime; }

void ScoreboardScene::UpdatePage(int dir)
{
    page = (dir == 1) ? ++page : --page;
    // boundary case check
    page = (page < 0) ? 0 : page;
    page = (page > page_cap) ? page_cap : page;
}

void ScoreboardScene::Draw() const
{
    Engine::IScene::Draw();
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    int items = (page == page_cap) ? info.size() % 5 : 5;
    for (int i = 0; i < items; ++i) {
        Engine::Label lbl(info[i + page * 5].first, "pirulen.ttf", 48,
                          (double)w / 2 - 150, (double)h / 5 + spacing * i, 0,
                          255, 0, 255, 0.5, 0.5);
        Engine::Label lbl1(std::to_string(info[i + page * 5].second),
                           "pirulen.ttf", 36, (double)w / 2 + 150,
                           (double)h / 5 + spacing * i, 0, 0, 0, 255, 0.5, 0.5);
        lbl.Draw();
        lbl1.Draw();
    }
    Engine::Label curr_page(std::to_string(page + 1) + " / ", "pirulen.ttf", 36,
                            (double)w / 2.05, (double)h * 0.75, 0, 0, 0, 255,
                            0.5, 0.5);
    Engine::Label total_page(std::to_string(page_cap + 1), "pirulen.ttf", 36,
                             (double)w / 1.95, (double)h * 0.75, 0, 0, 0, 255,
                             0.5, 0.5);
    curr_page.Draw();
    total_page.Draw();
}

void ScoreboardScene::BackOnClick(int stage)
{
    // Change to select scene.
    // clear the info vector to prevent memory leak
    info.clear();
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
