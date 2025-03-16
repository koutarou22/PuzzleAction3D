#pragma once
#include "Engine/GameObject.h"
class MoveEnemy :public GameObject
{
    int hModel_;
    const int MAX_RANGE = 9;
public:
   
    MoveEnemy(GameObject* parent);
    
    ~MoveEnemy();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

    void CanMoveRenge();

    float MoveEnemyDirection;

    XMVECTOR Move = XMVectorZero();
};

