#pragma once
#include "Engine/GameObject.h"


/// <summary>
/// �I�u�W�F�N�g�̉e�Ƃ��Ă̖��������I�u�W�F�N�g
/// </summary>
class Shadow : public GameObject
{
    int hShadowModel_;
    float posX, posY, posZ;//�����ʒu

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

