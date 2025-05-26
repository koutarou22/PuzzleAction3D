#pragma once
#include "Engine/GameObject.h"

/// <summary>
///Clear条件UI (現在は使用されていません)
/// </summary>
class TimerImage : public GameObject
{
    int hTimerImage_;
    float posX, posY, posZ;//初期位置
    float posX1;
    float posX2;
    bool GetChangeImageFlag;//鍵を持ってるか持ってないか判定するflag

    int timerValue; // タイマーの現在値
    int frameCounter; // フレーム単位の管理

public:
    TimerImage(GameObject* parent);

    ~TimerImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

