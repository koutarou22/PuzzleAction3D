#pragma once
#include "Engine/GameObject.h"
#include <string>

using std::string;

enum GAMEOVER_SCENE_SOUND
{
	GAMEOVER_SOUND_BGM = 0,
	GAMEOVER_SOUND_MAX,
};
/// <summary>
/// GameOverScene
/// </summary>
class GameOverScene : public GameObject
{
	int hImage_;
	int hGameOverSound_[GAMEOVER_SOUND_MAX];//GameOverBGMを登録

	//サウンドのパス
	string GameOverSoundPath_;

	//サウンドの名前
	string GameOverSoundName_[GAMEOVER_SOUND_MAX];
public:
	
	GameOverScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
};