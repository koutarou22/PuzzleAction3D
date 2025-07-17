#include "ClearScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include "Fadein.h"

ClearScene::ClearScene(GameObject* parent)
	: GameObject(parent, "ClearScene")
{

}

void ClearScene::Initialize()
{

	Instantiate<Fadein>(this);

	hImage_ = Image::Load("Scene/Clear.png");
	assert(hImage_ > 0);

	hClearBGM_ = Audio::Load("Sound//BGM//CLEAR_BGM.wav", true, 1);
	assert(hClearBGM_ >= 0);
}

void ClearScene::Update()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Fadein* pFadein = (Fadein*)FindObject("Fadein");

	Audio::Play(hClearBGM_);

	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_START))
	{

		//�J�n����炷
		pFadein->PlayStartSound();
		// �t�F�[�h�A�E�g���J�n
		pFadein->SetFadeOut(true); 
	}

	if (pFadein != nullptr)
	{
		bool isFadeOut = pFadein->GetFadeOut();
		if (isFadeOut)
		{
			pFadein->FadeIn(SCENE_ID_TITLE);
			return;
		}
	}
}

void ClearScene::Draw()
{
	Image::Draw(hImage_);
}

void ClearScene::Release()
{
}
