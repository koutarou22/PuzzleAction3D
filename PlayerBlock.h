#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"

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

    float Jump_Power;//�W�����v��
public:

    //�R���X�g���N�^
    PlayerBlock(GameObject* parent);
    //�f�X�g���N�^
    ~PlayerBlock();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;



    
    XMFLOAT3 &GetTransform() { return transform_.position_; }

    void OnCollision(GameObject* parent) override;

};

