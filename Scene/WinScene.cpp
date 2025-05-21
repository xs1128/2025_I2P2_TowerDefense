#include <allegro5/allegro5.h>
#include <functional>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "PlayScene.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "WinScene.hpp"
#include "allegro5/keyboard.h"

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
    AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW * 0.3, halfH,
                                   0, 0, 0.5, 0.5));
    AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW,
                                   (double)halfH / 4 - 10, 255, 255, 255, 255,
                                   0.5, 0.5));
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200,
                                  (double)halfH * 7 / 4 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW,
                                   (double)halfH * 7 / 4, 0, 0, 0, 255, 0.5,
                                   0.5));

    // TODO - Put a text box to enter name
    AddNewObject(new Engine::Label(
        "Please Enter your Precious Name", "pirulen.ttf", 36, halfW * 1.2,
        (double)halfH * 0.6, 0, 0, 0, 255, 0.5, 0.5));

    AddNewObject(new Engine::Label(name, "pirulen.ttf", 36, halfW * 1.2,
                                   (double)halfH * 1.5, 0, 0, 0, 255, 0.5,
                                   0.5));

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

    if (keyState[ALLEGRO_KEY_BACKSPACE] && name.size() > 0 &&
        input_timer == 0) {
        name[name.size() - 1] = '\0';
        input_timer = 10;
    }
    for (int i = ALLEGRO_KEY_A; i <= ALLEGRO_KEY_Z && name.size() <= 10; ++i) {
        if (keyState[i]) {
            name[name.size() - 1] = i - 1 + 'A';
            keyState[i] = false;
        }
    }
}
void WinScene::BackOnClick(int stage)
{
    // Change to select scene.
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
