#pragma once
#include "Engine/GameObject.h"
#include <string>

using std::string;

enum TITLE_SCENE_SOUND
{
	TITLE_SOUND_BGM = 0,//タイトルBGM
    TITLE_SOUND_MAX,
};

/// <summary>
/// TitleScene
/// </summary>
class TitleScene : public GameObject
{
	int hBackTitleImage_;//背景　
	int hBlackScreen_;   //簡単な暗転
	int hTitleSound_;    //音楽

	//音楽ファイルパス
	string SoundFilePath_ = "Sound//BGM//";
	string TitleSoundFileName_[TITLE_SOUND_MAX];//タイトルシーンの音楽関連のファイル名


public:
	
	TitleScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;
	
	void Release() override;
};