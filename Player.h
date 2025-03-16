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
    //�O�ҋ@���[�V����
    //�P�ړ����[�V����
    //�Q�ݒu���[�V����
    //�R�U�����[�V����
    //�S�W�����v���[�V����
    //�T���ꃂ�[�V����
    //�U�������[�V����
    int hPlayerAnimeModel_[7];
    int moveAnimationTimer_ = 0; // �A�j���[�V���������p�^�C�}�[
    int victoryAnimationTimer_ = 0;//�N���A
   


    int currentFrame_ = 0; // ���݂̃A�j���[�V�����t���[��
    int maxFrame_ = 0;     // �A�j���[�V�����̍ő�t���[��
    bool isAnimationFinished = false; // �A�j���[�V�������I���������ǂ���
    int currentAnimation_ = 0;        // ���݂̃A�j���[�V�����^�C�v

    bool onGround;//�n�ʂɂ��Ă��邩�m�F�p
    bool onMyBlock;//�����ŏo�����u���b�N�̏�ɏ���Ă��邩
    bool prevSpaceKey;//�X�y�[�X�L�[�������ꂽ���m�F�p
    bool ClearFlag_;//���ɐڐG���ɃN���A�����𓾂�p
    bool openGoal_;//Goal�ɐڐG�����Ƃ�

    float MoveFlagTimer_;
    float MoveTimer_;//������������ړ�

    bool PressKey_;

    float Jump_Power;//�W�����v��

    float posX, posY, posZ;//�����ʒu


    bool isHitEnemyFlag;//�G�ƐڐG���Ă��܂���Flag
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
    void SetHitEnmeyFlag(bool isHIt) { isHitEnemyFlag == isHIt; }

    void SetHitGoalFlag(bool isGoal) { ClearFlag_ == isGoal; }
    bool GetClearFlag() { return ClearFlag_; }


    XMVECTOR CameraMoveVector();

};