#pragma once
#include "Engine/GameObject.h"

/// <summary>
///Clear����UI
/// </summary>
class TextImage : public GameObject
{
    int hTextImage_[2];
    float posX, posY, posZ;//�����ʒu
    bool GetChangeImageFlag;//���������Ă邩�����ĂȂ������肷��flag
public:
    TextImage(GameObject* parent);

    ~TextImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

