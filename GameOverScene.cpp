#include "GameOverScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"


GameOverScene::GameOverScene(GameObject* parent)
	: GameObject(parent, "GameOverScene")
{

}

void GameOverScene::Initialize()
{
	hImage_ = Image::Load("Scene/GameOver.png");
	assert(hImage_ < 1);

	hGameOverBGM_ = Audio::Load("Sound//BGM//CLEAR_BGM.wav", true, 1);
	assert(hGameOverBGM_ >= 0);
}

void GameOverScene::Update()
{

	Audio::Play(hGameOverBGM_);
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void GameOverScene::Draw()
{
	Image::Draw(hImage_);
}

void GameOverScene::Release()
{
}
