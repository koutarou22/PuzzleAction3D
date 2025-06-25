#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Engine/Input.h"
#include "Stage.h"

namespace
{
	const float SRATE = 0.07f;//��ԑ��x
	const float GRAVITY = 0.005f;

	const int GRID_WIDTH = 10;
	const int GRID_HEIGHT = 10;
	const int MAX_STAGE_HEIGHT = 10;
	const int GRID_OFFSET_X = 5;
	const int GRID_OFFSET_Z = 4;


	int GROUND = 1.0f;
	float JUMP_HEIGHT = 1.0f;//�W�����v��

	XMFLOAT3 prepos;   //
	XMFLOAT3 nextpos;  //
	float moveRatio = 0.0f;

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
}


namespace PLAYER_ANIME_FRAME
{
	//59
	const int IDOL_ANIMATION_FRAME = 298;//�ҋ@�A�j���[�V����
	const int MOVE_ANIMATION_FRAME = 30;//�ړ��A�j���[�V�����̃t���[��
	const int SETTING_ANIMATION_FRAME = 80;//Block�ݒu���̃A�j���[�V�����̃t���[��
	const int ATTACK_ANIMATION_FRAME = 129;//Block�U�����̃A�j���[�V�����̃t���[��
	const int JUMP_ANIMATION_FRAME = 57;//�W�����v�A�j���[�V�����̃t���[��
	const int DAMAGE_ANIMATION_FRAME = 104;//����A�j���[�V�����̃t���[��
	const int VICTORY_ANIMATION_FRAME = 50;//�S�[���A�j���[�V�����̃t���[��
}

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "PlayerBlock.h"
#include "Engine/SceneManager.h"
#include "Residue.h"
#include "ResidueItem.h"
#include "KeyFlag.h"
#include "MoveEnemy.h"
#include "CameraController.h"


MOVE_METHOD Player::CanMoveTo(const XMFLOAT3& pos)
{
	int gx = static_cast<int>(pos.x + GRID_OFFSET_X);
	int gy = static_cast<int>(GRID_OFFSET_Z - pos.z);
	int gz = static_cast<int>(pos.y);

	if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT || gz < 0 || gz >= MAX_STAGE_HEIGHT)
	{
		Debug::Log("�����Ȃ�", true);
		return CANT_MOVE;
	}

	auto* stage = static_cast<Stage*>(FindObject("Stage"));
	auto& grid = stage->GetStageGrid();

	int current = grid[gz][GRID_HEIGHT - 1 - gy][gx];

	if (current == 0 && gz > 0)
	{
		int under = grid[gz - 1][GRID_HEIGHT - 1 - gy][gx];
		if (under == 0)
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


	if (gz + 1 < MAX_STAGE_HEIGHT)
	{
		int above = grid[gz + 1][GRID_HEIGHT - 1 - gy][gx];
		if (above == 0 && current != 0)
			return CAN_MOVE_JUMP;
	}
	else if (gz + 2 < MAX_STAGE_HEIGHT)
	{
		int twoabove = grid[gz + 2][GRID_HEIGHT - 1 - gy][gx];
		if (twoabove != 0 && current != 0)
			return CANT_JUMP;
	}

	return CANT_MOVE;
}

void Player::StandingStage(const XMFLOAT3& pos)
{

	int gx = static_cast<int>(pos.x + GRID_OFFSET_X);
	int gy = static_cast<int>(GRID_OFFSET_Z - pos.z);

	auto* stage = static_cast<Stage*>(FindObject("Stage"));
	auto& grid = stage->GetStageGrid();

	for (int y = MAX_STAGE_HEIGHT - 1; y >= 0; --y)
	{
		if (grid[y][GRID_HEIGHT - 1 - gy][gx] != 0)
		{
			GROUND = y + 1;
			break;
		}
	}
}

XMFLOAT3 Player::GetInputDirection()
{
	dir_ = { 0, 0, 0 };

	XMFLOAT3 LeftStick = Input::GetPadStickL(0);

	//�J�����������Ă��Ȃ���� �E �G�ɐڐG���Ă��Ȃ���� �E �S�[���ɐڐG���Ă��Ȃ���΁@�����Ă���
	if (!isMoveCamera_ || !isHitEnemy_ || !openGoal_)
	{
		//�ړ�(��)
		if (Input::IsKey(DIK_W) || LeftStick.y >= STICK_DEADZONE)  dir_.z += MOVE_GRID;
		//�ړ�(��O)
		else if (Input::IsKey(DIK_S) || LeftStick.y <= -STICK_DEADZONE) dir_.z -= MOVE_GRID;
		//�ړ�(��)
		else if (Input::IsKey(DIK_A) || LeftStick.x <= -STICK_DEADZONE) dir_.x -= MOVE_GRID;
		//�ړ�(�E)
		else if (Input::IsKey(DIK_D) || LeftStick.x >= STICK_DEADZONE)  dir_.x += MOVE_GRID;

		if (Input::IsKeyDown(DIK_L) || Input::IsPadButton(XINPUT_GAMEPAD_B) )
		{
			PlayerBlockInstans();
		}
	}

	return dir_;
}



void Player::UpdateDead()
{
	DeadAnimation();

}

void Player::UpdateClear()
{
	ClearAnimation();

}

void Player::DeadAnimation()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Residue* pResidue = (Residue*)FindObject("Residue");

	// �G�ɐڐG�����Ƃ��̏���
	if (isHitEnemy_)
	{
		SetPlayerAnimation(5);
		DeadAnimationTimer_--;

		if (DeadAnimationTimer_ <= 0)
		{
			isHitEnemy_ = false;

			// �c�@�����炷
			int currentResidue = pSceneManager->GetPlayerResidue();
			if (currentResidue > 0)
			{
				pSceneManager->SetPlayerResidue(currentResidue - 1); // �c�@��1���炷
			}

			//�c�@��1�ȏ�Ȃ� LoadScene �ɖ߂�
			if (pSceneManager->GetPlayerResidue() > 0)
			{
				pSceneManager->ChangeScene(SCENE_ID_LOAD);
			}
			else
			{
				pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
			}

			DeadAnimationTimer_ =PLAYER_ANIME_FRAME::DAMAGE_ANIMATION_FRAME;//�Ō�Ɍ��̃t���[���ɖ߂��Ă�����
		}
	}

}

void Player::ClearAnimation()
{
	if (openGoal_ && ClearFlag_)
	{
		SetPlayerAnimation(6);
		VictoryAnimationTimer_--;
		if (VictoryAnimationTimer_ <= 0)
		{
			VictoryAnimationTimer_ = PLAYER_ANIME_FRAME::VICTORY_ANIMATION_FRAME;

			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_CLEAR);
		}
	}
}

void Player::PlayerBlockInstans()
{
    PlayerBlock* existingBlock = (PlayerBlock*)FindObject("PlayerBlock");
    if (existingBlock != nullptr)
    {
        existingBlock->KillMe();
    }

    XMVECTOR PlayerPos = XMLoadFloat3(&(transform_.position_));

  
    XMVECTOR FrontDirection = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);

    if (Input::IsKey(DIK_UP) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
    {
        FrontDirection = XMVectorSet(0, 1.5f, -1.0f, 0.0f);
    }
    else if (Input::IsKey(DIK_DOWN) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
    {
        FrontDirection = XMVectorSet(0, -0.5f, -1.0f, 0.0f);
    }

    // �v���C���[�̉�]�p�x�����]�s����쐬
    XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

    // �O����1�}�X���z�u
    XMVECTOR blockPos = PlayerPos + FrontDirection * 1.0f;

    // �O���b�h�X�i�b�v�����iY�����̂�0.5�P�ʁj
    float gridSize = 1.0f;
    XMVECTOR snappedBlockPos = XMVectorSet(
        roundf(XMVectorGetX(blockPos) / gridSize) * gridSize,
        roundf(XMVectorGetY(blockPos) / 0.5f) * 0.5f,
        roundf(XMVectorGetZ(blockPos) / gridSize) * gridSize,
        0.0f
    );

    PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
    XMStoreFloat3(&(block->GetPosition()), snappedBlockPos);

    // �A�j���[�V���������Ȃǁi�K�v�Ȃ�j
    // SetPlayerAnimation(2);
    // MoveAnimationTimer_ = PLAYER_ANIME_FRAME::SETTING_ANIMATION_FRAME;
}


Player::Player(GameObject* parent)
	: GameObject(parent, "Player"), hSilly(-1), hPlayerModel_(-1), playerstate(PLAYER_STATE::MOVE)
{
}

void Player::Initialize()
{

	CantMoveFlag_ = false;
	CanJumpFlag_ = false;



	hSilly = Model::Load("ball.fbx");
	hPlayerModel_ = Model::Load("Player.fbx");
	transform_.scale_ = { 0.5f, 0.5f, 0.5f };
	transform_.position_ = { 0, 1.0f, 0 };

	isJumping = false;
	IsJumpInterpolation = false;

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1, 1, 1 });
	AddCollider(collision);

	string Path = "Animation//";
	string Fbx = ".fbx";

	// �ҋ@���
	hPlayerAnimeModel_[0] = Model::Load("Animation//Breathing Idle.fbx");
	assert(hPlayerAnimeModel_[0] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[0], 0, PLAYER_ANIME_FRAME::IDOL_ANIMATION_FRAME, 1.0);

	// �������[�V����
	hPlayerAnimeModel_[1] = Model::Load("Animation//Standard Walk.fbx");
	assert(hPlayerAnimeModel_[1] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[1], 0, PLAYER_ANIME_FRAME::MOVE_ANIMATION_FRAME, 1.0);

	// Block���������
	hPlayerAnimeModel_[2] = Model::Load("Animation//Magic Heal.fbx");
	assert(hPlayerAnimeModel_[2] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[2], 0, PLAYER_ANIME_FRAME::SETTING_ANIMATION_FRAME, 1.0);

	// Block�������o�����Ƃ�
	hPlayerAnimeModel_[3] = Model::Load("Animation//Standing 2H Magic Attack.fbx");
	assert(hPlayerAnimeModel_[3] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[3], 0, PLAYER_ANIME_FRAME::ATTACK_ANIMATION_FRAME, 1.0);

	// �W�����v�����Ƃ�
	hPlayerAnimeModel_[4] = Model::Load("Animation//Jumping.fbx");
	assert(hPlayerAnimeModel_[4] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[4], 10, PLAYER_ANIME_FRAME::JUMP_ANIMATION_FRAME, 1.2);

	// ���ꂽ�Ƃ�
	hPlayerAnimeModel_[5] = Model::Load("Animation//Down.fbx");
	assert(hPlayerAnimeModel_[5] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[5], 0, PLAYER_ANIME_FRAME::DAMAGE_ANIMATION_FRAME, 1.0);

	// �S�[�������Ƃ�
	hPlayerAnimeModel_[6] = Model::Load("Animation//Victory Idle.fbx");
	assert(hPlayerAnimeModel_[6] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[6], 0, PLAYER_ANIME_FRAME::VICTORY_ANIMATION_FRAME, 1.0);


	SetPlayerAnimation(0);

	//�c�@�̏���
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager != nullptr)
	{
		//�c�@�̒l��SceneManager����Ƃ��Ă���
		Player_Residue = pSceneManager->GetPlayerResidue();
	}

	//�\�߃t���[�������Ă���
	MoveAnimationTimer_ = PLAYER_ANIME_FRAME::MOVE_ANIMATION_FRAME;
	DeadAnimationTimer_ = PLAYER_ANIME_FRAME::DAMAGE_ANIMATION_FRAME;
	VictoryAnimationTimer_ = PLAYER_ANIME_FRAME::VICTORY_ANIMATION_FRAME;

}

void Player::PlayerFallDown()
{
	if (isFalling)
	{
		velocity.y -= GRAVITY;
		transform_.position_.y += velocity.y;

		if (transform_.position_.y <= GROUND)
		{
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
		moveRatio += SRATE;
		XMFLOAT3 horizontal = AddXMFLOAT3(prepos, MulXMFLOAT3(moveRatio, nextpos));

		//���������
		float offsetY =
			5.0f *                 // �������̌W���i�����̃X�P�[�����O�����A�ő卂���� JUMP_HEIGHT �ɂ��邽�߂̒萔�j
			JUMP_HEIGHT *          // �W�����v�̍ő卂���i���̒l���s�[�N�ɂ���j
			moveRatio *            // �J�n �� �I���ւ̐i�s�x�i0.0~1.0�j
			(1.0f - moveRatio);    // �i�s�x�ɑ΂��đΏ̂Ȍ����i���E�Ώ̂̕����������j


		StandingStage(transform_.position_);
		transform_.position_ = { horizontal.x, prepos.y + offsetY, horizontal.z };



		if (moveRatio >= 1.0f)
		{
			transform_.position_ = AddXMFLOAT3(prepos, nextpos);
			StandingStage(transform_.position_);
			transform_.position_.y = GROUND;

			isJumping = false;
			onGround = true;
			IsJumpInterpolation = false;
		}
	}
}

void Player::Jump()
{
	if (onGround)
	{
		prepos = transform_.position_;
		nextpos = GetInputDirection();
		nextpos.y = 1.0f;
		moveRatio = 0.0f;
		isJumping = true;
		onGround = false;
		IsJumpInterpolation = true;
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

}

void Player::UpdateMove()
{
	if (!isFalling)
	{
		PlayerMoveMent();
		JumpParabola();
	}
	else if (!IsWalkInterpolation)
	{
		PlayerFallDown();
	}

	// �A�j���[�V�����^�C�}�[�̐i�s
	if (MoveAnimationTimer_ > 0)
	{
		MoveAnimationTimer_--;
		if (MoveAnimationTimer_ == 0)
		{
			SetPlayerAnimation(0);
		}
	}
}
void Player::PlayerMoveMent()
{
    static bool moving = false;
    static float moveRatio = 0.0f;
    static XMFLOAT3 nextpos = { 0,0,0 };
    static XMFLOAT3 prepos = { 0,0,0 };

    // �J�������擾
    CameraController* pCamera = (CameraController*)FindObject("CameraController");

    // �X�e�[�W(���f�����)���擾�p
    Stage* pStage = (Stage*)FindObject("Stage");

    XMVECTOR move = XMVectorZero();

    // �J�����̉�]�s����擾
    XMMATRIX cameraRotation = pCamera->GetRotationMatrix();

    if (IsJumpInterpolation) return;
	if (isMoveCamera_) return;

    if (!moving)
    {
        // ���͕������擾�i�O���b�h�P�ʁj
        nextpos = GetInputDirection();

        if (!IsZero(nextpos))
        {
            // ���͕����x�N�g����XMVECTOR�ɕϊ�
            XMVECTOR inputDirVec = XMLoadFloat3(&nextpos);

            // �J�����̉�]�œ��̓x�N�g����ϊ��i���[���h���W��Ɂj
            XMVECTOR rotatedVec = XMVector3TransformNormal(inputDirVec, cameraRotation);

            // �ϊ����ʂ�XMFLOAT3�ɖ߂�
            XMStoreFloat3(&nextpos, rotatedVec);


            // �ړ���^�[�Q�b�g�ʒu���v�Z
            XMFLOAT3 target = AddXMFLOAT3(transform_.position_, nextpos);

            // �ړ��\������
            MOVE_METHOD method = CanMoveTo(target);

			if (CantMoveFlag_)
			{
				method = CANT_MOVE;
			}

			if (CanJumpFlag_)
			{
				method = CAN_MOVE_JUMP;
			}
            switch (method)
            {
            case CAN_MOVE_WALK:
                moving = true;
                IsWalkInterpolation = true;
                moveRatio = 0.001f;
                prepos = transform_.position_;
                break;

            case CAN_MOVE_FALL:
                prepos = transform_.position_;
                // ���͕����Ď擾�i�K�v�ɉ����āj
                nextpos = GetInputDirection();
                moveRatio = 0.001f;
                moving = true;
                IsWalkInterpolation = true;
                deferFall = true;

                playerstate = PLAYER_STATE::FALL;
                break;

            case CAN_MOVE_JUMP:
                Jump();
                break;

            default:
                method = MOVE_METHOD::CANT_MOVE;
                Debug::Log("CantMove....", true);
                break;
            }
        }
    }
    else
    {
        moveRatio += SRATE;

        if (moveRatio >= 1.0f)
        {
            transform_.position_ = AddXMFLOAT3(prepos, nextpos);
            moveRatio = 0.0f;
            moving = false;
            IsWalkInterpolation = false;
            nextpos = { 0, 0, 0 };

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
        }
    }

    XMVECTOR convertnextpos = XMLoadFloat3(&nextpos); // XMVECTOR

    // �v���C���[���ړ����������Ɍ�������
    if (!XMVector3Equal(convertnextpos, XMVectorZero()))
    {
        XMVECTOR pos = XMLoadFloat3(&(transform_.position_));

        // �p�x�v�Z
        float angle = atan2(XMVectorGetX(convertnextpos), XMVectorGetZ(convertnextpos));

        // �p�x��x�ɕϊ����Ĕ��f
        transform_.rotate_.y = XMConvertToDegrees(angle) + 180; // +180���Ŕ��]����

        XMStoreFloat3(&(transform_.position_), pos);
    }
}


void Player::Draw()
{
	Model::SetTransform(hPlayerModel_, transform_);
	Model::Draw(hPlayerModel_);

	Model::SetTransform(hSilly, transform_);
	Model::Draw(hSilly);

	//Debug
	{
		ImGui::Begin("Player_Debug");

		ImGui::Text("Player Position%5.2lf,%5.2lf,%5.2lf", transform_.position_.x, transform_.position_.y, transform_.position_.z);

		ImGui::Separator();
		ImGui::Text("MoveAnimationTimer%5.2lf"   ,    MoveAnimationTimer_);
		ImGui::Text("DeadAnimationTimer%5.2lf"   ,    DeadAnimationTimer_);
		ImGui::Text("VictoryAnimationTimer%5.2lf", VictoryAnimationTimer_);
		ImGui::Separator();

		ImGui::End();
	}
}

void Player::Release()
{
}

void Player::OnCollision(GameObject* parent)
{
	PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");
	KeyFlag* pKey = (KeyFlag*)FindObject("KeyFlag");
	MoveEnemy* pEnemy = (MoveEnemy*)FindObject("MoveEnemy");
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Residue* pResidue = (Residue*)FindObject("Residue");

	//�����I
	if (parent->GetObjectName() == "PlayerBlock")
	{
		XMFLOAT3 blockPos = parent->GetPosition();
		float blockY = blockPos.y;

		
		if (transform_.position_.y <= blockY + 1.0f)
		{
			CantMoveFlag_ = true;
		}

		// ��ɏ���Ă�Ƃ��͒��n����
		if (transform_.position_.y > blockY)
		{
			CanJumpFlag_ = true;
			onGround = true;
			transform_.position_.y = blockY + 0.46f;
		}
	}


	if (parent->GetObjectName() == "KeyFlag")
	{
		ClearFlag_ = true;
	}


	if (parent->GetObjectName() == "MoveEnemy" && pResidue != nullptr)
	{
		isHitEnemy_ = true;
		playerstate = PLAYER_STATE::DEAD;
	}

	if (parent->GetObjectName() == "GoalDoor")
	{
		openGoal_ = true;

		if (openGoal_ && ClearFlag_)
		{
			playerstate = PLAYER_STATE::CLEAR;
		}
	}


	ResidueItem* pResidueItem = (ResidueItem*)FindObject("ResidueItem");
	if (parent->GetObjectName() == "ResidueItem" && pResidue != nullptr)
	{
		GetRubyflag_ = true;

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

		if (pSceneManager != nullptr)
		{
			int currentResidue = pSceneManager->GetPlayerResidue();

			pSceneManager->SetPlayerResidue(currentResidue + 1);
		}

		pResidueItem->KillMe();
	}

}

void Player::SetPlayerAnimation(int AnimeType)
{
	switch (AnimeType)
	{
	case 0:
		hPlayerModel_ = hPlayerAnimeModel_[0];
		break;
	case 1:
		hPlayerModel_ = hPlayerAnimeModel_[1];
		break;
	case 2:
		hPlayerModel_ = hPlayerAnimeModel_[2];
		break;
	case 3:
		hPlayerModel_ = hPlayerAnimeModel_[3];
		break;
	case 4:
		hPlayerModel_ = hPlayerAnimeModel_[4];
		break;
	case 5:
		hPlayerModel_ = hPlayerAnimeModel_[5];
		break;
	case 6:
		hPlayerModel_ = hPlayerAnimeModel_[6];
		break;
	case 7:
		hPlayerModel_ = hPlayerAnimeModel_[7];
		break;
	default:
		break;
	}

}
