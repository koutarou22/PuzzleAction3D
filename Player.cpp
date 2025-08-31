#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Engine/Input.h"
#include "Stage.h"

#include "Engine/Audio.h"	

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "PlayerBlock.h"
#include "Engine/SceneManager.h"
#include "Remain.h"
#include "RemainItem.h"
#include "KeyFlag.h"
#include "Ghost.h"
#include "CameraController.h"
#include "Engine/VFX.h"


namespace
{
	// �ړ���ԗp�̏�ԕϐ�
	XMFLOAT3 prepos;   //�J�n
	XMFLOAT3 nextpos;  //���B�n�_
	float moveRatio = 0.0f;

	// �W�����v�^������ԃt���O
	bool isJumping = false;
	bool onGround = true;
	bool isFalling = false;

	
	XMFLOAT3 AddXMFLOAT3(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return { a.x + b.x, a.y + b.y, a.z + b.z };
	}

	XMFLOAT3 MulXMFLOAT3(float t, const XMFLOAT3& b)
	{
		return { t * b.x, t * b.y, t * b.z };
	}

	bool IsZero(const XMFLOAT3& v)
	{
		return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f;
	}

	// ����E�ړ��E�W�����v���
	const float MOVE_INTERPOLATION_SPEED = 0.07f;  // ��ԑ��x
	const float MOVE_RATIO_INITIAL = 0.001f;// ��ԊJ�n���̒l
	const float PLAYER_GRAVITY = 0.005f;// �v���C���[�̏d��
	const float JUMP_PARABOLA_COEFF = 5.0f;// �������̌W��
	float JUMP_HEIGHT = 1.0f;

	// ���T�C�Y,�ʒu�E�R���C�_�[
	const float INITIAL_PLAYER_SCALE = 0.5f;// �v���C���[�̏����T�C�Y
	const float INITIAL_PLAYER_X = 1.0f;// �v���C���[�̏���X���W
	const float INITIAL_PLAYER_Y = 1.0f;// �v���C���[�̏���Y���W
	const float INITIAL_PLAYER_Z = 0.0f;// �v���C���[�̏���Z���W
	const float COLLIDER_OFFSET_Y = 0.5f;// �v���C���[�̃R���C�_�[��Y�I�t�Z�b�g
	const float COLLIDER_SIZE = 1.0f;// �v���C���[�̃R���C�_�[�̃T�C�Y

	// �O���b�h���W�ϊ�
	const int STAGE_GRID_WIDTH = 10;// �X�e�[�W�̃O���b�h��
	const int STAGE_GRID_HEIGHT = 10;// �X�e�[�W�̃O���b�h����
	const int STAGE_HEIGHT_MAX = 10;// �X�e�[�W�̍ő卂��
	const int STAGE_OFFSET_X = 5;// �X�e�[�W��X�I�t�Z�b�g
	const int STAGE_OFFSET_Z = 4;// �X�e�[�W��Z�I�t�Z�b�g

	// �v���C���[�␳��X�i�b�v
	const float GRID_UNIT = 1.0f;// �O���b�h
	const float GRID_Y_UNIT = 0.5f;// �O���b�hY
	const float ROTATION_OFFSET_DEGREES = 180.0f;// �v���C���[�̉�]
	const float BLOCK_PLACE_VERTICAL_OFFSET = 0.5f; // �u���b�N�z�u�̐����I�t�Z�b�g
	const float GRID_VERTICAL_UNIT = 0.5f;       


	// �u���b�N�z�u
	const float BLOCK_PLACE_DISTANCE = 1.0f;// �u���b�N�z�u�̋���
	const float BLOCK_PLACE_HEIGHT_OFFSET = 0.5f;// �u���b�N�z�u�̍���

	// �u���b�NID�i����p�j
	const int STAGE_BLOCK_EMPTY = 0;// ��u���b�N
	const int STAGE_BLOCK_GHOST = 1;// �S�[�X�g
	const int STAGE_BLOCK_TURRET = 2;// �^���b�g
	const int STAGE_BLOCK_KEY = 3;// ��
	const int STAGE_BLOCK_DOOR = 4;// �h�A
	const int STAGE_BLOCK_GROUND = 5;// �n��
	const int STAGE_BLOCK_Remain = 6;// �c�@�񕜃A�C�e��
	const int STAGE_BLOCK_PLAYER_BLOCK = 7;// �v���C���[�u���b�N

	const float PLAYER_ROTATE_OFFSET_DEG = 180;//

	const float PARABOLA_COEFFICIENT = 5.0f;// �W�����v�̕������W��
}


namespace PLAYER_ANIME_FRAME
{
	const int IDOL_ANIMATION_FRAME = 120;  //�ҋ@�t���[��
	const int MOVE_ANIMATION_FRAME = 30;   //�ړ��t���[��
	const int SETTING_ANIMATION_FRAME = 80;//Block�ݒu���t���[��
	const int ATTACK_ANIMATION_FRAME = 129;//Block�U�����t���[��

	const int JUMP_ANIMATION_FRAME =30;   //�W�����v�t���[��
	const int DAMAGE_ANIMATION_FRAME = 104;//����t���[��
	const int VICTORY_ANIMATION_FRAME = 110;//�S�[���t���[��
	const int FALL_ANIMATION_FRAME = 32;   //�������t���[��
	const int LANDING_ANIMATION_FRAME = 35;//���n�t���[��
}


Player::Player(GameObject* parent)
	: GameObject(parent, "Player"), hPlayerModel_(-1), playerstate(PLAYER_STATE::MOVE), isHitEnemy_(false)

{
	prepos = { 0, 1, 0 };
	nextpos = { 0, 0, 0 };
	moveRatio = 0.0f;
	isJumping = false;
	isFalling = false;

	onMyBlock_ = false;
	onGround = true;

	//�����͑ҋ@�A�j���[�V����
	SetPlayerAnimation(ANIM_IDLE);


	//�T�E���h�̏�����
	SoundPlayerSE_[PLAYER_SE_WALK]    = -1;
	SoundPlayerSE_[PLAYER_SE_JUMP]    = -1;
	SoundPlayerSE_[PLAYER_SE_LANDING] = -1;
	SoundPlayerSE_[PLAYER_SE_SETTING] = -1;
	SoundPlayerSE_[PLAYER_SE_GETITEM] = -1;
	SoundPlayerSE_[PLAYER_SE_DEAD]    = -1;
}

void Player::Initialize()
{
	transform_.position_ = { 0, INITIAL_PLAYER_Y, 0 };
	transform_.scale_ = { INITIAL_PLAYER_SCALE, INITIAL_PLAYER_SCALE, INITIAL_PLAYER_SCALE };


	isJumping = false;
	IsJumpInterpolation = false;

	BoxCollider* collision = new BoxCollider({ 0, COLLIDER_OFFSET_Y, 0 }, { COLLIDER_SIZE, COLLIDER_SIZE, COLLIDER_SIZE });
	AddCollider(collision);

	// �A�j���[�V�����̓o�^
	//�ҋ@
	hPlayerAnimeModel_[ANIM_IDLE] = Model::Load("Animation//Breathing Idle.fbx");
	assert(hPlayerAnimeModel_[ANIM_IDLE] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_IDLE], 0, PLAYER_ANIME_FRAME::IDOL_ANIMATION_FRAME, 1.0);

	//�ړ�
	hPlayerAnimeModel_[ANIM_MOVE] = Model::Load("Animation//Standard Walk.fbx");
	assert(hPlayerAnimeModel_[ANIM_MOVE] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_MOVE], 0, PLAYER_ANIME_FRAME::MOVE_ANIMATION_FRAME, 1.0);

	//�ݒu
	hPlayerAnimeModel_[ANIM_SETTING] = Model::Load("Animation//Magic Heal.fbx");
	assert(hPlayerAnimeModel_[ANIM_SETTING] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_SETTING], 0, PLAYER_ANIME_FRAME::SETTING_ANIMATION_FRAME, 1.0);

	//�U���@��������
	hPlayerAnimeModel_[ANIM_ATTACK] = Model::Load("Animation//Standing 2H Magic Attack.fbx");
	assert(hPlayerAnimeModel_[ANIM_ATTACK] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_ATTACK], 0, PLAYER_ANIME_FRAME::ATTACK_ANIMATION_FRAME, 1.0);

	//�W�����v
	hPlayerAnimeModel_[ANIM_JUMP] = Model::Load("Animation//Jump.fbx");
	assert(hPlayerAnimeModel_[ANIM_JUMP] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_JUMP], 0, PLAYER_ANIME_FRAME::JUMP_ANIMATION_FRAME, 1.0);

	//���S
	hPlayerAnimeModel_[ANIM_DEAD] = Model::Load("Animation//Down.fbx");
	assert(hPlayerAnimeModel_[ANIM_DEAD] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_DEAD], 0, PLAYER_ANIME_FRAME::DAMAGE_ANIMATION_FRAME, 1.0);

	//����
	hPlayerAnimeModel_[ANIM_VICTORY] = Model::Load("Animation//Victory Idle.fbx");
	assert(hPlayerAnimeModel_[ANIM_VICTORY] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_VICTORY], 0, PLAYER_ANIME_FRAME::VICTORY_ANIMATION_FRAME, 1.0);

	//������
	hPlayerAnimeModel_[ANIM_FALL] = Model::Load("Animation//Fall A Loop.fbx");
	assert(hPlayerAnimeModel_[ANIM_FALL] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_FALL], 0, PLAYER_ANIME_FRAME::FALL_ANIMATION_FRAME, 1.0);

	//���n
	hPlayerAnimeModel_[ANIM_LANDING] = Model::Load("Animation//Landing2.fbx");
	assert(hPlayerAnimeModel_[ANIM_LANDING] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_LANDING], 10, PLAYER_ANIME_FRAME::LANDING_ANIMATION_FRAME, 1.0);


	//�T�E���h�̓o�^

	string PlayerPath = "Sound//SE//PlayerSE//";
	string ClearPath = "Sound//SE//ClearConditionsSE//";

	SoundPlayerSE_[PLAYER_SE_WALK] = Audio::Load(PlayerPath + "Walk//WalkingOnGround.wav");
	SoundPlayerSE_[PLAYER_SE_SETTING] = Audio::Load(PlayerPath + "Setting//BlockSet.wav");
	SoundPlayerSE_[PLAYER_SE_LANDING] = Audio::Load(PlayerPath + "Jump//Landing.wav");
	SoundPlayerSE_[PLAYER_SE_JUMP] = Audio::Load(PlayerPath + "Jump//Jump.wav");
	SoundPlayerSE_[PLAYER_SE_DEAD] = Audio::Load(PlayerPath + "Death//LightKick.wav");
	SoundPlayerSE_[PLAYER_SE_GETITEM] = Audio::Load(PlayerPath + "GetItem//Shining.wav");
	SoundPlayerSE_[PLAYER_SE_CLEAR] = Audio::Load(ClearPath + "CheersAndApplause.wav");
	SoundPlayerSE_[PLAYER_SE_DONT_CLEAR] = Audio::Load(ClearPath + "Locked DoorSE.wav");

	//�T�E���h�̓o�^�m�F
	assert(SoundPlayerSE_[PLAYER_SE_WALK] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_JUMP] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_LANDING] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_SETTING] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_GETITEM] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_DEAD] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_CLEAR] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_DONT_CLEAR] >= 0);


	//�����A�j���[�V�������Z�b�g
	hPlayerModel_ = hPlayerAnimeModel_[ANIM_IDLE];

	//�c�@���������Ă���
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager != nullptr)
	{
		Player_Remain = pSceneManager->GetPlayerRemain();
	}
}

/// <summary>
/// �ړ��\���ǂ����𔻒肷��
/// </summary>
/// <param name="pos"></param>
/// <returns></returns>
MOVE_METHOD Player::CanMoveTo(const XMFLOAT3& pos)
{
	
	int gx = static_cast<int>(roundf(pos.x + STAGE_OFFSET_X));
	int gy = static_cast<int>(roundf(STAGE_OFFSET_Z - pos.z));
	int gz = static_cast<int>(roundf(pos.y));

	//��ʊO�ɂ͂����Ȃ��悤�ɂ��鏈��
	if (gx < 0 || gx >= STAGE_GRID_WIDTH ||
		gy < 0 || gy >= STAGE_GRID_HEIGHT ||
		gz < 0 || gz >= STAGE_HEIGHT_MAX)
	{
		Debug::Log("�ړ��͈͊O & �����Ȃ�", true);
		return CANT_MOVE;
	}

	auto* stage = static_cast<Stage*>(FindObject("Stage"));
	auto& grid = stage->GetStageGrid();

	int current = grid[gz][STAGE_GRID_HEIGHT - 1 - gy][gx];

	if (current == STAGE_BLOCK_EMPTY)
	{
		int under = grid[gz - 1][STAGE_GRID_HEIGHT - 1 - gy][gx];
		if (under == STAGE_BLOCK_EMPTY)
		{
			Debug::Log("�������ɐ؂�ւ����", true);
			return CAN_MOVE_FALL;
		}
		else
		{
			Debug::Log("�ړ����ɐ؂�ւ����", true);
			return CAN_MOVE_WALK;
		}
	}
	else if (
		//�ȉ��̃I�u�W�F�N�g�͂��蔲���\
		current == STAGE_BLOCK_GHOST   || //�S�[�X�g
		current == STAGE_BLOCK_KEY     || //��
		current == STAGE_BLOCK_DOOR    || //�h�A
		current == STAGE_BLOCK_Remain)   //�c�@�񕜃A�C�e��
	{
		Debug::Log("�Ώۂ̃I�u�W�F�N�g�͈ړ��\", true);

		return CAN_MOVE_WALK;
	}

	//�ڂ̑O�̃u���b�N�������̏o�����u���b�N�Ȃ�
	if (current == STAGE_BLOCK_PLAYER_BLOCK)
	{
		Debug::Log("�����̏o�����u���b�N�ɃW�����v�\", true);
		return CAN_MOVE_JUMP_MY_BLOCK;
	}


	//�����ڂ̑O�̃u���b�N������A���̏オ�󂢂Ă�Ȃ�W�����v�\ 
	if (gz + 1 < STAGE_HEIGHT_MAX)
	{
		int above = grid[gz + 1][STAGE_GRID_HEIGHT - 1 - gy][gx];
		if (above == STAGE_BLOCK_EMPTY   ||
			above == STAGE_BLOCK_GHOST   ||
			above == STAGE_BLOCK_KEY     ||
			above == STAGE_BLOCK_DOOR    || 
			above == STAGE_BLOCK_Remain)
		{
			Debug::Log("�W�����v�\", true);
			return CAN_MOVE_JUMP;
		}
	}
	else if (gz + 2 < STAGE_HEIGHT_MAX)//���̏���󂢂Ă��Ȃ�������W�����v�͕s�\
	{
		int twoabove = grid[gz + 2][STAGE_GRID_HEIGHT - 1 - gy][gx];
		if (twoabove != STAGE_BLOCK_EMPTY && current != STAGE_BLOCK_EMPTY)
		{
			Debug::Log("�j�}�X�ȏ�𒴂��Ă��܂�");
			return CANT_JUMP;
		}
	}

	return CANT_MOVE;
}


void Player::StandingStage(const XMFLOAT3& pos)
{
	int gx = static_cast<int>(roundf(pos.x + STAGE_OFFSET_X));
	int gy = static_cast<int>(roundf(STAGE_OFFSET_Z - pos.z));

	auto* stage = static_cast<Stage*>(FindObject("Stage"));
	auto& grid = stage->GetStageGrid();

	int yStart = static_cast<int>(floorf(pos.y));

	//�ǂ��܂ŉ��ɒT�������w��
	int yEnd = yStart - STAGE_GRID_HEIGHT;

	//�O���b�h�͈̔͊O(0)�ɏo�Ȃ��悤�ɂ���
	if (yEnd < 0)
	{
		yEnd = 0;
	}


	for (int y = yStart; y >= yEnd; --y)
	{
		int current = grid[y][STAGE_GRID_HEIGHT - 1 - gy][gx];

		//�ȉ��̃u���b�N�͒n�ʂƂ��Ĉ���(�W�����v�\)
		if (current == STAGE_BLOCK_GROUND ||    //�n��
			current == STAGE_BLOCK_TURRET ||    //�G�^���b�g
			current == STAGE_BLOCK_PLAYER_BLOCK)//�����̃u���b�N
		{
			GROUND = y + 1;
			return;
		}
	}
}


XMFLOAT3 Player::GetInputDirection()
{
	PlayerBlock* pPlayerBlock = (PlayerBlock*)FindObject("PlayerBlock");

	//�ړ�������������
	dir_ = { 0, 0, 0 };

	//���X�e�B�b�N�̏����擾
	XMFLOAT3 LeftStick = Input::GetPadStickL(0);

	//�ړ�(��)
	if      (Input::IsKey(DIK_W) || LeftStick.y >= STICK_DEADZONE)  dir_.z += MOVE_GRID;
	//�ړ�(��O)
	else if (Input::IsKey(DIK_S) || LeftStick.y <= -STICK_DEADZONE) dir_.z -= MOVE_GRID;
	//�ړ�(��)
	else if (Input::IsKey(DIK_A) || LeftStick.x <= -STICK_DEADZONE) dir_.x -= MOVE_GRID;
	//�ړ�(�E)
	else if (Input::IsKey(DIK_D) || LeftStick.x >= STICK_DEADZONE)  dir_.x += MOVE_GRID;

	//�ړ����E�W�����v�� �����Ď����̃u���b�N��ł͂Ȃ��ꍇ
	if (!IsWalkInterpolation && !IsJumpInterpolation && !onMyBlock_)
	{
		//�u���b�N��A���Őݒu�ł��Ȃ��悤�ɂ��鏈��
		if (pPlayerBlock == nullptr || !pPlayerBlock->GetIsAnimation())
		{
			//�u���b�N�ݒu
			if (Input::IsKeyDown(DIK_L) || Input::IsPadButton(XINPUT_GAMEPAD_B))
			{
				PlayerBlockInstans();
			}
		}
	}

	return dir_;
}

void Player::UpdateDead()
{
	//���S�A�j���[�V�������X�V
	DeadAnimation();
}

void Player::UpdateClear()
{
	//�N���A�A�j���[�V�������X�V
	ClearAnimation();
}

void Player::DeadAnimation()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Remain* pRemain = (Remain*)FindObject("Remain");

	//isHitEnemy_��true�Ȃ�A�j���[�V�������Đ�
	if (isHitEnemy_ == true)
	{
		SetPlayerAnimation(5);
	}

	animationDeadTimer_--;


	Audio::Play(SoundPlayerSE_[PLAYER_SE_DEAD]);

	if (animationDeadTimer_ <= 0)
	{
		isHitEnemy_ = false;

		// �c�@�����炷
		int currentRemain = pSceneManager->GetPlayerRemain();
		if (currentRemain > 0)
		{
			pSceneManager->SetPlayerRemain(currentRemain - 1); // �c�@��1���炷
		}

		//�c�@��1�ȏ�Ȃ� LoadScene �ɖ߂�
		if (pSceneManager->GetPlayerRemain() > 0)
		{
			pSceneManager->ChangeScene(SCENE_ID_LOAD);
		}
		else
		{
			//Scene���؂�ς��O�Ɋm���Ɏc�@�����Z�b�g
			pSceneManager->ResetRemain(); 
			pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
		}
	}
}

void Player::ClearAnimation()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

	int MaxStage = pSceneManager->GetMaxStageNumber();



	if (openGoal_ && ClearFlag_)
	{
		SetPlayerAnimation(6);
		animationVictoryTimer_--;

		Audio::Play(SoundPlayerSE_[PLAYER_SE_CLEAR]);


		if (animationVictoryTimer_ <= 0)
		{
			// �X�e�[�W�����ɐi�߂�(�l��+1����)
			pSceneManager->NextStageCountPlus();

			// ���̃X�e�[�W�ԍ����擾
			int currentStage = pSceneManager->GetStageNumber();

			if (currentStage > MaxStage)
			{
				// �X�e�[�W���ő�l�𒴂�����N���A�V�[���� 
				pSceneManager->ChangeScene(SCENE_ID_CLEAR);

			}
			else
			{
				// �Ⴄ�Ȃ�ʏ�̃��[�h�V�[����
				pSceneManager->ChangeScene(SCENE_ID_LOAD);
			}
		}
	}

}


MOVE_METHOD Player::PlayerBlockInstans()
{
	// �����u���b�N������΍폜
	PlayerBlock* existingBlock = (PlayerBlock*)FindObject("PlayerBlock");
	if (existingBlock != nullptr)
	{
		//�Â��u���b�N(�O)�̈ʒu���擾
		XMFLOAT3& oldPos = existingBlock->GetPosition();
		int oldGx = static_cast<int>(oldPos.x + STAGE_OFFSET_X);
		int oldGy = static_cast<int>(STAGE_OFFSET_Z - oldPos.z);
		int oldGz = static_cast<int>(oldPos.y);

		auto* stage = static_cast<Stage*>(FindObject("Stage"));
		if (stage)
		{
			auto& grid = stage->GetStageGrid();
			//�O�̃u���b�N�͍폜(0)����
			grid[oldGz][STAGE_GRID_HEIGHT - 1 - oldGy][oldGx] = STAGE_BLOCK_EMPTY;//0�ɖ߂�
			StandingStage(transform_.position_);
		}

		existingBlock->KillMe();
	}

	// �v���C���[�̈ʒu�ƕ����ݒ�

	// �v���C���[�̈ʒu���擾
	XMVECTOR PlayerPos = XMLoadFloat3(&(transform_.position_));

	// �v���C���[�̑O�������w��
	XMVECTOR FrontDirection = XMVectorSet(0.0f, BLOCK_PLACE_VERTICAL_OFFSET, -1.0f, 0.0f);

	// �v���C���[�̉�]���l�����đO��������]
	XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

	// �O��������]������
	FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

	// �u���b�N��u���ʒu���v�Z
	XMVECTOR blockPos = PlayerPos + FrontDirection * BLOCK_PLACE_DISTANCE;

	//�O���b�h�ʒu
	XMVECTOR snappedBlockPos = XMVectorSet(
		round(XMVectorGetX(blockPos) / GRID_UNIT) * GRID_UNIT,
		round(XMVectorGetY(blockPos) / GRID_VERTICAL_UNIT) * GRID_VERTICAL_UNIT,
		round(XMVectorGetZ(blockPos) / GRID_UNIT) * GRID_UNIT,
		0.0f
	);

	// �O���b�h�ϊ�
	int gx = static_cast<int>(XMVectorGetX(snappedBlockPos) + STAGE_OFFSET_X);
	int gy = static_cast<int>(STAGE_OFFSET_Z - XMVectorGetZ(snappedBlockPos));
	int gz = static_cast<int>(XMVectorGetY(snappedBlockPos));

	auto* stage = static_cast<Stage*>(FindObject("Stage"));

	auto& grid = stage->GetStageGrid();

	// �ݒu�����`�F�b�N�@��ʊO�ɒu���Ȃ��悤�ɂ���--------------------
	if (gx < 0 || gx >= STAGE_GRID_WIDTH  ||
		gy < 0 || gy >= STAGE_GRID_HEIGHT ||
		gz < 0 || gz >= STAGE_HEIGHT_MAX)
	{
		return CANT_MOVE;
	}

	PlayerBlock* pPlayerBlock = (PlayerBlock*)FindObject("PlayerBlock");


	//�ڂ̑O�Ƀu���b�N�����邩�`�F�b�N+�A�j���[�V�������I����Ă��邩�m�F
	int cellValue = grid[gz][STAGE_GRID_HEIGHT - 1 - gy][gx];
	if (cellValue != STAGE_BLOCK_EMPTY && cellValue != STAGE_BLOCK_PLAYER_BLOCK &&
		cellValue != STAGE_BLOCK_GHOST && cellValue != STAGE_BLOCK_TURRET)
	{
		return CANT_MOVE;
	}


	//----------------------------------------------------------------

	// �u���b�N�ݒu
	PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, snappedBlockPos);
	block->SetPosition(pos);


	//�ݒuAnimation
	SetPlayerAnimation(ANIM_SETTING);

	//�����Ńu���b�N7��o�^����
	grid[gz][STAGE_GRID_HEIGHT - 1 - gy][gx] = STAGE_BLOCK_PLAYER_BLOCK;

	//�u���b�N�̐ݒu��
	Audio::Play(SoundPlayerSE_[PLAYER_SE_SETTING]);

	return CANT_MOVE;
}

// �v���C���[�̈ʒu���O���b�h�ɍ��킹�ĕ␳����
void Player::PlayerGridCorrection()
{
	float gridSize = 1.0f;
	float x = round((transform_.position_.x) / gridSize) * gridSize;
	float y = round((transform_.position_.y) / gridSize) * gridSize;
	float z = round((transform_.position_.z) / gridSize) * gridSize;
	transform_.position_ = { x,y,z };
}


void Player::PlayerFallDown()
{
	//��������
	if (isFalling)
	{
		//�d�͂�K�p
		velocity.y -= PLAYER_GRAVITY;
		transform_.position_.y += velocity.y;

		//�������̃A�j���[�V����
		SetPlayerAnimation(7);

		if (transform_.position_.y <= GROUND)
		{
			SetPlayerAnimation(8);
			Audio::Play(PLAYER_SE_LANDING);
			animationLandingTimer_--;
			if (animationLandingTimer_ <= 0)
			{
				animationLandingTimer_ = PLAYER_ANIME_FRAME::LANDING_ANIMATION_FRAME;
				SetPlayerAnimation(0);	
			}

			//���n������n�ʂɍ��킹��
			transform_.position_.y = GROUND;
			velocity.y = 0;
			isFalling = false;
			onGround = true;
		}
	}
}

void Player::JumpParabola()
{
	if (isJumping)
	{
		moveRatio += MOVE_INTERPOLATION_SPEED;
		XMFLOAT3 horizontal = AddXMFLOAT3(prepos, MulXMFLOAT3(moveRatio, nextpos));

		//���������
		float offsetY =
			PARABOLA_COEFFICIENT * // �������̌W��
			JUMP_HEIGHT *          // �W�����v�̍ő卂
			moveRatio *            // �J�n �� �I���i�s�x�i0.0~1.0�j
			(1.0f - moveRatio);    // �i�s�x�ɑ΂��đΏ̂Ȍ���


		// �ʒu���X�V
		StandingStage(transform_.position_);
		transform_.position_ = { horizontal.x, prepos.y + offsetY, horizontal.z };


		if (moveRatio >= 1.0f)
		{
			//�W�����v���I�������
			transform_.position_ = AddXMFLOAT3(prepos, nextpos);
			StandingStage(transform_.position_);
			transform_.position_.y = GROUND;

			isJumping = false;
			onGround = true;
			IsJumpInterpolation = false;

			Audio::Play(PLAYER_SE_LANDING);

			SetPlayerAnimation(0);
		}
	}
}

void Player::Jump(const XMFLOAT3& inputDir)
{
	if (onGround)
	{

		Audio::Play(SoundPlayerSE_[PLAYER_SE_JUMP]);

		prepos = transform_.position_;
		nextpos = inputDir;
		nextpos.y = 1.0f;
		moveRatio = 0.0f;
		isJumping = true;// �W�����v���t���O�𗧂Ă�
		onGround = false;
		IsJumpInterpolation = true;
		SetPlayerAnimation(4);
	}
}

void Player::Update()
{

	switch (playerstate)
	{
	case PLAYER_STATE::MOVE:
		UpdateMove();
		break;

	case PLAYER_STATE::DEAD:
		UpdateDead();
		break;

	case PLAYER_STATE::CLEAR:
		UpdateClear();
		break;

	default:
		playerstate = PLAYER_STATE::MOVE;
	}


	//���LAnimation�̌�������
	if (animationTimer_ > 0)
	{
		animationTimer_--;

		if (animationTimer_ == 0) {
			isAnimationLocked_ = false;

			SetPlayerAnimation(0); 

			animationLandingTimer_ = 0;
		}
	}

}

void Player::UpdateMove()
{
	//����������Ȃ���Βʏ�̈ړ��������s��
	if (!isFalling)
	{
		PlayerMoveMent();
		JumpParabola();
	}
	else if (!IsWalkInterpolation)
	{
	
		PlayerFallDown();	//�����̏���
		Audio::Stop(SoundPlayerSE_[PLAYER_SE_WALK]);
	}
}

void Player::PlayerMoveMent()
{
	static bool moving = false;
	static float moveRatio = 0.0f;

	CameraController* pCamera = (CameraController*)FindObject("CameraController");
	Stage* pStage = (Stage*)FindObject("Stage");

	XMFLOAT3 inputDir = GetInputDirection();

	//�J��������]���A�܂��̓W�����v��Ԓ��͈ړ��o���Ȃ�
	if (pCamera->IsRotating() || IsJumpInterpolation)
	{
		inputDir = { 0, 0, 0 };
		return;
	}


	if (!moving)
	{
		if (!IsZero(inputDir))
		{
			// ���͕������J�����̉�]�ɍ��킹�ĕϊ�
			XMMATRIX camRotMatrix = pCamera->GetRotationMatrix();

			// ���͕������J�����̉�]�ɍ��킹�ĕϊ�
			XMVECTOR inputVec = XMLoadFloat3(&inputDir);

			// �J�����̉�]�s����g���ē��̓x�N�g������]
			XMVECTOR rotatedVec = XMVector3TransformCoord(inputVec, camRotMatrix);

			// ��]��̃x�N�g����XMFLOAT3�ɕϊ�
			XMStoreFloat3(&nextpos, rotatedVec);

			XMFLOAT3 target = AddXMFLOAT3(transform_.position_, nextpos);

			//�ړ��\���ǂ����𔻒�
			MOVE_METHOD method = CanMoveTo(target);

			switch (method)
			{
			case CAN_MOVE_WALK:
				SetPlayerAnimation(ANIM_MOVE);

				Audio::Play(SoundPlayerSE_[PLAYER_SE_WALK]);


				moving = true;
				IsWalkInterpolation = true;
				moveRatio = MOVE_RATIO_INITIAL;
				prepos = transform_.position_;
				onGround = true;
				break;

			case CAN_MOVE_FALL:
				prepos = transform_.position_;
				moveRatio = MOVE_RATIO_INITIAL;
				moving = true;
				IsWalkInterpolation = true;
				deferFall = true;
				break;

			case CAN_MOVE_JUMP:
				Jump(nextpos);
				break;

			case CAN_MOVE_JUMP_MY_BLOCK:
				Jump(nextpos);
				onMyBlock_ = true;
				break;

			default:
				SetPlayerAnimation(ANIM_IDLE);
				break;
			}
		}
	}
	else
	{
		moveRatio += MOVE_INTERPOLATION_SPEED;

		if (moveRatio >= 1.0f)
		{
			transform_.position_ = AddXMFLOAT3(prepos, nextpos);
			moveRatio = 0.0f;
			moving = false;
			IsWalkInterpolation = false;
			nextpos = { 0, 0, 0 };

			//��Ԃ��I�������
			if (!IsWalkInterpolation)
			{
				//�Y���h�~�ŕ␳����
				PlayerGridCorrection();
				Audio::Stop(SoundPlayerSE_[PLAYER_SE_WALK]);
			}

			
			if (deferFall)
			{
				StandingStage(transform_.position_);
				isFalling = true;
				velocity = { 0, 0, 0 };
				deferFall = false;
			}
		}
		else
		{
			transform_.position_ = AddXMFLOAT3(prepos, MulXMFLOAT3(moveRatio, nextpos));
			SetPlayerAnimation(ANIM_MOVE);

			onMyBlock_ = false;
		}
	}

	//�v���C���[�̌������X�V
	if (!IsZero(nextpos))
	{
		float angle = atan2(nextpos.x, nextpos.z);
		transform_.rotate_.y = XMConvertToDegrees(angle) + PLAYER_ROTATE_OFFSET_DEG;
	}
}

void Player::Draw()
{
	Model::SetTransform(hPlayerModel_, transform_);
	Model::Draw(hPlayerModel_);
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* parent)
{
	PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");
	KeyFlag* pKey = (KeyFlag*)FindObject("KeyFlag");
	Ghost* pGhost = (Ghost*)FindObject("Ghost");
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Remain* pRemain = (Remain*)FindObject("Remain");
	RemainItem* pRemainItem = (RemainItem*)FindObject("RemainItem");


	if (parent->GetObjectName() == "KeyFlag")
	{
		ClearFlag_ = true;
		Audio::Play(SoundPlayerSE_[PLAYER_SE_GETITEM]);

	}

	if (parent->GetObjectName() == "Ghost" || parent->GetObjectName() == "Bullet")
	{
		isHitEnemy_ = true;

		if (isHitEnemy_)
		{
			playerstate = PLAYER_STATE::DEAD;
		}

	}

	if (parent->GetObjectName() == "GoalDoor")
	{
		openGoal_ = true;

		if (openGoal_ && ClearFlag_)
		{
			playerstate = PLAYER_STATE::CLEAR;
		}
		else
		{
			Audio::Play(SoundPlayerSE_[PLAYER_SE_DONT_CLEAR]);
		}
	}


	if (parent->GetObjectName() == "RemainItem" && pRemain != nullptr)
	{
		GetRubyflag_ = true;

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

		if (pSceneManager != nullptr)
		{
			Audio::Play(SoundPlayerSE_[PLAYER_SE_GETITEM]);

			int currentRemain = pSceneManager->GetPlayerRemain();

			// �c�@���ő�l�ɒB���Ă��Ȃ��ꍇ�A�c�@�𑝂₷
			pSceneManager->SetPlayerRemain(currentRemain + 1);
		}

		pRemainItem->KillMe();
	}
}

void Player::SetPlayerAnimation(int AnimeType)
{
	if (currentAnimIndex_ != AnimeType)
	{
		hPlayerModel_ = hPlayerAnimeModel_[AnimeType];
		currentAnimIndex_ = AnimeType;
		isAnimationLocked_ = true;

		switch (AnimeType)
		{
		case ANIM_IDLE:
			animationTimer_ = PLAYER_ANIME_FRAME::IDOL_ANIMATION_FRAME;
			break;

		case ANIM_MOVE:
			animationTimer_ = PLAYER_ANIME_FRAME::MOVE_ANIMATION_FRAME;
			break;

		case ANIM_SETTING:
			animationTimer_ = PLAYER_ANIME_FRAME::SETTING_ANIMATION_FRAME;
			break;

		case ANIM_ATTACK:
			animationTimer_ = PLAYER_ANIME_FRAME::ATTACK_ANIMATION_FRAME;
			break;

		case ANIM_JUMP:
			animationTimer_ = PLAYER_ANIME_FRAME::JUMP_ANIMATION_FRAME;
			break;

		case ANIM_DEAD:
			animationDeadTimer_ = PLAYER_ANIME_FRAME::DAMAGE_ANIMATION_FRAME;
			break;

		case ANIM_VICTORY:
			animationVictoryTimer_ = PLAYER_ANIME_FRAME::VICTORY_ANIMATION_FRAME;
			break;

		case ANIM_FALL:
			animationTimer_ = PLAYER_ANIME_FRAME::FALL_ANIMATION_FRAME;
			break;

		case ANIM_LANDING:
			animationLandingTimer_ = PLAYER_ANIME_FRAME::LANDING_ANIMATION_FRAME;
			break;

		default:
			break;
		}
	}
}


