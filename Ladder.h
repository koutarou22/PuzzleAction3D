#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// クリア条件用の扉
/// </summary>
class Ladder : public GameObject
{
    int hModel_;
    //bool SetLadder_;//プレイヤークラスのClearFlagを保存する用

    float posX, posY, posZ;
public:

    Ladder(GameObject* parent);

    ~Ladder();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;
};

