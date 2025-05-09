#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// 鍵を入手したかわかるUI
/// </summary>
class KeyImage : public GameObject
{
    int hKeyImage_[2];
    float posX, posY, posZ;//初期位置
    bool GetChangeImageFlag;//鍵を持ってるか持ってないか判定するflag
public:
    KeyImage(GameObject* parent);
  
    ~KeyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

