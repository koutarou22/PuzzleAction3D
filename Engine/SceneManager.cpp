#include "sceneManager.h"

#include "Model.h"
#include "Image.h"
#include "Audio.h"
#include "../TitleScene.h"
#include "../PlayScene.h"
#include "../ClearScene.h"
#include "../GameOverScene.h"
#include "../LoadScene.h"
#include "../Player.h"


SceneManager::SceneManager(GameObject * parent)
	: GameObject(parent, "SceneManager"),StageClearCounter_(1)
{
}

void SceneManager::Initialize()
{
	//最初のシーンを準備
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う　＝　シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		//そのシーンのオブジェクトを全削除
		KillAllChildren();

		//ロードしたデータを全削除 
		Audio::Release();
		Model::AllRelease();
		Image::AllRelease();

		//次のシーンを作成
		switch (nextSceneID_)
		{
		case SCENE_ID_TITLE: Instantiate<TitleScene>(this); break;
		case SCENE_ID_PLAY: Instantiate<PlayScene>(this); break;
		case SCENE_ID_LOAD: Instantiate<LoadScene>(this);break;
		case SCENE_ID_CLEAR: Instantiate<ClearScene>(this); break;
		case SCENE_ID_GAMEOVER: Instantiate<GameOverScene>(this); break;
		}
		Audio::Initialize();
		currentSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

//シーン切り替え（実際に切り替わるのはこの次のフレーム）
void SceneManager::ChangeScene(SCENE_ID next)
{
	nextSceneID_ = next;
}

void SceneManager::SetPlayerRemain(int Remain)
 {
        //残機が 1 未満になったら初期値にリセット
        if (Remain <= 0)
        {
            Remain = Remain;
        }

        if (Remain > MAX_Remain)
        {
            Remain = MAX_Remain;
        }

        playerRemain_ = Remain;
    }

void SceneManager::ResetStageNumber()
{
	StageClearCounter_ = 1;
}

void SceneManager::ResetRemain()
{
	playerRemain_ = PLAYER_INITIALIZE_Remain;
}
