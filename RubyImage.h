#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// Ruby����肵�����킩��UI
/// </summary>
class RubyImage : public GameObject
{
    int hRubyImage_[1];
    float posX, posY, posZ;//�����ʒu
    bool GetChangImageFlag;//Ruby�������Ă邩�����ĂȂ������肷��flag
public:
    RubyImage(GameObject* parent);

    ~RubyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

