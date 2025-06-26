#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"


namespace
{
    static float fadeAlpha = 0.00f; // 黒画像の透明度
    const float fadeSpeed = 0.02f; // フェードの速度
    bool isFadingOut;
    bool isLoading;
    int loadWaitTimer;
}

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"),hBlackScreen_(-1),hImage_(-1)
{

}

void TitleScene::Initialize()
{
    fadeAlpha = 0.0f;  // **明示的にリセット**
    isFadingOut = false;
    isLoading = false;
    loadWaitTimer = 60;

    hBlackScreen_ = Image::Load("Scene//Black.png");
    hImage_ = Image::Load("Scene//Title.png");

    hTitleSound_ = Audio::Load("Sound//loop100804.wav", true, 1);
    hStartSound_ = Audio::Load("Sound//SE03.wav", false, 1);
}


void TitleScene::Update()
{
    if (fadeAlpha == 0.0f) 
    {
        isFadingOut = false;
    }

   // Audio::Play(hTitleSound_);

    if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_START))
    {
       
        Audio::Play(hStartSound_);
        isFadingOut = true;
    }

    if (isFadingOut)
    {
        fadeAlpha += fadeSpeed;
        if (fadeAlpha >= 1.0f)
        {
            fadeAlpha = 1.0f;
            //Audio::Stop(hTitleSound_);
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
