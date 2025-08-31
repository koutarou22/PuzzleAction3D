#include "GameOverScene.h"
#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"
#include "Fadein.h"

namespace
{
	static float fadeAlpha = 0.00f; // 黒画像の透明度
	const float fadeSpeed = 0.02f; // フェードの速度
	bool isFadingOut;         // フェードアウト中かどうか      
	bool isLoading;           // ロード中かどうか
	int loadWaitTimer;        // ロード待機タイマー
}

GameOverScene::GameOverScene(GameObject* parent)
	: GameObject(parent, "GameOverScene")
{

}

void GameOverScene::Initialize()
{

	GameOverSoundPath_ = "Sound//BGM//";
	GameOverSoundName_[GAMEOVER_SOUND_BGM] = "GAMEOVER_BGM.wav";
	Instantiate<Fadein>(this);

	hImage_ = Image::Load("Scene/GameOver.png");
	assert(hImage_ >= 0);

	hGameOverSound_[GAMEOVER_SOUND_BGM] = Audio::Load(GameOverSoundPath_ + GameOverSoundName_[GAMEOVER_SOUND_BGM], true, 1);
	assert(hGameOverSound_[GAMEOVER_SOUND_BGM] >= 0);
}

void GameOverScene::Update()
{

	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Fadein* pFadein = (Fadein*)FindObject("Fadein");

	Audio::Play(hGameOverSound_[GAMEOVER_SOUND_BGM]);


	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_START))
	{
		//開始音を鳴らす
		pFadein->PlayStartSound();
		// フェードアウトを開始
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

void GameOverScene::Draw()
{
	Image::Draw(hImage_);
}

void GameOverScene::Release()
{
}
