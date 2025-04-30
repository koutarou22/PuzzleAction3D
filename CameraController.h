#pragma once
#include "Engine/GameObject.h"


/// <summary>
/// �J�����𑀍삷��N���X
/// </summary>
class CameraController :public GameObject
{
    Transform transform_;
    XMVECTOR target_;
    XMVECTOR position_;

    float CamPosX, CamPosY, CamPosZ;

    int CamState_;

   

  
    float targetRotationY = 0.0f; // �ڕW��]�p�x

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
};

