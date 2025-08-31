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
	ClearSoundPath_ = "Sound//BGM//";
	ClearSoundName_[CLEAR_SOUND_BGM] = "CLEAR_BGM.wav";
	Instantiate<Fadein>(this);

	hImage_ = Image::Load("Scene/Clear.png");
	assert(hImage_ > 0);

	hClearBGM_[CLEAR_SOUND_BGM] = Audio::Load(ClearSoundPath_ + ClearSoundName_[CLEAR_SOUND_BGM], true, 1);
	assert(hClearBGM_[CLEAR_SOUND_BGM] >= 0);
}

void ClearScene::Update()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Fadein* pFadein = (Fadein*)FindObject("Fadein");

	Audio::Play(hClearBGM_[CLEAR_SOUND_BGM]);

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
