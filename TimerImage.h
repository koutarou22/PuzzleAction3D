#pragma once
#include "Engine/GameObject.h"

/// <summary>
///Clear����UI (���݂͎g�p����Ă��܂���)
/// </summary>
class TimerImage : public GameObject
{
    int hTimerImage_;
    float posX, posY, posZ;//�����ʒu
    float posX1;
    float posX2;
    bool GetChangeImageFlag;//���������Ă邩�����ĂȂ������肷��flag

    int timerValue; // �^�C�}�[�̌��ݒl
    int frameCounter; // �t���[���P�ʂ̊Ǘ�

public:
    TimerImage(GameObject* parent);

    ~TimerImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

