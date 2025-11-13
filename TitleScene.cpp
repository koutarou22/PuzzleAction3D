#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include "Fadein.h"


TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hBlackScreen_(-1), hBackTitleImage_(-1), hTitleSound_(-1)
{

}

void TitleScene::Initialize()
{
    Instantiate<Fadein>(this);

	TitleSoundFileName_[TITLE_SOUND_BGM] = "20250630020514.wav"; // タイトルBGMのファイル名を設定

    hBackTitleImage_ = Image::Load("Scene//Title1.png");
    assert(hBackTitleImage_ >= 0);

    hTitleSound_ = Audio::Load(SoundFilePath_ + TitleSoundFileName_[TITLE_SOUND_BGM], true, 1);
	assert(hTitleSound_ >= 0);
   
}


void TitleScene::Update()
{

    SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Fadein* pFadein = (Fadein*)FindObject("Fadein");

	// タイトルBGMが再生されていない場合は再生
    Audio::Play(hTitleSound_);


	//押されたらフェードアウト開始
    if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_START))
    {  
		
        //開始音を鳴らす
		pFadein->PlayStartSound(); 
        // フェードアウト開始
		pFadein->SetFadeOut(true); 
    }

    if (pFadein != nullptr)
    {
        bool isFadeOut = pFadein->GetFadeOut();
        if (isFadeOut)
        {
            pFadein->FadeIn(SCENE_ID_LOAD);
            return;
        }
    }
  
}

void TitleScene::Draw()
{
    Image::Draw(hBackTitleImage_);
}


void TitleScene::Release()
{

}
