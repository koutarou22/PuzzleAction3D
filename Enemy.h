#pragma once
#include "Engine/GameObject.h"
class Enemy :public GameObject
{
    int hModel_;
    const int MAX_RANGE = 9;
public:
    //�R���X�g���N�^
    Enemy(GameObject* parent);
    //�f�X�g���N�^
    ~Enemy();

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

    void CanMoveRenge();
};

