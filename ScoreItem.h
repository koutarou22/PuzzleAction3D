#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// �X�R�A�A�b�v���B���v�f�@Item
/// </summary>
class ScoreItem : public GameObject
{
    int hModel_;
    float posX, posY, posZ;//�����ʒu

public:

    ScoreItem(GameObject* parent);

    ~ScoreItem();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;
};

