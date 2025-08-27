#pragma once
#include "Engine/GameObject.h"

/// <summary>
///UI‚ð‚Ü‚Æ‚ß‚éƒNƒ‰ƒX
/// </summary>
class UI : public GameObject
{
public:
    UI(GameObject* parent);

    ~UI();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};