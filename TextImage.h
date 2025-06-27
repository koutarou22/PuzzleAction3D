#pragma once
#include "Engine/GameObject.h"

/// <summary>
///ClearğŒUI
/// </summary>
class TextImage : public GameObject
{
    int hTextImage_[2];
    float posX, posY, posZ;//‰ŠúˆÊ’u
    bool GetChangeImageFlag;//Œ®‚ğ‚Á‚Ä‚é‚©‚Á‚Ä‚È‚¢‚©”»’è‚·‚éflag
public:
    TextImage(GameObject* parent);

    ~TextImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

