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

    int nextFace = 0; //���̖ʂ��i�[����ϐ�
    int currentFace = 0; //�J�����̌���
    float RotateProgress = 1.0f; //��ԏ��
    const float RotateSpeed = 0.05f; //��ԑ��x
  
    float targetRotationY = 0.0f; // �Ώۂ̉�]�p�x

    bool isNotPlayerMove_;//camera�𓮂����Ă���Ԃ͓����Ȃ�������悤�ϐ�

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


    //camera�̏����܂Ƃ߂��֐�
    void CameraControl();

    void SetNotMovePlayer(bool NotMove) { isNotPlayerMove_ = NotMove; }
    bool GetNotMovePlayer() { return isNotPlayerMove_; }
};

