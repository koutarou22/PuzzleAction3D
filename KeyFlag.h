#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// �����J�������p
/// </summary>
class KeyFlag : public GameObject
{
    int hModel_;
    float posX,posY,posZ;//�����ʒu

    int Random;
    XMFLOAT3 KeyPos[2][2][2];
public:
   
    KeyFlag(GameObject* parent);
    
    ~KeyFlag();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;
};

