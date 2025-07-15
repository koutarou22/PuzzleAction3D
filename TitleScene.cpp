#include "TitleScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include "Fadein.h"


TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hBlackScreen_(-1), hBackTitleImage_(-1), hTitleSound_(-1), hStartSound_(-1), IsPlaySE_(false)
{

}

void TitleScene::Initialize()
{
    Instantiate<Fadein>(this);


    hBackTitleImage_ = Image::Load("Scene//Title.png");
    assert(hBackTitleImage_ >= 0);

    hTitleSound_ = Audio::Load("Sound//BGM//20250630020514.wav", true, 1);
    hStartSound_ = Audio::Load("Sound//SE//SE03.wav", false, 1);
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
		//��񂵂��Đ����Ȃ��悤�ɂ��鏈��
        if (!IsPlaySE_)
        {
            Audio::Play(hStartSound_);
        }
		IsPlaySE_ = true;
      
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
