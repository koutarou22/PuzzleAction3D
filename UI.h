#pragma once
#include "Engine/GameObject.h"

/// <summary>
///UI���܂Ƃ߂�N���X
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