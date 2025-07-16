#pragma once
#include "Engine/GameObject.h"

/// <summary>
///Clear条件を教えてくれるUI
/// </summary>
class TextImage : public GameObject
{
    int hTextImage_[3];
    float posX, posY, posZ;//初期位置
    bool GetChangeImageFlag;//鍵を持ってるか持ってないか判定するflag

	bool GetClearFlag;//クリア条件を満たしているかどうかのフラグ
public:
    TextImage(GameObject* parent);

    ~TextImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

