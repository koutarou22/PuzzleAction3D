#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// ������肵�����킩��UI
/// </summary>
class KeyImage : public GameObject
{
    int hKeyImage_[2];
    float posX, posY, posZ;//�����ʒu
    bool GetChangeImageFlag;//���������Ă邩�����ĂȂ������肷��flag
public:
    KeyImage(GameObject* parent);
  
    ~KeyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

