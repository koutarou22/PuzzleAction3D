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
	hImage_ = Image::Load("Image/Title.png");
	assert(hImage_ < 1);
}

void TitleScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
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
