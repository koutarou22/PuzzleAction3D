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
public:
    //�R���X�g���N�^
    CameraController(GameObject* parent);

    //�f�X�g���N�^
    ~CameraController();

    //������
    void Initialize();

    //�X�V
    void Update();

    //�`��
    void Draw();

    //�J��
    void Release();
};

