#pragma once
#include "Engine/GameObject.h"

/// <summary>
///Clear条件UI
/// </summary>
class TimerImage : public GameObject
{
    int hTimerImage_;
    float posX, posY, posZ;//初期位置
    bool GetChangeImageFlag;//鍵を持ってるか持ってないか判定するflag
public:
    TimerImage(GameObject* parent);

    ~TimerImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

