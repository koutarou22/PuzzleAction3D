#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"


TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{

}

void TitleScene::Initialize()
{
	hImage_ = Image::Load("Scene/Title.png");
	assert(hImage_ < 1);
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE)||Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void TitleScene::Draw()
{
	Image::Draw(hImage_);
}

void TitleScene::Release()
{

}
