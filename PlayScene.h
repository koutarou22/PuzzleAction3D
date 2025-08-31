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
    string PlaySoundPath_;//���y�̃p�X
    string PlaySoundName_[PLAY_SOUND_MAX];//���y�̖��O

	int hBackImage_;//�w�i�摜

    int hPlaySound_[PLAY_SOUND_MAX]; //�J�n�������ꂽ��BGM��炷
public:
    PlayScene(GameObject* parent);
    ~PlayScene();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

