#pragma once
#include <string>
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"
#include<list>

using std::list;

using std::string;
class FBX;

/// <summary>
/// �v���C���[�̏��N���X
/// </summary>
class Player :public GameObject
{
    
    enum Direction//�v���C���[�̕������Ǘ�
    {
        NONE,
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD
    };

    Direction MoveDirection = NONE;

    bool isBlockCanOnly;//�u���b�N����o���؂�܂ōēx�ݒu�ł��Ȃ�����

    int hPlayerModel_;//�A�j���[�V�����̃��f�����i�[����ϐ�

    //�O�ҋ@���[�V����
    //�P�ړ����[�V����
    //�Q�ݒu���[�V����
    //�R�U�����[�V����
    //�S�W�����v���[�V����
    //�T���ꃂ�[�V����
    //�U�������[�V����
    int hPlayerAnimeModel_[7];//�A�j���[�V�����̃��f���z��
    int MoveAnimationTimer_;//�A�j���[�V�����̃^�C�}�[
    int VictoryAnimationTimer_;//�N���A���̃A�j���[�V�����^�C�}�[
  
    bool onGround;//�n�ʂɂ��Ă��邩�m�F�p
    bool onMyBlock;//�����ŏo�����u���b�N�̏�ɏ���Ă��邩
    bool prevSpaceKey;//�X�y�[�X�L�[�������ꂽ���m�F�p
    bool ClearFlag_;//���ɐڐG���ɃN���A�����𓾂�p
    bool openGoal_;//Goal�ɐڐG�����Ƃ�

    bool GetRubyflag;//Ruby����肵��������p

    float MoveFlagTimer_;
    float MoveTimer_;//������������ړ�

    bool PressKey_;

    float Jump_Power;//�W�����v��

    float posX, posY, posZ;//�����ʒu


    bool isHitEnemy_;//�G�ƐڐG����

    //�J�����������Ă邩�̏���
    bool isMoveCamera_;

    /// <summary>
    /// �ړ��������Ă鎞��Ԃ�����!....�p�̕ϐ�����
    /// </summary>
    int isMove_;//�ꎞ�I�Ȉړ��t���[�����i�[���锻��ϐ�
    bool isMove_interpolation;//���͂�����Ό����������鏈��
    bool isGoMove;//0�ɂȂ����瓮���Ă�����I�̕ϐ�


   

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

    void PlayerMove(XMVECTOR BaseMove , XMVECTOR NextPos, float x, float y , float z );

    

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
    float GetRayWide() const { return 1.0f; }

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

    //���O�͌�Œ�������
    XMFLOAT3 GetRayStartX() const
    {
        XMFLOAT3 rayStart = transform_.position_;
        rayStart.x += GetRayWide();
        return rayStart;
    }

    void SetonGround(bool ground)
    {
        onGround = ground;
    }
    void OnCollision(GameObject* parent) override;


    void StageHeight();
    /// <summary>
    /// �v���C���[���������ǂ���������i�߂Ȃ��������(�����蔻��)
    /// </summary>
    /// <param name="newPosition"></param>
    /// <returns></returns>
    bool IsBlocked(XMVECTOR Position);


    void SetClearFlag(bool ClearFlag) { ClearFlag_ = ClearFlag; }
    void SetBlockAnimeEnd(bool EndAnimation) { isBlockCanOnly = EndAnimation; }
    void SetHitEnmeyFlag(bool isHIt) { isHitEnemy_ == isHIt; }

    void SetHitGoalFlag(bool isGoal) { ClearFlag_ == isGoal; }
    bool GetClearFlag() { return ClearFlag_; }

    bool GetScoreFlag() { return GetRubyflag; }

    void SetJumpPower(float Jump) { Jump_Power = Jump; }



    //�J�������������Ă���Ƃ��v���C���[�͓������Ƃ��o���Ȃ�����
    void SetMoveCamera(bool MoveCamera) { isMoveCamera_ = MoveCamera; }
    bool GetMoveCamera() { return isMoveCamera_; }


    /// <summary>
    /// �v���C���[���n�ʂɒ����E�܂��͍s���I����Ƀ}�X�ڂ̒����ɕ␳���鏈��
    /// </summary>
    void PlayerGridCorrection();

    int GetPlayerModel() { return hPlayerModel_; }

    /// <summary>
    /// Animation�̊Ǘ�
    /// </summary>
    void Animation();
};