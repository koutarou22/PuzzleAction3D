#include "Fadein.h"
#include "Engine/Image.h"
#include "Engine/Audio.h"

namespace
{
	static float fadeAlpha = 0.00f; // ���摜�̓����x
	const float fadeSpeed = 0.02f; // �t�F�[�h�̑��x

	int loadWaitTimer;             // ���[�h�ҋ@�^�C�}�[
}

Fadein::Fadein(GameObject* parent) : GameObject(parent, "Fadein"),
hBlackScreen_(-1),
isFadingOut(false),
isLoading(false)
{
	// �t�F�[�h�A�E�g���łȂ��ꍇ�̓t�F�[�h�C��
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

			// ����^�C�g���V�[���ɖ߂����Ƃ��ɃX�e�[�W�ԍ������Z�b�g
			// ������̓v���C�V�[���E���[�h�V�[���ŌĂԎ����Ȃ����ߐi�s�ɖ��͂Ȃ����A�����ŌĂԏꍇ�͒��ӂ��K�v
			pSceneManager->ResetStageNumber();

			// �V�[���̕ύX
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
