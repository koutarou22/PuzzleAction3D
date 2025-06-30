#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Engine/Input.h"
#include "Stage.h"

namespace
{
	const float SRATE = 0.07f;//補間速度
	const float GRAVITY = 0.005f;

	// 移動補間用の状態変数
	XMFLOAT3 prepos;   //開始
	XMFLOAT3 nextpos;  //到達地点
	float moveRatio = 0.0f;

	// ジャンプ／落下状態フラグ
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

	// 動作・移動・ジャンプ補間
	const float MOVE_INTERPOLATION_SPEED = 0.07f;   // SRATE：補間速度
	const float MOVE_RATIO_INITIAL = 0.001f;
	const float PLAYER_GRAVITY = 0.005f;
	const float JUMP_PARABOLA_COEFF = 5.0f;
	float JUMP_HEIGHT = 1.0f;

	// モデルのSize
	const float INITIAL_PLAYER_SCALE = 0.5f;
	const float INITIAL_PLAYER_X = 1.0f;
	const float INITIAL_PLAYER_Y = 1.0f;
	const float INITIAL_PLAYER_Z = 0.0f;
	const float COLLIDER_OFFSET_Y = 0.5f;
	const float COLLIDER_SIZE = 1.0f;

	// グリッド座標変換
	const int STAGE_GRID_WIDTH = 10;
	const int STAGE_GRID_HEIGHT = 10;
	const int STAGE_HEIGHT_MAX = 10;
	const int STAGE_OFFSET_X = 5;
	const int STAGE_OFFSET_Z = 4;

	// プレイヤー補正やスナップ
	const float GRID_UNIT = 1.0f;
	const float GRID_Y_UNIT = 0.5f;
	const float ROTATION_OFFSET_DEGREES = 180.0f;
	const float BLOCK_PLACE_VERTICAL_OFFSET = 0.5f; 
	const float GRID_VERTICAL_UNIT = 0.5f;         


	// ブロック配置
	const float BLOCK_PLACE_DISTANCE = 1.0f;
	const float BLOCK_PLACE_HEIGHT_OFFSET = 0.5f;

	// ブロックID（判定用）
	const int STAGE_BLOCK_EMPTY = 0;
	const int STAGE_BLOCK_GHOST = 1;
	const int STAGE_BLOCK_TURRET = 2;
	const int STAGE_BLOCK_KEY = 3;
	const int STAGE_BLOCK_DOOR = 4;
	const int STAGE_BLOCK_GROUND = 5;
	const int STAGE_BLOCK_RESIDUE = 6;
	const int STAGE_BLOCK_PLAYER_BLOCK = 7;

	const float PLAYER_ROTATE_OFFSET_DEG = 180;

	const float PARABOLA_COEFFICIENT = 5.0f;
}


namespace PLAYER_ANIME_FRAME
{
	const int IDOL_ANIMATION_FRAME = 120;  //待機フレーム
	const int MOVE_ANIMATION_FRAME = 30;   //移動フレーム
	const int SETTING_ANIMATION_FRAME = 80;//Block設置時フレーム
	const int ATTACK_ANIMATION_FRAME = 129;//Block攻撃時フレーム
	const int JUMP_ANIMATION_FRAME =30;   //ジャンプフレーム
	const int DAMAGE_ANIMATION_FRAME = 104;//やられフレーム
	const int VICTORY_ANIMATION_FRAME = 50;//ゴールフレーム
	const int FALL_ANIMATION_FRAME = 32;   //落下中フレーム
	const int LANDING_ANIMATION_FRAME = 35;//着地フレーム
}

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "PlayerBlock.h"
#include "Engine/SceneManager.h"
#include "Residue.h"
#include "ResidueItem.h"
#include "KeyFlag.h"
#include "Ghost.h"
#include "CameraController.h"


MOVE_METHOD Player::CanMoveTo(const XMFLOAT3& pos)
{
	int gx = static_cast<int>(roundf(pos.x + STAGE_OFFSET_X));
	int gy = static_cast<int>(roundf(STAGE_OFFSET_Z - pos.z));
	int gz = static_cast<int>(roundf(pos.y));

	if (gx < 0 || gx >= STAGE_GRID_WIDTH ||
		gy < 0 || gy >= STAGE_GRID_HEIGHT ||
		gz < 0 || gz >= STAGE_HEIGHT_MAX)
	{
		Debug::Log("移動範囲外 & 動けない", true);
		return CANT_MOVE;
	}

	auto* stage = static_cast<Stage*>(FindObject("Stage"));
	auto& grid = stage->GetStageGrid();

	int current = grid[gz][STAGE_GRID_HEIGHT - 1 - gy][gx];

	if (current == STAGE_BLOCK_EMPTY && gz > 0)
	{
		int under = grid[gz - 1][STAGE_GRID_HEIGHT - 1 - gy][gx];
		if (under == STAGE_BLOCK_EMPTY)
		{
			Debug::Log("落下中に切り替わった", true);
			return CAN_MOVE_FALL;
		}
		else
		{
			Debug::Log("移動中に切り替わった", true);
			return CAN_MOVE_WALK;
		}
	}
	else if (
		current == STAGE_BLOCK_GHOST ||
		current == STAGE_BLOCK_KEY ||
		current == STAGE_BLOCK_DOOR ||
		current == STAGE_BLOCK_RESIDUE)
	{
		return CAN_MOVE_WALK;
	}

	if (gz + 1 < STAGE_HEIGHT_MAX)
	{
		int above = grid[gz + 1][STAGE_GRID_HEIGHT - 1 - gy][gx];
		if (above == STAGE_BLOCK_EMPTY ||
			above == STAGE_BLOCK_GHOST ||
			above == STAGE_BLOCK_KEY ||
			above == STAGE_BLOCK_DOOR ||
			above == STAGE_BLOCK_RESIDUE)
		{
			return CAN_MOVE_JUMP;
		}
	}
	else if (gz + 2 < STAGE_HEIGHT_MAX)
	{
		int twoabove = grid[gz + 2][STAGE_GRID_HEIGHT - 1 - gy][gx];
		if (twoabove != STAGE_BLOCK_EMPTY && current != STAGE_BLOCK_EMPTY)
		{
			Debug::Log("ニマス以上を超えています");
		}
		return CANT_JUMP;
	}

	return CANT_MOVE;
}


void Player::StandingStage(const XMFLOAT3& pos)
{
	int gx = static_cast<int>(roundf(pos.x + STAGE_OFFSET_X));
	int gy = static_cast<int>(roundf(STAGE_OFFSET_Z - pos.z));

	auto* stage = static_cast<Stage*>(FindObject("Stage"));
	auto& grid = stage->GetStageGrid();

	for (int y = STAGE_HEIGHT_MAX - 1; y >= 0; --y)
	{
		int current = grid[y][STAGE_GRID_HEIGHT - 1 - gy][gx];

		if (current == STAGE_BLOCK_GROUND ||
			current == STAGE_BLOCK_TURRET ||
			current == STAGE_BLOCK_PLAYER_BLOCK)
		{
			GROUND = y + 1;
			return;
		}
	}
}


XMFLOAT3 Player::GetInputDirection()
{
	dir_ = { 0, 0, 0 };

	XMFLOAT3 LeftStick = Input::GetPadStickL(0);

	//移動(奥)
	if (Input::IsKey(DIK_W) || LeftStick.y >= STICK_DEADZONE)       dir_.z += MOVE_GRID;
	//移動(手前)
	else if (Input::IsKey(DIK_S) || LeftStick.y <= -STICK_DEADZONE) dir_.z -= MOVE_GRID;
	//移動(左)
	else if (Input::IsKey(DIK_A) || LeftStick.x <= -STICK_DEADZONE) dir_.x -= MOVE_GRID;
	//移動(右)
	else if (Input::IsKey(DIK_D) || LeftStick.x >= STICK_DEADZONE)  dir_.x += MOVE_GRID;

	if (Input::IsKeyDown(DIK_L) || Input::IsPadButton(XINPUT_GAMEPAD_B))
	{
		PlayerBlockInstans();
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

	SetPlayerAnimation(5);
	animationDeadTimer_--;

	if (animationDeadTimer_ <= 0)
	{
		isHitEnemy_ = false;

		// 残機を減らす
		int currentResidue = pSceneManager->GetPlayerResidue();
		if (currentResidue > 0)
		{
			pSceneManager->SetPlayerResidue(currentResidue - 1); // 残機を1減らす
		}

		//残機が1以上なら LoadScene に戻す
		if (pSceneManager->GetPlayerResidue() > 0)
		{
			pSceneManager->ChangeScene(SCENE_ID_LOAD);
		}
		else
		{
			pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
		}
	}
}

void Player::ClearAnimation()
{
	if (openGoal_ && ClearFlag_)
	{
		SetPlayerAnimation(6);
		animationVictoryTimer_--;
		if (animationVictoryTimer_ <= 0)
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_CLEAR);
		}
	}
}

MOVE_METHOD Player::PlayerBlockInstans()
{
	// 既存ブロックがあれば削除
	PlayerBlock* existingBlock = (PlayerBlock*)FindObject("PlayerBlock");
	if (existingBlock != nullptr)
	{
		XMFLOAT3& oldPos = existingBlock->GetPosition();
		int oldGx = static_cast<int>(oldPos.x + STAGE_OFFSET_X);
		int oldGy = static_cast<int>(STAGE_OFFSET_Z - oldPos.z);
		int oldGz = static_cast<int>(oldPos.y);

		auto* stage = static_cast<Stage*>(FindObject("Stage"));
		if (stage)
		{
			auto& grid = stage->GetStageGrid();
			grid[oldGz][STAGE_GRID_HEIGHT - 1 - oldGy][oldGx] = STAGE_BLOCK_EMPTY;
			StandingStage(transform_.position_);
		}

		existingBlock->KillMe();
	}

	// プレイヤーの位置と方向設定

	XMVECTOR PlayerPos = XMLoadFloat3(&(transform_.position_));

	XMVECTOR FrontDirection = XMVectorSet(0.0f, BLOCK_PLACE_VERTICAL_OFFSET, -1.0f, 0.0f);

	XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

	XMVECTOR blockPos = PlayerPos + FrontDirection * BLOCK_PLACE_DISTANCE;

	//グリッド位置
	XMVECTOR snappedBlockPos = XMVectorSet(
		round(XMVectorGetX(blockPos) / GRID_UNIT) * GRID_UNIT,
		round(XMVectorGetY(blockPos) / GRID_VERTICAL_UNIT) * GRID_VERTICAL_UNIT,
		round(XMVectorGetZ(blockPos) / GRID_UNIT) * GRID_UNIT,
		0.0f
	);

	// グリッド変換
	int gx = static_cast<int>(XMVectorGetX(snappedBlockPos) + STAGE_OFFSET_X);
	int gy = static_cast<int>(STAGE_OFFSET_Z - XMVectorGetZ(snappedBlockPos));
	int gz = static_cast<int>(XMVectorGetY(snappedBlockPos));

	auto* stage = static_cast<Stage*>(FindObject("Stage"));

	auto& grid = stage->GetStageGrid();

	// 設置条件チェック　画面外に置けないようにする--------------------
	if (gx < 0 || gx >= STAGE_GRID_WIDTH ||
		gy < 0 || gy >= STAGE_GRID_HEIGHT ||
		gz < 0 || gz >= STAGE_HEIGHT_MAX)
	{
		return CANT_MOVE;
	}

	int cellValue = grid[gz][STAGE_GRID_HEIGHT - 1 - gy][gx];
	if (cellValue != STAGE_BLOCK_EMPTY && cellValue != STAGE_BLOCK_PLAYER_BLOCK)
	{
		return CANT_MOVE;
	}
	//----------------------------------------------------------------

	// ブロック設置
	PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, snappedBlockPos);
	block->SetPosition(pos);

	//設置Animation
	SetPlayerAnimation(ANIM_SETTING);

	grid[gz][STAGE_GRID_HEIGHT - 1 - gy][gx] = STAGE_BLOCK_PLAYER_BLOCK;

	// 上に他のブロックがあるか
	if (gz + 1 < STAGE_HEIGHT_MAX)
	{
		int above = grid[gz + 1][STAGE_GRID_HEIGHT - 1 - gy][gx];
		if (above == STAGE_BLOCK_PLAYER_BLOCK)
		{
			return CAN_MOVE_JUMP;
		}
	}

	return CANT_MOVE;
}



void Player::PlayerGridCorrection()
{


	float gridSize = 1.0f;
	float x = round((transform_.position_.x) / gridSize) * gridSize;
	float y = round((transform_.position_.y) / gridSize) * gridSize;
	float z = round((transform_.position_.z) / gridSize) * gridSize;
	transform_.position_ = { x,y,z };
}

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"), hPlayerModel_(-1), playerstate(PLAYER_STATE::MOVE), isHitEnemy_(false)
{
}

void Player::Initialize()
{
	transform_.scale_ = { INITIAL_PLAYER_SCALE, INITIAL_PLAYER_SCALE, INITIAL_PLAYER_SCALE };
	transform_.position_ = { 0, INITIAL_PLAYER_Y, 0 };

	isJumping = false;
	IsJumpInterpolation = false;

	BoxCollider* collision = new BoxCollider({ 0, COLLIDER_OFFSET_Y, 0 }, { COLLIDER_SIZE, COLLIDER_SIZE, COLLIDER_SIZE });
	AddCollider(collision);

	// アニメーションの登録

	//待機
	hPlayerAnimeModel_[ANIM_IDLE] = Model::Load("Animation//Breathing Idle.fbx");
	assert(hPlayerAnimeModel_[ANIM_IDLE] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_IDLE], 0, PLAYER_ANIME_FRAME::IDOL_ANIMATION_FRAME, 1.0);

	//移動
	hPlayerAnimeModel_[ANIM_MOVE] = Model::Load("Animation//Standard Walk.fbx");
	assert(hPlayerAnimeModel_[ANIM_MOVE] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_MOVE], 0, PLAYER_ANIME_FRAME::MOVE_ANIMATION_FRAME, 1.0);

	//設置
	hPlayerAnimeModel_[ANIM_SETTING] = Model::Load("Animation//Magic Heal.fbx");
	assert(hPlayerAnimeModel_[ANIM_SETTING] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_SETTING], 0, PLAYER_ANIME_FRAME::SETTING_ANIMATION_FRAME, 1.0);

	//攻撃　※未実装
	hPlayerAnimeModel_[ANIM_ATTACK] = Model::Load("Animation//Standing 2H Magic Attack.fbx");
	assert(hPlayerAnimeModel_[ANIM_ATTACK] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_ATTACK], 0, PLAYER_ANIME_FRAME::ATTACK_ANIMATION_FRAME, 1.0);

	//ジャンプ
	hPlayerAnimeModel_[ANIM_JUMP] = Model::Load("Animation//Jump.fbx");
	assert(hPlayerAnimeModel_[ANIM_JUMP] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_JUMP], 0, PLAYER_ANIME_FRAME::JUMP_ANIMATION_FRAME, 1.0);

	//死亡
	hPlayerAnimeModel_[ANIM_DEAD] = Model::Load("Animation//Down.fbx");
	assert(hPlayerAnimeModel_[ANIM_DEAD] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_DEAD], 0, PLAYER_ANIME_FRAME::DAMAGE_ANIMATION_FRAME, 1.0);

	//勝利
	hPlayerAnimeModel_[ANIM_VICTORY] = Model::Load("Animation//Victory Idle.fbx");
	assert(hPlayerAnimeModel_[ANIM_VICTORY] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_VICTORY], 0, PLAYER_ANIME_FRAME::VICTORY_ANIMATION_FRAME, 1.0);

	//落下中
	hPlayerAnimeModel_[ANIM_FALL] = Model::Load("Animation//Fall A Loop.fbx");
	assert(hPlayerAnimeModel_[ANIM_FALL] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_FALL], 0, PLAYER_ANIME_FRAME::FALL_ANIMATION_FRAME, 1.0);

	//着地
	hPlayerAnimeModel_[ANIM_LANDING] = Model::Load("Animation//Landing2.fbx");
	assert(hPlayerAnimeModel_[ANIM_LANDING] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[ANIM_LANDING], 10, PLAYER_ANIME_FRAME::LANDING_ANIMATION_FRAME, 1.0);


	//初期アニメーションをセット
	hPlayerModel_ = hPlayerAnimeModel_[ANIM_IDLE];

	// 残機を持ってくる
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager != nullptr)
	{
		Player_Residue = pSceneManager->GetPlayerResidue();
	}
}


void Player::PlayerFallDown()
{
	if (isFalling)
	{
		velocity.y -= PLAYER_GRAVITY;

		transform_.position_.y += velocity.y;

		SetPlayerAnimation(7);

		if (transform_.position_.y <= GROUND)
		{
			SetPlayerAnimation(8);
			animationLandingTimer_--;
			if (animationLandingTimer_ <= 0)
			{
				animationLandingTimer_ = PLAYER_ANIME_FRAME::LANDING_ANIMATION_FRAME;
				SetPlayerAnimation(0);
			}

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

		//放物線補間
		float offsetY =
			PARABOLA_COEFFICIENT * // 放物線の係数
			JUMP_HEIGHT *          // ジャンプの最大高
			moveRatio *            // 開始 → 終了進行度（0.0~1.0）
			(1.0f - moveRatio);    // 進行度に対して対称な減少


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

			SetPlayerAnimation(0);
		}
	}
}

void Player::Jump(const XMFLOAT3& inputDir)
{
	if (onGround)
	{
		prepos = transform_.position_;
		nextpos = inputDir;
		nextpos.y = 1.0f;
		moveRatio = 0.0f;
		isJumping = true;
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


	//共有Animationの減少処理
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
	if (!isFalling)
	{
		PlayerMoveMent();
		JumpParabola();
	}
	else if (!IsWalkInterpolation)
	{
		PlayerFallDown();
	}
}

void Player::PlayerMoveMent()
{
	static bool moving = false;
	static float moveRatio = 0.0f;
	static XMFLOAT3 nextpos = { 0, 0, 0 };
	static XMFLOAT3 prepos = { 0, 0, 0 };

	CameraController* pCamera = (CameraController*)FindObject("CameraController");
	Stage* pStage = (Stage*)FindObject("Stage");

	XMFLOAT3 inputDir = GetInputDirection();

	if (pCamera->IsRotating() || IsJumpInterpolation)
	{
		inputDir = { 0, 0, 0 };
		return;
	}

	if (!moving)
	{
		if (!IsZero(inputDir))
		{
			// 入力方向をカメラの回転に合わせて変換
			XMMATRIX camRotMatrix = pCamera->GetRotationMatrix();
			XMVECTOR inputVec = XMLoadFloat3(&inputDir);
			XMVECTOR rotatedVec = XMVector3TransformCoord(inputVec, camRotMatrix);
			XMStoreFloat3(&nextpos, rotatedVec);

			XMFLOAT3 target = AddXMFLOAT3(transform_.position_, nextpos);
			MOVE_METHOD method = CanMoveTo(target);

			switch (method)
			{
			case CAN_MOVE_WALK:
				SetPlayerAnimation(ANIM_MOVE);
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

			//補間が終わったら
			if (!IsWalkInterpolation)
			{
				//ズレ防止で補正する
				PlayerGridCorrection();
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
		}
	}

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
	Residue* pResidue = (Residue*)FindObject("Residue");
	ResidueItem* pResidueItem = (ResidueItem*)FindObject("ResidueItem");


	if (parent->GetObjectName() == "KeyFlag")
	{
		ClearFlag_ = true;
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
	}


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


