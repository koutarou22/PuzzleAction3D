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
    int hModel_;
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

    /// <summary>
    /// Jump�̏���
    /// </summary>
    void Jump();

    /// <summary>
    /// �n�ʂƂ̔�������(���C�L���X�g)
    /// </summary>
    void GroundCheck();

    /// <summary>
    /// �v���C���[���u���b�N���o�������鏈��
    /// </summary>
    void PlayerBlockInstans();

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    int GetModelHandle() { return hModel_; }

    float GetRayHeight() const { return 1.0f; }

    XMFLOAT3 GetRayStart() const 
    {
        XMFLOAT3 rayStart = transform_.position_; 
        rayStart.y += GetRayHeight();
        return rayStart;
    }

    void SetonGround(bool ground)
    {
     
        onGround = ground;
    }
    void OnCollision(GameObject* parent) override;


    void StageHeight();
    bool IsBlocked(XMVECTOR newPosition);
};

