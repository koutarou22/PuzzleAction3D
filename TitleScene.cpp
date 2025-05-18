#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"


namespace
{
	static float fadeAlpha = 0.00f; // 黒画像の透明度
	const float fadeSpeed = 0.02f; // フェードの速度
	bool isFadingOut = false;
}
TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"),hBlackScreen_(-1),hImage_(-1)
{

}

void TitleScene::Initialize()
{
	hBlackScreen_ = Image::Load("Scene//Black.png");
	//assert(hBlackScreen_ < 1);

	hImage_ = Image::Load("Scene//Title.png");
	//assert(hImage_ < 1);



	hTitleSound_ = Audio::Load("Sound//loop100804.wav",true,1);
	//assert(hTitleSound_ < 1);

	hStartSound_ = Audio::Load("Sound//SE03.wav",false,1);
	//assert(hStartSound_ < 1);
}

void TitleScene::Update()
{
	Audio::Play(hTitleSound_);

	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		Audio::Stop(hTitleSound_);
		Audio::Play(hStartSound_);
		isFadingOut = true;
	}

	if (isFadingOut)
	{
		fadeAlpha += fadeSpeed;
		if (fadeAlpha >= 1.0f)
		{
			fadeAlpha = 1.0f;
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_LOAD);
		}
	}
}

void TitleScene::Draw()
{
    Image::Draw(hImage_);  

    Image::SetAlpha(hBlackScreen_, fadeAlpha * 255);  
    Image::Draw(hBlackScreen_);
}


void TitleScene::Release()
{

}
