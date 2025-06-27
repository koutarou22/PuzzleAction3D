#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Engine/Input.h"
#include "Stage.h"

namespace
{
	const float SRATE = 0.07f;//補間速度
	const float GRAVITY = 0.005f;

	const int GRID_WIDTH = 10;
	const int GRID_HEIGHT = 10;
	const int MAX_STAGE_HEIGHT = 10;
	const int GRID_OFFSET_X = 5;
	const int GRID_OFFSET_Z = 4;


	int GROUND = 1.0f;
	float JUMP_HEIGHT = 1.0f;//ジャンプ力

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
	const int IDOL_ANIMATION_FRAME = 298;//待機アニメーション
	const int MOVE_ANIMATION_FRAME = 30;//移動アニメーションのフレーム
	const int SETTING_ANIMATION_FRAME = 80;//Block設置時のアニメーションのフレーム
	const int ATTACK_ANIMATION_FRAME = 129;//Block攻撃時のアニメーションのフレーム
	//57
	const int JUMP_ANIMATION_FRAME = 27;//ジャンプアニメーションのフレーム
	const int DAMAGE_ANIMATION_FRAME = 104;//やられアニメーションのフレーム
	const int VICTORY_ANIMATION_FRAME = 50;//ゴールアニメーションのフレーム
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
	int gx = static_cast<int>(pos.x + GRID_OFFSET_X);
	int gy = static_cast<int>(GRID_OFFSET_Z - pos.z);
	int gz = static_cast<int>(pos.y);

	//画面外にいかない処理
	if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT || gz < 0 || gz >= MAX_STAGE_HEIGHT)
	{
		Debug::Log("移動範囲外 & 動けない", true);
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
			Debug::Log("落下中に切り替わった", true);
			return CAN_MOVE_FALL;
		}
		else
		{
			Debug::Log("移動中に切り替わった", true);
			return CAN_MOVE_WALK;
		}
	}
	else if (current == 1 || current == 3 || current == 4 || current == 6)
	{
		return CAN_MOVE_WALK;
	}


	if (gz + 1 < MAX_STAGE_HEIGHT)
	{
		int above = grid[gz + 1][GRID_HEIGHT - 1 - gy][gx];
		if (above == 0 || above == 1 || above == 3 || above == 4 || above == 6)
			return CAN_MOVE_JUMP;
	}
	else if (gz + 2 < MAX_STAGE_HEIGHT)
	{
		int twoabove = grid[gz + 2][GRID_HEIGHT - 1 - gy][gx];
		if (twoabove != 0 && current != 0)
			Debug::Log("ニマス以上を超えています");
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
		int current = grid[y][GRID_HEIGHT - 1 - gy][gx];

		// 5
		if (current == 5 || current == 2 || current == 7)
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

	//カメラが動いていなければ ・ 敵に接触していなければ ・ ゴールに接触していなければ　動いていい
	if (!isMoveCamera_ || !isHitEnemy_ || !openGoal_)
	{
		//移動(奥)
		if (Input::IsKey(DIK_W) || LeftStick.y >= STICK_DEADZONE)  dir_.z += MOVE_GRID;
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

	// 敵に接触したときの処理
	if (isHitEnemy_)
	{
		SetPlayerAnimation(5);
		DeadAnimationTimer_--;

		if (DeadAnimationTimer_ <= 0)
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

			DeadAnimationTimer_ = PLAYER_ANIME_FRAME::DAMAGE_ANIMATION_FRAME;//最後に元のフレームに戻してあげる
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

MOVE_METHOD Player::PlayerBlockInstans()
{
	// 既にブロックが存在するなら削除
	PlayerBlock* existingBlock = (PlayerBlock*)FindObject("PlayerBlock");
	if (existingBlock != nullptr)
	{
		// 1. 既存ブロックの位置からグリッド座標を計算して、grid を 0 に戻す
		const XMFLOAT3& oldPos = existingBlock->GetPosition();
		int oldGx = static_cast<int>(oldPos.x + GRID_OFFSET_X);
		int oldGy = static_cast<int>(GRID_OFFSET_Z - oldPos.z);
		int oldGz = static_cast<int>(oldPos.y);

		auto* stage = static_cast<Stage*>(FindObject("Stage"));
		if (stage != nullptr)
		{
			auto& grid = stage->GetStageGrid();
			grid[oldGz][GRID_HEIGHT - 1 - oldGy][oldGx] = 0;
		}

		// 2. ブロックを削除
		existingBlock->KillMe();
		
	}

	// プレイヤーの位置を持ってくる
	XMVECTOR PlayerPos = XMLoadFloat3(&(transform_.position_));

	// デフォルトの前方向を設定する
	XMVECTOR FrontDirection = XMVectorSet(0.0f, 0.5f, -1.0f, 0.0f);

	
	if (Input::IsKey(DIK_UP) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		FrontDirection = XMVectorSet(0, 1.5f, -1.0f, 0.0f);
	}
	else if (Input::IsKey(DIK_DOWN) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		FrontDirection = XMVectorSet(0, -0.5f, -1.0f, 0.0f);
	}

	// プレイヤーの回転角度に基づいて前方向を回転
	XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

	// ブロックを1マス分前に出す
	XMVECTOR blockPos = PlayerPos + FrontDirection * 1.0f;

	float gridSize = 1.0f;
	XMVECTOR snappedBlockPos = XMVectorSet(
		round(XMVectorGetX(blockPos) / gridSize) * gridSize,
		round(XMVectorGetY(blockPos) / 0.5) * 0.5,
		round(XMVectorGetZ(blockPos) / gridSize) * gridSize,
		0.0f
	);

	// ブロックのインスタンスを生成してスナップ座標に配置 
	PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
	XMFLOAT3 pos;

	XMStoreFloat3(&pos, snappedBlockPos);
	block->SetPosition(pos);


	// アニメーション処理を呼コム
	SetPlayerAnimation(2);
	MoveAnimationTimer_ = PLAYER_ANIME_FRAME::SETTING_ANIMATION_FRAME;

	// snappedBlockPos をグリッド座標に変換して 7 を登録
	//やってることはCanMoveto とほぼ同じ
	int gx = static_cast<int>(XMVectorGetX(snappedBlockPos) + GRID_OFFSET_X);
	int gy = static_cast<int>(GRID_OFFSET_Z - XMVectorGetZ(snappedBlockPos));
	int gz = static_cast<int>(XMVectorGetY(snappedBlockPos));

	auto* stage = static_cast<Stage*>(FindObject("Stage"));
	if (stage)
	{
		auto& grid = stage->GetStageGrid();
		grid[gz][GRID_HEIGHT - 1 - gy][gx] = 7;

		int current = grid[gz][GRID_HEIGHT - 1 - gy][gx];

		if (gz + 1 < MAX_STAGE_HEIGHT)
		{
			int above = grid[gz + 1][GRID_HEIGHT - 1 - gy][gx];
			if (above == 7)
				return CAN_MOVE_JUMP;
		}
	}
}


Player::Player(GameObject* parent)
	: GameObject(parent, "Player"), hPlayerModel_(-1), playerstate(PLAYER_STATE::MOVE),isHitEnemy_(false)
{
}

void Player::Initialize()
{
	hPlayerModel_ = Model::Load("NewPlayer.fbx");
	transform_.scale_ = { 0.5f, 0.5f, 0.5f };
	transform_.position_ = { 0, 1.0f, 0 };

	isJumping = false;
	IsJumpInterpolation = false;

	BoxCollider* collision = new BoxCollider({ 0, 0.5, 0 }, { 1, 1, 1 });
	AddCollider(collision);

	string Path = "Animation//";
	string Fbx = ".fbx";

	// 待機状態
	hPlayerAnimeModel_[0] = Model::Load("Animation//Breathing Idle2.fbx");
	assert(hPlayerAnimeModel_[0] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[0], 0, PLAYER_ANIME_FRAME::IDOL_ANIMATION_FRAME, 1.0);

	// 歩くモーション
	hPlayerAnimeModel_[1] = Model::Load("Animation//Standard Walk.fbx");
	assert(hPlayerAnimeModel_[1] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[1], 0, PLAYER_ANIME_FRAME::MOVE_ANIMATION_FRAME, 1.0);

	// Blockを作った時
	hPlayerAnimeModel_[2] = Model::Load("Animation//Magic Heal.fbx");
	assert(hPlayerAnimeModel_[2] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[2], 0, PLAYER_ANIME_FRAME::SETTING_ANIMATION_FRAME, 1.0);

	// Blockを押し出したとき
	hPlayerAnimeModel_[3] = Model::Load("Animation//Standing 2H Magic Attack.fbx");
	assert(hPlayerAnimeModel_[3] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[3], 0, PLAYER_ANIME_FRAME::ATTACK_ANIMATION_FRAME, 1.0);

	// ジャンプしたとき
	hPlayerAnimeModel_[4] = Model::Load("Animation//Jumping.fbx");
	assert(hPlayerAnimeModel_[4] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[4], 10, PLAYER_ANIME_FRAME::JUMP_ANIMATION_FRAME, 1.2);

	// やられたとき
	hPlayerAnimeModel_[5] = Model::Load("Animation//Down.fbx");
	assert(hPlayerAnimeModel_[5] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[5], 0, PLAYER_ANIME_FRAME::DAMAGE_ANIMATION_FRAME, 1.0);

	// ゴールしたとき
	hPlayerAnimeModel_[6] = Model::Load("Animation//Victory Idle.fbx");
	assert(hPlayerAnimeModel_[6] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[6], 0, PLAYER_ANIME_FRAME::VICTORY_ANIMATION_FRAME, 1.0);


	SetPlayerAnimation(0);

	//残機の処理
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager != nullptr)
	{
		//残機の値をSceneManagerからとってくる
		Player_Residue = pSceneManager->GetPlayerResidue();
	}

	//予めフレームを入れておく
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

		//放物線補間
		float offsetY =
			5.0f *                 // 放物線の係数（高さのスケーリング調整、最大高さを JUMP_HEIGHT にするための定数）
			JUMP_HEIGHT *          // ジャンプの最大高さ（この値をピークにする）
			moveRatio *            // 開始 → 終了への進行度（0.0~1.0）
			(1.0f - moveRatio);    // 進行度に対して対称な減少（左右対称の放物線を作る）


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


	// アニメーションタイマーを減少させる
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

	CameraController* pCamera = (CameraController*)FindObject("CameraController");
	Stage* pStage = (Stage*)FindObject("Stage");

	// カメラが回転中ならプレイヤーの移動処理をスキップ
	if (pCamera->IsRotating() || IsJumpInterpolation) return;

	if (!moving)
	{
		nextpos = GetInputDirection();

		if (!IsZero(nextpos))
		{
			// ※カメラ回転を加味しない方向入力（グリッド基準）
			XMFLOAT3 target = AddXMFLOAT3(transform_.position_, nextpos);
			MOVE_METHOD method = CanMoveTo(target);

			switch (method)
			{
			case CAN_MOVE_WALK:
				SetPlayerAnimation(1);
				moving = true;
				IsWalkInterpolation = true;
				moveRatio = 0.001f;
				prepos = transform_.position_;
				onGround = true;
				break;

			case CAN_MOVE_FALL:

				prepos = transform_.position_;
				moveRatio = 0.001f;
				moving = true;
				IsWalkInterpolation = true;
				deferFall = true;
				playerstate = PLAYER_STATE::FALL;
				break;

			case CAN_MOVE_JUMP:
				Jump(nextpos);
				
				break;

			default:
				SetPlayerAnimation(0);
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

			SetPlayerAnimation(0);

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

	// 入力方向に応じてキャラを回転させる処理
	if (!IsZero(nextpos))
	{
		float angle = atan2(nextpos.x, nextpos.z);
		transform_.rotate_.y = XMConvertToDegrees(angle) + 180.0f;
	}
}

void Player::Draw()
{
	Model::SetTransform(hPlayerModel_, transform_);
	Model::Draw(hPlayerModel_);

	//Debug
	{
		ImGui::Begin("Player_Debug");

		ImGui::Text("Player Position%5.2lf,%5.2lf,%5.2lf", transform_.position_.x, transform_.position_.y, transform_.position_.z);

		ImGui::Separator();
		ImGui::Text("MoveAnimationTimer%5.2lf", MoveAnimationTimer_);
		ImGui::Text("DeadAnimationTimer%5.2lf", DeadAnimationTimer_);
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
	Ghost* pGhost = (Ghost*)FindObject("Ghost");
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Residue* pResidue = (Residue*)FindObject("Residue");

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