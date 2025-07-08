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
	: GameObject(parent, "TitleScene"),hBlackScreen_(-1), hBackTitleImage_(-1)
{

}

void TitleScene::Initialize()
{
    fadeAlpha = 0.0f; 
    isFadingOut = false;
    isLoading = false;
    loadWaitTimer = 60;

    hBlackScreen_ = Image::Load("Scene//Black.png");
    assert(hBlackScreen_ >= 0);
    hBackTitleImage_ = Image::Load("Scene//Title.png");
    assert(hBackTitleImage_ >= 0);

    hTitleSound_ = Audio::Load("Sound//BGM//20250630020514.wav", true, 1);
    hStartSound_ = Audio::Load("Sound//SE//SE03.wav", false, 1);
}


void TitleScene::Update()
{

    SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

    if (fadeAlpha == 0.0f) 
    {
        isFadingOut = false;
    }

   // Audio::Play(hTitleSound_);

    if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_START))
    {
       
        //Audio::Play(hStartSound_);
        isFadingOut = true;
    }

    if (isFadingOut)
    {
        fadeAlpha += fadeSpeed;
        if (fadeAlpha >= 1.0f)
        {
            fadeAlpha = 1.0f;
            //Audio::Stop(hTitleSound_);
            
			//毎回タイトルシーンに戻ったときにステージ番号をリセット
            pSceneManager->ResetStageNumber();
          
            pSceneManager->ChangeScene(SCENE_ID_LOAD);

          
        }
    }
}

void TitleScene::Draw()
{
    Image::Draw(hBackTitleImage_);

    Image::SetAlpha(hBlackScreen_, fadeAlpha * 255);  
    Image::Draw(hBlackScreen_);
}


void TitleScene::Release()
{

}
