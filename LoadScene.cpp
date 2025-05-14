#include "LoadScene.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"

LoadScene::LoadScene(GameObject* parent)
	: GameObject(parent, "LoadScene")
{
}

void LoadScene::Initialize()
{
	hImage_ = Image::Load("Scene//LoadTest.png");
	assert(hImage_ < 1);
}

void LoadScene::Update()
{
	if (Input::IsKeyDown(DIK_P) || Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_PLAY);
	}
}

void LoadScene::Draw()
{
	Image::Draw(hImage_);
}

void LoadScene::Release()
{
}
