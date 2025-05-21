#include <algorithm>
#include <filesystem>
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
    std::string filepath = "Resource/scoreboard.txt";
    // init curr page
    page = 0;

    spacing = 75;

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // load the previous data of player score
    std::ifstream f(filepath);
    std::string line;
    while (getline(f, line)) {
        if (!line.empty()) {
            std::string name, date, time;
            int score;
            std::stringstream ss(line);
            ss >> name >> score >> date >> time;

            info.push_back({name, score, date + " " + time});
        }
    }
    f.close();
    // sort the info according to score
    // use built-in sort and lambda for comparison
    sort(info.begin(), info.end(), [](const auto &a, const auto &b) {
        return std::get<1>(a) > std::get<1>(b);
    });

    page_cap = (info.size() == 0) ? 0 : (info.size() + 4) / 5 - 1;
    page_cap = (page_cap == 0) ? 1 : page_cap;

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

    bgmId = AudioHelper::PlayBGM("happy.ogg");
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

    Engine::Label name_lbl("Name", "pirulen.ttf", 56, (double)w / 6,
                           (double)h / 8, 0, 0, 0, 255, 0.5, 0.5);
    Engine::Label score_lbl("Score", "pirulen.ttf", 56, (double)w / 2.5,
                            (double)h / 8, 0, 0, 0, 255, 0.5, 0.5);
    Engine::Label datetime_lbl("Date & Time", "pirulen.ttf", 56, (double)w / 1.4,
                            (double)h / 8, 0, 0, 0, 255, 0.5, 0.5);
    name_lbl.Draw();
    score_lbl.Draw();
    datetime_lbl.Draw();

    // basically the items either 1 to 5 per page and the case 0 is handled by diving previously to get page_cap
    int items = (page == page_cap) ? ((info.size() % 5 == 0) ? 5 : info.size() % 5) : 5;
    for (int i = 0; i < items; ++i) {
        auto [name, score, date] = info[i + page * 5];
        Engine::Label lbl(name, "pirulen.ttf", 48, (double)w / 6,
                          (double)h / 4 + spacing * i, 255, 255, 255, 255, 0.5,
                          0.5);
        Engine::Label lbl1(std::to_string(score), "pirulen.ttf", 48,
                           (double)w / 2.5, (double)h / 4 + spacing * i,
                           255, 255, 255, 255, 0.5, 0.5);
        Engine::Label lbl2(date,  "pirulen.ttf", 48,
                           (double)w / 1.4, (double)h / 4 + spacing * i,
                           255, 255, 255, 255, 0.5, 0.5);
        lbl.Draw();
        lbl1.Draw();
        lbl2.Draw();
    }
    Engine::Label curr_page(std::to_string(page + 1), "pirulen.ttf", 36,
                            (double)w / 2.2, (double)h * 0.75, 0, 0, 0, 255,
                            0.5, 0.5);
    Engine::Label total_page(" / " + std::to_string(page_cap + 1),
                             "pirulen.ttf", 36, (double)w / 2, (double)h * 0.75,
                             0, 0, 0, 255, 0.5, 0.5);
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
