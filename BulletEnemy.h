#pragma once
#include "Engine/GameObject.h"
class BulletEnemy :public GameObject
{
    int hModel_;

    int Timer_;
    bool isAttack_;
    const int MAX_RANGE = 9;
public:

    BulletEnemy(GameObject* parent);

    ~BulletEnemy();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;
};

