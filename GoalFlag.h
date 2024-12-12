#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// クリア条件用の扉
/// </summary>
class GoalFlag : public GameObject
{
    int hModel_;

public:
    //コンストラクタ
    GoalFlag(GameObject* parent);
    //デストラクタ
    ~GoalFlag();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;
};

