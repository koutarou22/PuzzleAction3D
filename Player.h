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
    int hWalk_;//�����Ƃ��̃A�j���[�V����
    int hJump_;//�W�����v���̃A�j���[�V����
    int hAction_;//�u���b�N��u�������̃A�j���[�V����

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
    /// Player�̓���
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
    /// �v���C���[�̖ڂ̑O�Ƀu���b�N���o�������鏈��
    /// </summary>
    void PlayerBlockInstans();

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    int GetModelHandle() { return hModel_; }

    float GetRayHeight() const { return 1.0f; }

    /// <summary>
    /// ���C�̊J�n�ʒu
    /// </summary>
    /// <returns></returns>
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
    /// <summary>
    /// �v���C���[���������ǂ���������i�߂Ȃ��������
    /// </summary>
    /// <param name="newPosition"></param>
    /// <returns></returns>
    bool IsBlocked(XMVECTOR Position);
};

