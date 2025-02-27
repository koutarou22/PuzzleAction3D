#pragma once
#include "Engine/GameObject.h"

/// <summary>
///ClearğŒUI
/// </summary>
class TimerImage : public GameObject
{
    int hTimerImage_;
    float posX, posY, posZ;//‰ŠúˆÊ’u
    bool GetChangeImageFlag;//Œ®‚ğ‚Á‚Ä‚é‚©‚Á‚Ä‚È‚¢‚©”»’è‚·‚éflag
public:
    TimerImage(GameObject* parent);

    ~TimerImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

