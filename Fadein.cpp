#include "Fadein.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"

namespace
{
	static float fadeAlpha = 0.00f; // 黒画像の透明度
	const float fadeSpeed = 0.02f; // フェードの速度

	int loadWaitTimer;             // ロード待機タイマー
}

Fadein::Fadein(GameObject* parent) : GameObject(parent, "Fadein"),
hBlackScreen_(-1),
isFadingOut(false),
isLoading(false)
{
	// フェードアウト中でない場合はフェードイン
	if (fadeAlpha == 0.0f)
	{
		isFadingOut = false;
	}


	fadeAlpha = 0.0f;
	isFadingOut = false;
	isLoading = false;
	loadWaitTimer = 60;

	hBlackScreen_ = Image::Load("Scene//Black.png");
	assert(hBlackScreen_ >= 0);

}

void Fadein::Initialize()
{
	hStartSE_[CHANGE_SCENE_SE_START] = Audio::Load(SceneSEPath + "SE03.wav");
	assert(hStartSE_[CHANGE_SCENE_SE_START] >= 0);
}

void Fadein::Update()
{
}

void Fadein::Draw()
{
	Image::SetAlpha(hBlackScreen_, fadeAlpha * 255);
	Image::Draw(hBlackScreen_);
}

void Fadein::Release()
{
}

void Fadein::FadeIn(SCENE_ID scene)
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

	if (isFadingOut)
	{
		fadeAlpha += fadeSpeed;
		if (fadeAlpha >= 1.0f)
		{
			fadeAlpha = 1.0f;

			// 毎回タイトルシーンに戻ったときにステージ番号をリセット
			// ※現状はプレイシーン・ロードシーンで呼ぶ事がないため進行に問題はないが、そこで呼ぶ場合は注意が必要
			pSceneManager->ResetStageNumber();

			// シーンの変更
			pSceneManager->ChangeScene(scene);

		}
	}
}

void Fadein::PlayStartSound()
{
	if (!IsPlaySE_)
	{
		Audio::Play(hStartSE_[CHANGE_SCENE_SE_START]);
		IsPlaySE_ = true;
	}
}
