#include "GameSceneFactory.h"

#include "TitleScene/TitleScene.h"
#include "GameScene/GameScene.h"
#include "LevelEditorSample/LevelEditorSample.h"
#include "WinScene/WinScene.h"
#include "LoseScene/LoseScene.h"

IGameScene* GameSceneFactory::CreateScene(const std::string& sceneName){

    //シーンを生成
    IGameScene* newScene = nullptr;

    //タイトル
    if (sceneName == "Title") {
        newScene = new TitleScene();
    }
    //ゲーム
    else if (sceneName == "Game") {
        newScene = new GameScene();
    }
    //勝ち
    else if (sceneName == "Win") {
        newScene = new WinScene();
    }
    //負け
    else if (sceneName == "Lose") {
        newScene = new LoseScene();
    }
    //レベルデータ確認用
    else if (sceneName == "LevelEdhitorSample") {
        newScene = new LevelEditorSample();
    }


    return newScene;
}
