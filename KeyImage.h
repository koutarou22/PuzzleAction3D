#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// Œ®‚ğ“üè‚µ‚½‚©‚í‚©‚éUI
/// </summary>
class KeyImage : public GameObject
{
    int hKeyImage_[2];
    float posX, posY, posZ;//‰ŠúˆÊ’u
    bool GetChangeImageFlag;//Œ®‚ğ‚Á‚Ä‚é‚©‚Á‚Ä‚È‚¢‚©”»’è‚·‚éflag
public:
    KeyImage(GameObject* parent);
  
    ~KeyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

