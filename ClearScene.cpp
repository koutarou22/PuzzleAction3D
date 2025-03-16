#include "ClearScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"

ClearScene::ClearScene(GameObject* parent)
	: GameObject(parent, "ClearScene")
{

}

void ClearScene::Initialize()
{
	hImage_ = Image::Load("Scene/Clear.png");
	assert(hImage_ < 1);
}

void ClearScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void ClearScene::Draw()
{
	Image::Draw(hImage_);
}

void ClearScene::Release()
{
}
