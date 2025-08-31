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
	int hClearBGM_[CLEAR_SOUND_MAX];//ClearBGMを登録

	// クリアサウンドのパス
	string ClearSoundPath_ = "Sound//BGM//";

	// クリアサウンドの名前
	string ClearSoundName_[CLEAR_SOUND_MAX];
public:
	
	ClearScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
};