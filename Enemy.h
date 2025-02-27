#pragma once
#include "Engine/GameObject.h"
class Enemy :public GameObject
{
    int hModel_;
    const int MAX_RANGE = 9;
public:
   
    Enemy(GameObject* parent);
    
    ~Enemy();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

    void CanMoveRenge();

    float MoveEnemyDirection;
};

