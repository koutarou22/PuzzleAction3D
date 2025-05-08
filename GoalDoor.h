#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// �N���A�����p�̔�
/// </summary>
class GoalDoor: public GameObject
{
    int hModel_;
    bool GoalFlag_;//�v���C���[�N���X��ClearFlag��ۑ�����p

    int Random;

    float posX, posY, posZ;


public:

    GoalDoor(GameObject* parent);

    ~GoalDoor();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

    void SetHitGoalFlag(bool isHitPlayer) { GoalFlag_ = isHitPlayer; }


};

