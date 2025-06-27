#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"


#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

class FBX;

/// <summary>
/// �v���C���[���g�p����u���b�N
/// </summary>
class PlayerBlock :public GameObject
{
    FBX* pFbx;
    int hModel_;
    bool onGround;//�n�ʂɂ��Ă��邩�m�F�p
    bool prevSpaceKey;//�X�y�[�X�L�[�������ꂽ���m�F�p
    //bool isTimerZero;
    //bool TimeFullVerdict;

    float Jump_Power;         //�W�����v��

    bool  isHitMoveRight_;    //�E
    bool  isHitMoveLeft_;     //��
    bool  isHitMoveForward_;  //��O
    bool  isHitMoveBackward_; //��

    bool MoveHitCheck_;//�����������ǂ���

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

