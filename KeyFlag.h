#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// 扉を開く条件用
/// </summary>
class KeyFlag : public GameObject
{
    int hKey_;
    float posX,posY,posZ;//初期位置

    //簡単な縦揺れのアニメーション
    float totalTime_;//合計の時間を格納する用
    float deltaTime;//1フレームの時間(秒で換算予定)
    

    float amplitude_;  // 振動の振れ幅
    float frequency_;  // 揺れの速度
    float offsetY_;    //最終的な計算を格納する用

    bool isGetKey_;//鍵を入手したか

public:
   
    KeyFlag(GameObject* parent);
    
    ~KeyFlag();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    void OnCollision(GameObject* parent) override;

    void KeyVibrationAnimation();

    void SetBasePosition(float x, float y, float z);
};

