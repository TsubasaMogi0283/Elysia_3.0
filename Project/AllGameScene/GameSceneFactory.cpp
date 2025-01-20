#include "GameSceneFactory.h"

#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "WinScene/WinScene.h"
#include "LoseScene/LoseScene.h"

std::unique_ptr<IGameScene> GameSceneFactory::CreateScene(const std::string& sceneName){

    //シーンを生成
    std::unique_ptr<IGameScene> newScene = nullptr;

    //タイトル
    if (sceneName == "Title") {
        newScene = std::make_unique<TitleScene>();
    }
    //ゲーム
    else if (sceneName == "Game") {
        newScene = std::make_unique<GameScene>();
    }
    //勝ち
    else if (sceneName == "Win") {
        newScene = std::make_unique<WinScene>();
    }
    //負け
    else if (sceneName == "Lose") {
        newScene = std::make_unique<LoseScene>();
    }


    return newScene;
}
