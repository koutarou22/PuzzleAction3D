#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// スコアアップか隠し要素　Item
/// </summary>
class ScoreItem : public GameObject
{
    int hModel_;
    float posX, posY, posZ;//初期位置

public:

    ScoreItem(GameObject* parent);

    ~ScoreItem();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;
};

