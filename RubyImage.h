#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// Rubyを入手したかわかるUI
/// </summary>
class RubyImage : public GameObject
{
    int hRubyImage_[1];
    float posX, posY, posZ;//初期位置
    bool GetChangImageFlag;//Rubyを持ってるか持ってないか判定するflag
public:
    RubyImage(GameObject* parent);

    ~RubyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

