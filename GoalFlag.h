#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// �N���A�����p�̔�
/// </summary>
class GoalFlag : public GameObject
{
    int hModel_;

public:
    //�R���X�g���N�^
    GoalFlag(GameObject* parent);
    //�f�X�g���N�^
    ~GoalFlag();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;
};

