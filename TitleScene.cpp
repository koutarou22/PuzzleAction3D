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

	TitleSoundFileName_[TITLE_SOUND_BGM] = "20250630020514.wav"; // �^�C�g��BGM�̃t�@�C������ݒ�

    hBackTitleImage_ = Image::Load("Scene//Title.png");
    assert(hBackTitleImage_ >= 0);

    hTitleSound_ = Audio::Load(SoundFilePath_ + TitleSoundFileName_[TITLE_SOUND_BGM], true, 1);
	assert(hTitleSound_ >= 0);
   
}


void TitleScene::Update()
{

    SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Fadein* pFadein = (Fadein*)FindObject("Fadein");

	// �^�C�g��BGM���Đ�����Ă��Ȃ��ꍇ�͍Đ�
    Audio::Play(hTitleSound_);


	//�����ꂽ��t�F�[�h�A�E�g�J�n
    if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_START))
    {  
		
        //�J�n����炷
		pFadein->PlayStartSound(); 
        // �t�F�[�h�A�E�g�J�n
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
