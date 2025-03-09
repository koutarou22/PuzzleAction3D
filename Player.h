#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"

class FBX;

/// <summary>
/// �v���C���[�̏��N���X
/// </summary>
class Player :public GameObject
{

    enum Direction
    {
        NONE,
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD
    };

    Direction MoveDirection = NONE;

    bool isBlocked;//�v���C���[���X�e�[�W�ɂԂ�������i�߂Ȃ�����
    bool isBlockCanOnly;//�u���b�N����o���؂�܂ōēx�ݒu�ł��Ȃ�����
    FBX* pFbx;
    int hPlayerModel_;

    int hPlayerTestModel_;
    //�P�ҋ@���[�V����
    //�Q�ړ����[�V����
    //�R�U�����[�V����
    int hPlayerAnimeModel_[4];

    int hWalk_;//�����Ƃ��̃A�j���[�V����
    int hJump_;//�W�����v���̃A�j���[�V����
    int hAction_;//�u���b�N��u�������̃A�j���[�V����

    bool onGround;//�n�ʂɂ��Ă��邩�m�F�p
    bool prevSpaceKey;//�X�y�[�X�L�[�������ꂽ���m�F�p
    bool ClearFlag_;//���ɐڐG���ɃN���A�����𓾂�p
    float MoveTimer_;//������������ړ�

    bool PressKey_;

    float Jump_Power;//�W�����v��

    float posX, posY, posZ;//�����ʒu
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

    void SetPlayerAnimation(int AnimeType);
    

    XMFLOAT3 &GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    int GetModelHandle() { return hPlayerModel_; }

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

    void SetClearFlag(bool ClearFlag) { ClearFlag_ = ClearFlag; }
    void SetBlockAnimeEnd(bool EndAnimation) { isBlockCanOnly = EndAnimation; }
    bool GetClearFlag() { return ClearFlag_; }

};