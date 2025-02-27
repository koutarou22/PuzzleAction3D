#pragma once
#include "Engine/GameObject.h"

/// <summary>
///Clear����UI
/// </summary>
class TimerImage : public GameObject
{
    int hTimerImage_;
    float posX, posY, posZ;//�����ʒu
    bool GetChangeImageFlag;//���������Ă邩�����ĂȂ������肷��flag
public:
    TimerImage(GameObject* parent);

    ~TimerImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

