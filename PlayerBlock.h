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
    int hModel_;
    bool onGround;//地面についているか確認用
    bool prevSpaceKey;//スペースキーが押されたか確認用
    //bool isTimerZero;
    //bool TimeFullVerdict;

    float Jump_Power;         //ジャンプ力

    bool  isHitMoveRight_;    //右
    bool  isHitMoveLeft_;     //左
    bool  isHitMoveForward_;  //手前
    bool  isHitMoveBackward_; //奥

    bool MoveHitCheck_;//動かしたかどうか

 /*   float TimeCount_;*/

    void AnimateBlock();

   
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

    void BlockRange();

    void SetMoveRight(bool Right) { isHitMoveRight_ = Right; }
    void SetMoveLeft(bool Left) { isHitMoveLeft_ = Left; }
    void SetMoveForward(bool Forwaed) { isHitMoveForward_ = Forwaed; }
    void SetMoveBackwaed(bool Backwaed) { isHitMoveBackward_ = Backwaed; }


    bool GetMoveHit(){ return MoveHitCheck_; }

};

