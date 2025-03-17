#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// Ruby‚ğ“üè‚µ‚½‚©‚í‚©‚éUI
/// </summary>
class RubyImage : public GameObject
{
    int hRubyImage_[1];
    float posX, posY, posZ;//‰ŠúˆÊ’u
    bool GetChangImageFlag;//Ruby‚ğ‚Á‚Ä‚é‚©‚Á‚Ä‚È‚¢‚©”»’è‚·‚éflag
public:
    RubyImage(GameObject* parent);

    ~RubyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

