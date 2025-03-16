#include "GameOverScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"


GameOverScene::GameOverScene(GameObject* parent)
	: GameObject(parent, "GameOverScene")
{

}

void GameOverScene::Initialize()
{
	hImage_ = Image::Load("Scene/GameOver.png");
	assert(hImage_ < 1);
}

void GameOverScene::Update()
{
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
