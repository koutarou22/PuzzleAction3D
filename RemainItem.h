#pragma once
#include "Engine/GameObject.h"


/// <summary>
/// 残基をアップさせるための処理
/// </summary>
class RemainItem: public GameObject
{
    int hRemain_;

    //初期位置
    float posX, posY, posZ;

    //簡単な縦揺れのアニメーション用変数

    float totalTime_;  //合計の時間を格納する用
    float deltaTime;   //1フレームの時間(秒で換算予定)

    float amplitude_;  // 振動の振れ幅
    float frequency_;  // 揺れの速度
    float offsetY_;    //最終的な計算を格納する用

public:

    RemainItem(GameObject* parent);

    ~RemainItem();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

    void RemainVibrationAnimation();

    void SetBasePosition(float x, float y, float z);
};

