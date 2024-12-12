#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"

class FBX;

enum CameraType
{

};

/// <summary>
/// �v���C���[�̏��N���X
/// </summary>
class Player :public GameObject
{
    FBX* pFbx;
    int hModel;
    bool onGround;//�n�ʂɂ��Ă��邩�m�F�p
    bool prevSpaceKey;//�X�y�[�X�L�[�������ꂽ���m�F�p

    float Jump_Power;//�W�����v��
public:

    //�R���X�g���N�^
    Player(GameObject* parent);
    //�f�X�g���N�^
    ~Player();

    //������
    void Initialize() override;

    //�X�V
    void Update() override;

    //�`��
    void Draw() override;

    //�J��
    void Release() override;

    /// <summary>
    /// Player�̑���
    /// </summary>
    void PlayerControl();
    /// <summary>
    /// Player���s����͈͂𐧌�����
    /// </summary>
    void PlayerRange();

    void Jump();

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

};

