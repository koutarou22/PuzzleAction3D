#pragma once
#include "Engine/GameObject.h"


/// <summary>
/// カメラを操作するクラス
/// </summary>
class CameraController :public GameObject
{
    Transform transform_;
    XMVECTOR target_;
    XMVECTOR position_;

    float CamPosX, CamPosY, CamPosZ;

    int CamState_;

    int nextFace = 0; //次の面を格納する変数
    int currentFace = 0; //カメラの向き
    float RotateProgress = 1.0f; //補間状態
    const float RotateSpeed = 0.05f; //補間速度
  
    float targetRotationY = 0.0f; // 対象の回転角度

    bool isNotPlayerMove_;//cameraを動かしている間は動けなくさせるよう変数

public:
  
    CameraController(GameObject* parent);

    ~CameraController();

    void Initialize();

    void Update();

    void Draw();

    void Release();

    void DefaultComera();

    XMMATRIX GetRotationMatrix() const;
  
    int GetCurrentFace() const;

    XMFLOAT3 Getroatate() { return transform_.rotate_; }

    XMVECTOR GetForwardVector() const;


    //cameraの情報をまとめた関数
    void CameraControl();

    void SetNotMovePlayer(bool NotMove) { isNotPlayerMove_ = NotMove; }
    bool GetNotMovePlayer() { return isNotPlayerMove_; }
};

