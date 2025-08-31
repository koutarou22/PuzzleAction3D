#pragma once
#include "Engine/GameObject.h"
#include <string>

using std::string;

enum TITLE_SCENE_SOUND
{
	TITLE_SOUND_BGM = 0,//�^�C�g��BGM
    TITLE_SOUND_MAX,
};

/// <summary>
/// TitleScene
/// </summary>
class TitleScene : public GameObject
{
	int hBackTitleImage_;//�w�i�@
	int hBlackScreen_;   //�ȒP�ȈÓ]
	int hTitleSound_;    //���y

	//���y�t�@�C���p�X
	string SoundFilePath_ = "Sound//BGM//";
	string TitleSoundFileName_[TITLE_SOUND_MAX];//�^�C�g���V�[���̉��y�֘A�̃t�@�C����

public:
	
	TitleScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;
	
	void Release() override;
};