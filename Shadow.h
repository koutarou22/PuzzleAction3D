#pragma once
#include "Engine/GameObject.h"


/// <summary>
/// オブジェクトの影としての役割を持つオブジェクト
/// </summary>
class Shadow : public GameObject
{
    int hShadowModel_;
    float posX, posY, posZ;//初期位置

public:

    Shadow(GameObject* parent);

    ~Shadow();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void SetPosition(XMFLOAT3 pos) { transform_.position_ = pos; }


    int GetShadowModel() { return hShadowModel_; }

    XMFLOAT3 SetScale() { return transform_.scale_; }

};

