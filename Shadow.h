#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// オブジェクトの影としての役割を持つオブジェクト
/// </summary>
class Shadow : public GameObject
{
    int hModel_;
    float posX, posY, posZ;

public:

    Shadow(GameObject* parent);

    ~Shadow();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }


    int GetShadowModel() { return hModel_; }

};

