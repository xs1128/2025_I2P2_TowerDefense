#include <allegro5/allegro5.h>
#include <ctime>
#include <fstream>
#include <functional>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "Scene/PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"
#include "allegro5/allegro_primitives.h"

// key state to track which key is pressed
std::vector<bool> keyState(ALLEGRO_KEY_MAX, false);

void WinScene::Initialize()
{
    name = "";
    ticks = 0;
    input_timer = 0;
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW * 0.35, halfH,
                                   0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 64, halfW * 1.25,
                                   (double)halfH / 2 - 10, 255, 255, 255, 255,
                                   0.5, 0.5));
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW * 0.7,
                                  (double)halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW * 0.7 + 200,
                                   (double)halfH * 7 / 4, 0, 0, 0, 255, 0.5,
                                   0.5));

    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW * 1.3,
                                  (double)halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::SubmitOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Submit", "pirulen.ttf", 48,
                                   halfW * 1.3 + 200, (double)halfH * 7 / 4, 0,
                                   0, 0, 255, 0.5, 0.5));

    AddNewObject(new Engine::Label("Enter your Precious Name", "pirulen.ttf",
                                   48, halfW * 1.3, (double)halfH * 0.8, 0, 0,
                                   0, 255, 0.5, 0.5));

    // load a happy bgm provided
    bgmId = AudioHelper::PlayAudio("win.wav");
}
void WinScene::Terminate()
{
    IScene::Terminate();
    AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime)
{
    ticks += deltaTime;
}

void WinScene::Draw() const
{
    IScene::Draw();
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;

    // Draw the text box rectangle
    double box_x = (double)w / 2.5;
    double box_y = (double)h / 2;
    double box_w = 700;
    double box_h = 150;
    al_draw_filled_rectangle(box_x, box_y, box_x + box_w, box_y + box_h,
                             al_map_rgb(255, 255, 255));
    al_draw_rectangle(box_x, box_y, box_x + box_w, box_y + box_h,
                      al_map_rgb(0, 0, 0), 2);

    Engine::Label input_lbl(name, "pirulen.ttf", 56, box_x * 1.05,
                            box_y + box_h / 2, 0, 0, 0, 255, 0, 0.5);
    input_lbl.Draw();
}

void WinScene::BackOnClick(int stage)
{
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void WinScene::SubmitOnClick()
{
    // Submit the name to the scoreboard.
    if (name.length() > 0) {
        std::ofstream f("Resource/scoreboard.txt", std::ios::app);
        if (!f) {
            Engine::LOG(Engine::WARN) << "Scoreboard file open failed";
            return;
        }
        int money = dynamic_cast<PlayScene *>(
                        Engine::GameEngine::GetInstance().GetScene("play"))
                        ->GetMoney();
        // get current date and time
        // TODO - cosider changing to chrono later on
        std::time_t t = std::time(nullptr);
        std::tm *now = std::localtime(&t);
        char datetime[32];
        std::strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S", now);
        std::cout << datetime << std::endl;
        // write name, money, and datetime to file
        f << name << " " << money << " " << datetime << "\n";

        f.close();
    }
}

void WinScene::OnKeyDown(int keyCode)
{
    IScene::OnKeyDown(keyCode);
    if (keyCode >= ALLEGRO_KEY_A && keyCode <= ALLEGRO_KEY_Z &&
        name.length() <= 10) {
        name += keyCode - ALLEGRO_KEY_A + 'A';
        keyState[keyCode] = false;
    }
    else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9 &&
             name.length() <= 10) {
        name += keyCode - ALLEGRO_KEY_A + '0';
        keyState[keyCode] = false;
    }
    else if (keyCode == ALLEGRO_KEY_BACKSPACE && !name.empty() &&
             input_timer == 0) {
        name.pop_back();
        input_timer = 8;
    }
}
