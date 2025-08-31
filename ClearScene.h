#pragma once
#include "Engine/GameObject.h"
#include<string>

using std::string;

enum CLEAR_SCENE_SOUND
{
	CLEAR_SOUND_BGM,
	CLEAR_SOUND_MAX,
};
/// <summary>
/// ClearScene
/// </summary>
class ClearScene : public GameObject
{
	int hImage_;
	int hClearBGM_[CLEAR_SOUND_MAX];//ClearBGM��o�^

	// �N���A�T�E���h�̃p�X
	string ClearSoundPath_ = "Sound//BGM//";

	// �N���A�T�E���h�̖��O
	string ClearSoundName_[CLEAR_SOUND_MAX];
public:
	
	ClearScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
};