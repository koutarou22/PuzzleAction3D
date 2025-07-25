#pragma once
#include "Engine/GameObject.h"

enum KEY_IMAGE_TYPE
{
    KEY_IMAGE_NO_KEY = 0, //鍵を持っていない状態
    KEY_IMAGE_KEY,      //鍵を持っている状態
	KEY_IMAGE_MAX,      //鍵の状態
};

/// <summary>
/// 鍵を入手したかわかるUI
/// </summary>
class KeyImage : public GameObject
{
    int hKeyImage_[KEY_IMAGE_MAX];
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

