#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/BoxCollider.h"
#include "Stage.h"
#include <iostream>
#include "Engine/Debug.h"
#include "PlayerBlock.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

#include "KeyFlag.h"
#include "Ladder.h"
#include "MoveEnemy.h"
#include "Engine/SceneManager.h"
#include "Engine/Camera.h"
#include "CameraController.h"
#include "ResidueItem.h"
#include "Residue.h"



namespace
{
	//移動をフレームで補間用 (テスト)
	//ここの数値で移動(見た目)速度を操作する
	const int PLAYER_MOVE_INTERPOLATION = 20;

	const int   MAX_RANGE = 9;//プレイヤーが行ける範囲
	const float MAX_MOVE_FRAME = 10;//プレイヤーが再度動けるまでのフレーム

	const float MOVE_SPEED = 1.0f / PLAYER_MOVE_INTERPOLATION;//プレイヤーの移動速度
	const float MOVE_AERIAL = 0.05f;//プレイヤーの移動速度

	const float GROUND = 1.0f;//初期位置(Y)
	const float GROUND_LIMIT = 1.0f;
	const float JUMP_HEIGHT = 1.2f;//ジャンプ力
	const float GRAVITY = 0.005f;//重力
	const float MAX_GRAVITY = 6.0f;

	const int PLAYER_HEIGHT = 1.5f;

}

namespace AnimaFrame
{
	//59
	const int IDOL_ANIMATION_FRAME = 298;//待機アニメーション
	const int MOVE_ANIMATION_FRAME = 30;//移動アニメーションのフレーム
	const int SETTING_ANIMATION_FRAME = 80;//Block設置時のアニメーションのフレーム
	const int ATTACK_ANIMATION_FRAME = 129;//Block攻撃時のアニメーションのフレーム
	const int JUMP_ANIMATION_FRAME = 57;//ジャンプアニメーションのフレーム
	const int DAMAGE_ANIMATION_FRAME = 104;//やられアニメーションのフレーム
	const int VICTORY_ANIMATION_FRAME = 50;//ゴールアニメーションのフレーム
}

void Player::UpdateMove()
{
	PlayerControl();
	PlayerRange();
	StageHeight();

	// アニメーションタイマーの進行
	if (MoveAnimationTimer_ > 0)
	{
		MoveAnimationTimer_--;
		if (MoveAnimationTimer_ == 0)
		{
			SetPlayerAnimation(0);
		}
	}

}

void Player::UpdateDead()
{
	DeadAnimation();
}

void Player::UpdateClear()
{
	ClearAnimation();
}


void Player::ClearAnimation()
{
	if (openGoal_ && ClearFlag_)
	{
		SetPlayerAnimation(6);
		VictoryAnimationTimer_--;
		if (VictoryAnimationTimer_ <= 0)
		{
			VictoryAnimationTimer_ = AnimaFrame::VICTORY_ANIMATION_FRAME;

			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_CLEAR);
		}
	}
}

Player::Player(GameObject* parent) : GameObject(parent, "Player")
, ClearFlag_(false), onGround(true), isBlockCanOnly(false), onMyBlock(false), Jump_Power(0.0f), hPlayerModel_(-1), MoveTimer_(MAX_MOVE_FRAME), isHitEnemy_(false), openGoal_(false), GetRubyflag(false)
, isMoveCamera_(false)
{

	isMove_interpolation = false;

	isGoMove = false;
}

Player::~Player()
{
}

void Player::Initialize()
{

	string Path = "Animation//";
	string Fbx = ".fbx";

	// 待機状態
	hPlayerAnimeModel_[0] = Model::Load("Animation//Breathing Idle.fbx");
	assert(hPlayerAnimeModel_[0] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[0], 0, AnimaFrame::IDOL_ANIMATION_FRAME, 1.0);

	// 歩くモーション
	hPlayerAnimeModel_[1] = Model::Load("Animation//Standard Walk.fbx");
	assert(hPlayerAnimeModel_[1] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[1], 0, AnimaFrame::MOVE_ANIMATION_FRAME, 1.0);

	// Blockを作った時
	hPlayerAnimeModel_[2] = Model::Load("Animation//Magic Heal.fbx");
	assert(hPlayerAnimeModel_[2] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[2], 0, AnimaFrame::SETTING_ANIMATION_FRAME, 1.0);

	// Blockを押し出したとき
	hPlayerAnimeModel_[3] = Model::Load("Animation//Standing 2H Magic Attack.fbx");
	assert(hPlayerAnimeModel_[3] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[3], 0, AnimaFrame::ATTACK_ANIMATION_FRAME, 1.0);

	// ジャンプしたとき
	hPlayerAnimeModel_[4] = Model::Load("Animation//Jumping.fbx");
	assert(hPlayerAnimeModel_[4] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[4], 10, AnimaFrame::JUMP_ANIMATION_FRAME, 1.2);

	// やられたとき
	hPlayerAnimeModel_[5] = Model::Load("Animation//Down.fbx");
	assert(hPlayerAnimeModel_[5] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[5], 0, AnimaFrame::DAMAGE_ANIMATION_FRAME, 1.0);

	// ゴールしたとき
	hPlayerAnimeModel_[6] = Model::Load("Animation//Victory Idle.fbx");
	assert(hPlayerAnimeModel_[6] >= 0);
	Model::SetAnimFrame(hPlayerAnimeModel_[6], 0, AnimaFrame::VICTORY_ANIMATION_FRAME, 1.0);

	// プレイヤー初期位置
	transform_.position_ = { 3, 1.0, posZ };

	// コライダーの追加
	BoxCollider* collision = new BoxCollider({ 0, 0.55, 0 }, { 0.8, 1, 0.8 });
	AddCollider(collision);


	SetPlayerAnimation(0);

	transform_.scale_ = { 0.6, 0.6, 0.6 };

	isMove_ = PLAYER_MOVE_INTERPOLATION;

	MoveDirection = NONE;
	player_state = MOVE;

	//残機の処理
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager != nullptr)
	{
		//残機の値をSceneManagerからとってくる
		Player_Residue = pSceneManager->GetPlayerResidue();
	}

	//予めフレームを入れておく
	//MoveAnimationTimer_ = AnimaFrame::MOVE_ANIMATION_FRAME;
	DeadAnimationTimer_ = AnimaFrame::DAMAGE_ANIMATION_FRAME;
	VictoryAnimationTimer_ = AnimaFrame::VICTORY_ANIMATION_FRAME;
}


void Player::Update()
{
	switch (player_state)
	{
	case Player::MOVE:
		UpdateMove();
		break;
	case Player::DEAD:
		UpdateDead();
		break;
	case Player::CLEAR:
		UpdateClear();
		break;
	default:
		break;
	}
}

void Player::Draw()
{
	Model::SetTransform(hPlayerModel_, transform_);
	Model::Draw(hPlayerModel_);

	{
		ImGui::Text("Player Position%5.2lf,%5.2lf,%5.2lf", transform_.position_.x, transform_.position_.y, transform_.position_.z);
		ImGui::Text("Player Jump Pawer%5.2lf", Jump_Power);
		ImGui::Text("Player Move CoolTime %d", isMove_);
		ImGui::Separator();
		ImGui::Text("MoveAnimationTime %d", MoveAnimationTimer_);
		ImGui::Text("DeadAnimationTime %d", DeadAnimationTimer_);
		ImGui::Text("VictoryAnimationTime %d", VictoryAnimationTimer_);

		{
			static float pos[3] = { posX,posY,posZ };
			ImGui::Separator();

			if (ImGui::InputFloat3("Player_Position", pos, "%.3f"))
			{
				transform_.position_ = { pos[0],pos[1], pos[2] };
			}
		}
	}
}

void Player::Release()
{

}

void Player::PlayerControl()
{
	// カメラオブジェクトを取得
	CameraController* pCamera = (CameraController*)FindObject("CameraController");
	XMFLOAT3 LeftStick = Input::GetPadStickL(0);
	XMVECTOR move = XMVectorZero();


	XMVECTOR BaseMove = XMVectorZero();
	XMVECTOR NextPosition = XMVectorZero();


	// カメラが動いていなければ ・敵に接触していなければ ・ゴールに接触していなければ　動いていい
	if (!isMoveCamera_ || !isHitEnemy_ || !openGoal_)
	{
		// ジャンプの処理
		if (Input::IsKey(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_A))
		{
			if (!prevSpaceKey && onGround)
			{
				Jump();
			}
			prevSpaceKey = true;
		}
		else
		{
			prevSpaceKey = false;
		}

		// フレームでの移動
		if (isMove_interpolation)
		{
			isMove_--;

			// 進捗を計算
			float progress = (float)isMove_ / PLAYER_MOVE_INTERPOLATION;


			switch (MoveDirection)
			{
			case LEFT:
				PlayerMove(BaseMove, NextPosition, -1.0f, 0.0f, 0.0f);
				break;
			case RIGHT:
				PlayerMove(BaseMove, NextPosition, 1.0f, 0.0f, 0.0f);
				break;
			case FORWARD:
				PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, -1.0f);
				break;
			case BACKWARD:
				PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, 1.0f);
				break;
			default:

				break;
			}



			if (progress <= 0.0f)
			{
				isGoMove = true;
				isMove_interpolation = false;

				if (MoveDirection != NONE)
				{
					MoveDirection = NONE; //確実にリセットする
				}
				//到着したらここで一回停止し初期化
				PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, 0.0f);
				isMove_ = PLAYER_MOVE_INTERPOLATION;


				//マス目を修正
				// PlayerGridCorrection();

			}
			else
			{
				isGoMove = false;
			}


			if (Input::IsKey(DIK_LEFT))
			{
				MoveDirection = LEFT;
			}
			else if (Input::IsKey(DIK_RIGHT))
			{
				MoveDirection = RIGHT;
			}
		}
	}

	//補間が開始してなかったら(途中で切り替えられないようにする処理)
	if (!isMove_interpolation)
	{
		// 左移動の処理
		if (onGround && (Input::IsKey(DIK_A) || LeftStick.x <= -0.5f) && MoveDirection == NONE)
		{
			isMove_interpolation = true;
			MoveDirection = LEFT;
		}
		else if (!onGround && (Input::IsKey(DIK_A) || LeftStick.x <= -0.3f) && MoveDirection == NONE)
		{
			PlayerMove(BaseMove, NextPosition, -1.0f, 0.0f, 0.0f);
			isMove_ = PLAYER_MOVE_INTERPOLATION;
		}

		// 右移動の処理
		if (onGround && (Input::IsKey(DIK_D) || LeftStick.x >= 0.3f) && MoveDirection == NONE)
		{
			isMove_interpolation = true;
			MoveDirection = RIGHT;
		}
		else if (!onGround && (Input::IsKey(DIK_D) || LeftStick.x >= 0.3f) && MoveDirection == NONE)
		{
			PlayerMove(BaseMove, NextPosition, 1.0f, 0.0f, 0.0f);
		}

		// 奥移動の処理
		if (onGround && (Input::IsKey(DIK_W) || LeftStick.y >= 0.3f) && MoveDirection == NONE)
		{
			isMove_interpolation = true;
			MoveDirection = BACKWARD;

		}
		else if (!onGround && (Input::IsKey(DIK_W) || LeftStick.y >= 0.3f) && MoveDirection == NONE)
		{
			PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, 1.0f);
		}

		// 手前移動の処理
		if (onGround && (Input::IsKey(DIK_S) || LeftStick.y <= -0.3f) && MoveDirection == NONE)
		{
			isMove_interpolation = true;
			MoveDirection = FORWARD;

		}
		else if (!onGround && (Input::IsKey(DIK_S) || LeftStick.y <= -0.3f) && MoveDirection == NONE)
		{
			PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, -1.0f);


		}

		// ブロック生成
		if (Input::IsKeyDown(DIK_L) || Input::IsPadButton(XINPUT_GAMEPAD_B) && !isBlockCanOnly)
		{
			PlayerBlockInstans();
		}
	}




// 重力処理（落下）
Jump_Power -= GRAVITY;
transform_.position_.y += Jump_Power;

// StageGridCorrection();

}

void Player::PlayerMove(XMVECTOR BaseMove, XMVECTOR NextPos, float x, float y, float z)
{
	// カメラを取得
	CameraController* pCamera = (CameraController*)FindObject("CameraController");

	//ステージ(モデル情報)を取得用
	Stage* pStage = (Stage*)FindObject("Stage");

	XMVECTOR move = XMVectorZero();

	// カメラの回転行列を取得
	XMMATRIX cameraRotation = pCamera->GetRotationMatrix();



	BaseMove = XMVectorSet(x, y, z, 0.0f); //最初に指定するべくベクトル

	//move = BaseMove;
	move = XMVector3TransformCoord(BaseMove, cameraRotation); // カメラ基準で補正

	bool isMoving = false;

	int StageModel = pStage->GetStageModel();

	if (onGround)
	{
		// 地上移動処理
		BaseMove = XMVectorSet(
			transform_.position_.x + XMVectorGetX(move) * MOVE_SPEED,
			transform_.position_.y + 0.01f,
			transform_.position_.z + XMVectorGetZ(move) * MOVE_SPEED,
			0.0f
		);
		if (!IsBlocked(BaseMove))
		{
			transform_.position_.x += XMVectorGetX(move) * MOVE_SPEED;
			transform_.position_.z += XMVectorGetZ(move) * MOVE_SPEED;
			MoveTimer_ = MAX_MOVE_FRAME;
			MoveAnimationTimer_ = AnimaFrame::MOVE_ANIMATION_FRAME;
			SetPlayerAnimation(1);
			isMoving = true;
		}

	}
	else
	{
		// 空中移動処理
		move = XMVector3TransformCoord(BaseMove, cameraRotation);
		transform_.position_.x += XMVectorGetX(move) * MOVE_AERIAL;
		transform_.position_.z += XMVectorGetZ(move) * MOVE_AERIAL;

		NextPos = XMVectorSet(
			transform_.position_.x + XMVectorGetX(move) * MOVE_AERIAL,
			transform_.position_.y,
			transform_.position_.z + XMVectorGetZ(move) * MOVE_AERIAL,
			0.0f
		);

		if (IsBlocked(NextPos))
		{
			transform_.position_.x -= XMVectorGetX(move) * MOVE_AERIAL;
			transform_.position_.z -= XMVectorGetZ(move) * MOVE_AERIAL;
		}
	}

	// プレイヤーが移動した方向に向く処理
	if (!XMVector3Equal(move, XMVectorZero()))
	{
		XMVECTOR pos = XMLoadFloat3(&(transform_.position_));

		XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(XM_PIDIV2));
		XMVECTOR modifiedVec = XMVector3TransformNormal(move, rot);

		float angle = atan2(XMVectorGetX(move), XMVectorGetZ(move));

		transform_.rotate_.y = XMConvertToDegrees(angle);

		XMStoreFloat3(&(transform_.position_), pos);

		// 反転
		angle += XM_PI;

		//  計算した角度を度に変換して反映
		transform_.rotate_.y = XMConvertToDegrees(angle);
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
	XMVECTOR FrontDirection = XMVectorSet(0, 0.45, -1, 0);

	if (Input::IsKey(DIK_UP) || Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		FrontDirection = XMVectorSet(0, 1.5, -1, 0);
	}

	if (Input::IsKey(DIK_DOWN) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		FrontDirection = XMVectorSet(0, -0.5, -1, 0);
	}

	XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

	XMVECTOR blockPos = PlayerPos + FrontDirection * 1.0f;

	if (IsBlocked(blockPos))
	{
		Debug::Log("ブロック配置不可: ステージブロックと重なる", true);
		return;
	}

	float gridSize = 1.0f;
	XMVECTOR snappedBlockPos = XMVectorSet
	(
		round(XMVectorGetX(blockPos) / gridSize) * gridSize,
		round(XMVectorGetY(blockPos) / 0.5) * 0.5,
		round(XMVectorGetZ(blockPos) / gridSize) * gridSize,
		0
	);

	PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
	XMStoreFloat3(&(block->GetPosition()), snappedBlockPos);

	SetPlayerAnimation(2);
	MoveAnimationTimer_ = AnimaFrame::SETTING_ANIMATION_FRAME;
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

void Player::OnCollision(GameObject* parent)
{
	PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");
	KeyFlag* pKey = (KeyFlag*)FindObject("KeyFlag");
	MoveEnemy* pEnemy = (MoveEnemy*)FindObject("MoveEnemy");
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Residue* pResidue = (Residue*)FindObject("Residue");


	if (parent->GetObjectName() == "PlayerBlock")
	{
		XMVECTOR blockPos = XMLoadFloat3(&(pBlock->GetPosition()));
		float blockY = XMVectorGetY(blockPos);

		if (transform_.position_.y <= blockY)
		{
		
			switch (MoveDirection)
			{
			case LEFT:
				pBlock->SetMoveLeft(true); 
				break;
			case RIGHT:
				pBlock->SetMoveRight(true);
				break;
			case FORWARD:
				pBlock->SetMoveForward(true);
				break;
			case BACKWARD:
				pBlock->SetMoveBackwaed(true);
				break;
			default:
				NONE;
				break;
			}

			//SetPlayerAnimation(3);
			MoveAnimationTimer_ = AnimaFrame::ATTACK_ANIMATION_FRAME;
		}


		//playerがブロックの上にいたらそこに乗る
		if (transform_.position_.y > pBlock->GetPosition().y)
		{
			transform_.position_.y = pBlock->GetPosition().y + 0.46;

			Jump_Power = 0.0f;

			onGround = true;
			onMyBlock = true;
		}

		MoveDirection = NONE;
	}

	if (parent->GetObjectName() == "KeyFlag")
	{
		ClearFlag_ = true;
	}


	if (parent->GetObjectName() == "MoveEnemy" && pResidue != nullptr)
	{
		isHitEnemy_ = true;
		player_state = DEAD;
	}

	if (parent->GetObjectName() == "GoalDoor")
	{
		openGoal_ = true;

		if (openGoal_ && ClearFlag_)
		{
			player_state = CLEAR;
		}
	}


	ResidueItem* pResidueItem = (ResidueItem*)FindObject("ResidueItem");
	if (parent->GetObjectName() == "ResidueItem" && pResidue != nullptr)
	{
		GetRubyflag = true;

		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

		if (pSceneManager != nullptr)
		{
			int currentResidue = pSceneManager->GetPlayerResidue();

			pSceneManager->SetPlayerResidue(currentResidue + 1);
		}

		pResidueItem->KillMe();
	}
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

			DeadAnimationTimer_ = AnimaFrame::DAMAGE_ANIMATION_FRAME;//最後に元のフレームに戻してあげる
		}
	}
}


void Player::StageHeight()
{
	Stage* pstage = (Stage*)FindObject("Stage");
	PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");
	if (pstage != nullptr)
	{
		float gridSize = 1.0f;
		float snappedX = round(transform_.position_.x / gridSize);
		float snappedZ = round(transform_.position_.z / gridSize);

		float GroundHeight = pstage->GetGroundHeight(snappedX, snappedZ);

		if (transform_.position_.y <= GroundHeight) {
			transform_.position_.y = GroundHeight;
			onGround = true;
			Jump_Power = 0.0f;
		}
		else {
			onGround = false;
		}
	}
}

bool Player::IsBlocked(XMVECTOR Position)
{
	Stage* stage = (Stage*)FindObject("Stage");
	if (stage)
	{
		float X = floor(XMVectorGetX(Position)+1.0);
		float Z = floor(XMVectorGetZ(Position)+1.0);

		int StageWidth = stage->GetWidth();
		int StageHeight = stage->GetHeight();

		if (X >= 0 && X < StageWidth && Z >= 0 && Z < StageHeight)
		{
			float blockHeight = stage->GetBlockHeight(X, Z);
			float playerHeight = XMVectorGetY(Position);

			if (blockHeight >= playerHeight)
			{
				Debug::Log("Y座標判定: ブロックに接触", true);
				return true;
			}
		}
		return false;
	}
}

void Player::PlayerRange()
{
	if (transform_.position_.x < 0)
	{
		transform_.position_.x = 0;
	}
	if (transform_.position_.x > MAX_RANGE)
	{
		transform_.position_.x = MAX_RANGE;
	}
	if (transform_.position_.z < 0)
	{
		transform_.position_.z = 0;
	}
	if (transform_.position_.z > MAX_RANGE)
	{
		transform_.position_.z = MAX_RANGE;
	}
}

void Player::Jump()
{
	Jump_Power = sqrtf(2 * GRAVITY * JUMP_HEIGHT);
	onGround = false;
	onMyBlock = false;
	MoveDirection = NONE;


	SetPlayerAnimation(4);
	MoveAnimationTimer_ = AnimaFrame::JUMP_ANIMATION_FRAME;

	if (onGround)
	{
		SetPlayerAnimation(0);
	}

}

void Player::PlayerGridCorrection()
{
	// 地上でのグリッド位置補正
	if (onGround && !onMyBlock)
	{
		float gridSize = 1.0f;
		float x = round((transform_.position_.x) / gridSize) * gridSize;
		float y = round((transform_.position_.y) / gridSize) * gridSize;
		float z = round((transform_.position_.z) / gridSize) * gridSize;
		transform_.position_.x = x;
		transform_.position_.y = y;
		transform_.position_.z = z;
	}
}

