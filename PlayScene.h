#pragma once
#include "Engine//GameObject.h"
#include<string>

using std::string;

enum PLAY_SCENE_SOUND
{
    PLAY_SOUND_PLAY = 0,
    PLAY_SOUND_MAX,
};

/// <summary>
/// PlayScene
/// </summary>
class PlayScene : public GameObject
{
    string PlaySoundPath_;//音楽のパス
    string PlaySoundName_[PLAY_SOUND_MAX];//音楽の名前

	int hBackImage_;//背景画像

    int hPlaySound_[PLAY_SOUND_MAX]; //開始を押されたらBGMを鳴らす
public:
    PlayScene(GameObject* parent);
    ~PlayScene();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

