#pragma once
#include "Engine/GameObject.h"
class Bullet :public GameObject
{
    int hModel_;

    int Random;
    XMFLOAT3 GoalPos[3][3][3];
    XMFLOAT3 posX[2], posY[2], posZ[2];

public:

    Bullet(GameObject* parent);

    ~Bullet();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;


   /* float GetRayHeight() const { return 1.0f; }
    /// <summary>
    /// ���C�̊J�n�ʒu
    /// </summary>
    /// <returns></returns>
    XMFLOAT3 GetRayStart() const
    {
        XMFLOAT3 rayStart = transform_.position_;
        rayStart.y += GetRayHeight();
        return rayStart;
    }*/

};

