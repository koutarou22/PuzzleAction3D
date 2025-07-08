#pragma once
#include "Engine/GameObject.h"
#include "Shadow.h"

#include <string>

using std::string;

// �ړ��𔻒肷�鏈��
enum MOVE_METHOD
{
    CAN_MOVE_WALK,   // �ړ���
    CAN_MOVE_JUMP,   // �W�����v��
    CANT_MOVE,       // �ړ��s��
    CANT_JUMP,       // �W�����v�s��
    CAN_MOVE_FALL,   // �����\

	CAN_MOVE_JUMP_MY_BLOCK, // �����̃u���b�N��ŃW�����v�\
	CANT_MOVE_MY_BLOCK,     // �����̃u���b�N��ňړ��s��

    MAX_MOVE_METHOD, // ��O
};

// �v���C���[�̏��
enum PLAYER_STATE
{
    MOVE,
    JUMP,
    FALL,
    DEAD,
    CLEAR
};

enum PLAYER_ANIMATION_TYPE
{
    ANIM_IDLE = 0,     // �O�F�ҋ@���[�V����
    ANIM_MOVE,         // �P�F�ړ����[�V����
    ANIM_SETTING,      // �Q�F�ݒu���[�V����
    ANIM_ATTACK,       // �R�F�U�����[�V����
    ANIM_JUMP,         // �S�F�W�����v���[�V����
    ANIM_DEAD,         // �T�F���ꃂ�[�V����
    ANIM_VICTORY,      // �U�F�������[�V����
    ANIM_FALL,         // �V�F���������[�V����
    ANIM_LANDING,      // �W�F���n���[�V����
    ANIM_MAX
};


//���֌W
enum PLAYER_SOUND_SE
{
	PLAYER_SE_WALK = 0, //����SE
    PLAYER_SE_SETTING,  //�ݒuSE
    PLAYER_SE_JUMP,     //�W�����vSE
    PLAYER_SE_LANDING,  // ���nSE
    PLAYER_SE_GETITEM,  //�A�C�e���擾SE

    PLAYER_SE_DEAD,     // ����SE
    PLAYER_SE_CLEAR,    // �N���ASE
    PLAYER_SE_MAX,
};


class Player : public GameObject
{
    // -------------------- �萔��` --------------------
    static constexpr float STICK_DEADZONE = 0.5f;
    static constexpr float MOVE_GRID = 1.0f;
    static constexpr float DEFAULT_GROUND_HEIGHT = 1.0f;
    static constexpr int PLAYER_ANIMATION_COUNT = 10;
    // --------------------------------------------------

    //�N���A�������̊m�F
    int ClearCount = 0;

    // ���͏����֌W
    MOVE_METHOD CanMoveTo(const XMFLOAT3& pos);
    XMFLOAT3 GetInputDirection();

    XMFLOAT3 velocity = { 0, 0, 0 };
    XMFLOAT3 dir_;     // ���͕����p

    bool IsJumpInterpolation = false; // �W�����v��Ԓ��t���O
    bool IsWalkInterpolation = false; // ������Ԓ��t���O
    bool deferFall = false;           // �������ꎞ�}��

    void PlayerMoveMent();
    void PlayerFallDown();

    void JumpParabola();
    void Jump(const XMFLOAT3& inputDir);

    PLAYER_STATE playerstate = PLAYER_STATE::MOVE;

    // ---------------- �A�j���[�V���� ----------------
    int hPlayerModel_;                                // ���ݎg�p���̃v���C���[���f��
    int hPlayerAnimeModel_[PLAYER_ANIMATION_COUNT];   // �A�j���[�V�����ʃ��f���z��

    // �A�j���[�V��������
    int animationTimer_;
    int currentAnimIndex_;
    bool isAnimationLocked_;       // ���b�N���͕ʓ���s��

    int animationDeadTimer_;       // �G�ڐG��
    int animationVictoryTimer_;    // �N���A��
    int animationLandingTimer_;    // ���n��
    // ------------------------------------------------------

    // �n�ʏ��E���
    int GROUND = static_cast<int>(DEFAULT_GROUND_HEIGHT); // �ڒn����
    int Player_Residue;  // �c�@��



    // ��ԊǗ� 
    bool isMoveCamera_= false;  
    bool isHitEnemy_  = false;
    bool ClearFlag_   = false;
    bool openGoal_    = false;
    bool onMyBlock_   = false;
    bool GetRubyflag_ = false;

    //�T�E���h
    int SoundPlayerSE_[PLAYER_SE_MAX];

    // �����u���b�N
    void UpdateMove();
    void UpdateDead();
    void UpdateClear();

    void DeadAnimation();
    void ClearAnimation();

    MOVE_METHOD PlayerBlockInstans();
    void PlayerGridCorrection();

public:
    Player(GameObject* parent);
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    void OnCollision(GameObject* parent) override;

    //SET.GET
    void SetClearFlag(bool ClearFlag) { ClearFlag_ = ClearFlag; }
    void SetHitEnmeyFlag(bool isHIt) { isHitEnemy_ = isHIt; }
    void SetHitGoalFlag(bool isGoal) { ClearFlag_ = isGoal; }
    bool GetClearFlag() { return ClearFlag_; }

    int GetGroundHeight() { return GROUND; }

	// �v���C���[�̃N���A�������L
	int GetClearCount() { return ClearCount; }

    // �X�e�[�W�ɗ�����
    void StandingStage(const XMFLOAT3& pos);

    // �A�j���[�V�����ݒ�i0:�ҋ@?8:�����j
    void SetPlayerAnimation(int AnimeType);
};