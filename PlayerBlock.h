#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"


#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

class FBX;

/// <summary>
/// プレイヤーが使用するブロック
/// </summary>
class PlayerBlock :public GameObject
{
    FBX* pFbx;
    int hPlayerBlockModel_;

    void AnimateBlock();//簡単な生成Animation

   
public:

    PlayerBlock(GameObject* parent);

    ~PlayerBlock();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 &GetPosition() { return transform_.position_; }
    XMFLOAT3 &GetScale()    { return transform_.position_; }

    void OnCollision(GameObject* parent) override;
};

