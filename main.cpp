// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.h"
#include "Scene/SettingsScene.hpp"

int main(int argc, char **argv) {
	// set boolean for log
	Engine::LOG::SetConfig(true);
	// Engine namespace create instance of game engine
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // DONE HACKATHON-2 (2/3): Register Scenes here
    game.AddNewScene("stage-select", new StageSelectScene());
	game.AddNewScene("play", new PlayScene());
	game.AddNewScene("lose", new LoseScene());
	game.AddNewScene("win", new WinScene());
	game.AddNewScene("start", new StartScene());
	game.AddNewScene("settings", new SettingsScene());

    // DONE HACKATHON-1 (1/1): Change the start scene
	// init everything and start event loop until end
	game.Start("start", 60, 1600, 832);
	return 0;
}
