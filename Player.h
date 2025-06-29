#pragma once
#include "Engine/GameObject.h"

//�ړ��𔻒肷�鏈��
enum MOVE_METHOD
{
	CAN_MOVE_WALK,   //�ړ���
	CAN_MOVE_JUMP,   //�W�����v��
	CANT_MOVE,       //�ړ��s��
	CANT_JUMP,       //�W�����v�s��
	CAN_MOVE_FALL,   //�����\
	MAX_MOVE_METHOD, //��O
};

//�v���C���[�̏��
enum PLAYER_STATE
{
	MOVE, JUMP, FALL, DEAD, CLEAR
};

class Player : public GameObject
{


	//���͂Ɋւ��鏈��
	//���͉\�Ȕ͈�
	const float STICK_DEADZONE = 0.5f;
	//�ړ�����
	const float MOVE_GRID = 1.0f;

	/// <summary>
	/// �ړ��\�����肷�鏈��
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <returns></returns>
	MOVE_METHOD CanMoveTo(const XMFLOAT3& pos);

	/// <summary>
	/// �X�e�[�W�̏�ɏ��鏈��
	/// </summary>
	/// <param name="pos"></param>
	void StandingStage(const XMFLOAT3& pos);


	/// <summary>
	///���͏���
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetInputDirection();

	XMFLOAT3 velocity = { 0, 0, 0 };

	bool IsJumpInterpolation; // �W�����v��Ԓ��t���O
	bool IsWalkInterpolation; // ������Ԓ��t���O
	bool deferFall = false;   //�����ɐ؂�ւ��̂�x�炷�p

	XMFLOAT3 dir_; // ���͕����p

	void PlayerMoveMent();
	void PlayerFallDown();

	void JumpParabola();
	void Jump(const XMFLOAT3& inputDir);

	PLAYER_STATE playerstate = PLAYER_STATE::MOVE;

	//-----------------���f���o�^�p--------------------------
	
	int hPlayerModel_;//�A�j���[�V�����̃��f�����i�[����ϐ�

	//�O�ҋ@���[�V����
	//�P�ړ����[�V����
	//�Q�ݒu���[�V����
	//�R�U�����[�V����
	//�S�W�����v���[�V����
	//�T���ꃂ�[�V����
	//�U�������[�V����
	//�V���������[�V����
	//�W�������[�V����

	int hPlayerAnimeModel_[10];//�A�j���[�V�����̃��f���z��
	//-------------------------------------------------------

	// ���ʃ^�C�}�[
	int animationTimer_;      
	int currentAnimIndex_;    // ���݂̃A�j���C���f�b�N�X
	bool isAnimationLocked_;  // �t���[����0�ɂȂ�܂ő��̓�����󂯕t���Ȃ��ꍇ�p

	//����ȃA�j���[�V�����^�C�}�[
	int animationDeadTimer_;//�G�ڐG���̃^�C�}�[
	int animationVictoryTimer_;//�N���A���̃A�j���[�V�����^�C�}�[
	int animationLandingTimer_;//���n���̃A�j���[�V����


	//�v���C���[�̎c�@
	int Player_Residue;

	//�J�����������Ă���̂��m�F
	bool isMoveCamera_;

	//�G�ƐڐG���Ă�̂�����p
	bool isHitEnemy_;

	//���ɐڐG���ɃN���A�����𓾂�p
	bool ClearFlag_;

	//Goal�ɐڐG�����Ƃ�
	bool openGoal_;

	bool GetRubyflag_;//Ruby����肵��������p
	void UpdateMove();
	void UpdateDead();
	void UpdateClear();

	void DeadAnimation();   //�G�ڐG����Animation
	void ClearAnimation();  //�S�[������Animation

	MOVE_METHOD PlayerBlockInstans();
public:
	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* parent) override;

	//Set�EGet�֐��@�e�����|���̂ŕۗ�

	void SetClearFlag(bool ClearFlag) { ClearFlag_ = ClearFlag; }
	//void SetBlockAnimeEnd(bool EndAnimation) { isBlockCanOnly = EndAnimation; }
	void SetHitEnmeyFlag(bool isHIt) { isHitEnemy_ == isHIt; }

	void SetHitGoalFlag(bool isGoal) { ClearFlag_ == isGoal; }
	bool GetClearFlag() { return ClearFlag_; }
	//�J�������������Ă���Ƃ��v���C���[�͓������Ƃ��o���Ȃ�����
	void SetMoveCamera(bool moveCamera) { isMoveCamera_ = moveCamera; }
	bool GetMoveCamera() { return isMoveCamera_; }
	int GetPlayerModel() { return hPlayerModel_; }



  //�O�ҋ@���[�V����
  //�P�ړ����[�V����
  //�Q�ݒu���[�V����
  //�R�U�����[�V����
  //�S�W�����v���[�V����
  //�T���ꃂ�[�V����
  //�U�������[�V����
  void SetPlayerAnimation(int AnimeType);


};