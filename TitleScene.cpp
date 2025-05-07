#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"


TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene")
{

}

void TitleScene::Initialize()
{
	hImage_ = Image::Load("Scene//Title.png");
	assert(hImage_ < 1);

	hTitleSound_ = Audio::Load("Sound//loop100804.wav",true,1);
	assert(hTitleSound_ < 1);

	hStartSound_ = Audio::Load("Sound//SE03.wav",false,1);
	//assert(hStartSound_ < 1);
}

void TitleScene::Update()
{
	Audio::Play(hTitleSound_);

	if (Input::IsKeyDown(DIK_SPACE)||Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		Audio::Stop(hTitleSound_);
		Audio::Play(hStartSound_);
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
