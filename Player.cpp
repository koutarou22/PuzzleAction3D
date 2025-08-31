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
	const float MOVE_INTERPOLATION_SPEED = 0.07f;  // 補間速度
	const float MOVE_RATIO_INITIAL = 0.001f;// 補間開始時の値
	const float PLAYER_GRAVITY = 0.005f;// プレイヤーの重力
	const float JUMP_PARABOLA_COEFF = 5.0f;// 放物線の係数
	float JUMP_HEIGHT = 1.0f;

	// モサイズ,位置・コライダー
	const float INITIAL_PLAYER_SCALE = 0.5f;// プレイヤーの初期サイズ
	const float INITIAL_PLAYER_X = 1.0f;// プレイヤーの初期X座標
	const float INITIAL_PLAYER_Y = 1.0f;// プレイヤーの初期Y座標
	const float INITIAL_PLAYER_Z = 0.0f;// プレイヤーの初期Z座標
	const float COLLIDER_OFFSET_Y = 0.5f;// プレイヤーのコライダーのYオフセット
	const float COLLIDER_SIZE = 1.0f;// プレイヤーのコライダーのサイズ

	// グリッド座標変換
	const int STAGE_GRID_WIDTH = 10;// ステージのグリッド幅
	const int STAGE_GRID_HEIGHT = 10;// ステージのグリッド高さ
	const int STAGE_HEIGHT_MAX = 10;// ステージの最大高さ
	const int STAGE_OFFSET_X = 5;// ステージのXオフセット
	const int STAGE_OFFSET_Z = 4;// ステージのZオフセット

	// プレイヤー補正やスナップ
	const float GRID_UNIT = 1.0f;// グリッド
	const float GRID_Y_UNIT = 0.5f;// グリッドY
	const float ROTATION_OFFSET_DEGREES = 180.0f;// プレイヤーの回転
	const float BLOCK_PLACE_VERTICAL_OFFSET = 0.5f; // ブロック配置の垂直オフセット
	const float GRID_VERTICAL_UNIT = 0.5f;       


	// ブロック配置
	const float BLOCK_PLACE_DISTANCE = 1.0f;// ブロック配置の距離
	const float BLOCK_PLACE_HEIGHT_OFFSET = 0.5f;// ブロック配置の高さ

	// ブロックID（判定用）
	const int STAGE_BLOCK_EMPTY = 0;// 空ブロック
	const int STAGE_BLOCK_GHOST = 1;// ゴースト
	const int STAGE_BLOCK_TURRET = 2;// タレット
	const int STAGE_BLOCK_KEY = 3;// 鍵
	const int STAGE_BLOCK_DOOR = 4;// ドア
	const int STAGE_BLOCK_GROUND = 5;// 地面
	const int STAGE_BLOCK_Remain = 6;// 残機回復アイテム
	const int STAGE_BLOCK_PLAYER_BLOCK = 7;// プレイヤーブロック

	const float PLAYER_ROTATE_OFFSET_DEG = 180;//

	const float PARABOLA_COEFFICIENT = 5.0f;// ジャンプの放物線係数
}


namespace PLAYER_ANIME_FRAME
{
	const int IDOL_ANIMATION_FRAME = 120;  //待機フレーム
	const int MOVE_ANIMATION_FRAME = 30;   //移動フレーム
	const int SETTING_ANIMATION_FRAME = 80;//Block設置時フレーム
	const int ATTACK_ANIMATION_FRAME = 129;//Block攻撃時フレーム

	const int JUMP_ANIMATION_FRAME =30;   //ジャンプフレーム
	const int DAMAGE_ANIMATION_FRAME = 104;//やられフレーム
	const int VICTORY_ANIMATION_FRAME = 110;//ゴールフレーム
	const int FALL_ANIMATION_FRAME = 32;   //落下中フレーム
	const int LANDING_ANIMATION_FRAME = 35;//着地フレーム
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

	//初期は待機アニメーション
	SetPlayerAnimation(ANIM_IDLE);


	//サウンドの初期化
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


	//サウンドの登録

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

	//サウンドの登録確認
	assert(SoundPlayerSE_[PLAYER_SE_WALK] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_JUMP] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_LANDING] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_SETTING] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_GETITEM] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_DEAD] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_CLEAR] >= 0);
	assert(SoundPlayerSE_[PLAYER_SE_DONT_CLEAR] >= 0);


	//初期アニメーションをセット
	hPlayerModel_ = hPlayerAnimeModel_[ANIM_IDLE];

	//残機情報を持ってくる
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	if (pSceneManager != nullptr)
	{
		Player_Remain = pSceneManager->GetPlayerRemain();
	}
}

/// <summary>
/// 移動可能かどうかを判定する
/// </summary>
/// <param name="pos"></param>
/// <returns></returns>
MOVE_METHOD Player::CanMoveTo(const XMFLOAT3& pos)
{
	
	int gx = static_cast<int>(roundf(pos.x + STAGE_OFFSET_X));
	int gy = static_cast<int>(roundf(STAGE_OFFSET_Z - pos.z));
	int gz = static_cast<int>(roundf(pos.y));

	//画面外にはいけないようにする処理
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

	if (current == STAGE_BLOCK_EMPTY)
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
		//以下のオブジェクトはすり抜け可能
		current == STAGE_BLOCK_GHOST   || //ゴースト
		current == STAGE_BLOCK_KEY     || //鍵
		current == STAGE_BLOCK_DOOR    || //ドア
		current == STAGE_BLOCK_Remain)   //残機回復アイテム
	{
		Debug::Log("対象のオブジェクトは移動可能", true);

		return CAN_MOVE_WALK;
	}

	//目の前のブロックが自分の出したブロックなら
	if (current == STAGE_BLOCK_PLAYER_BLOCK)
	{
		Debug::Log("自分の出したブロックにジャンプ可能", true);
		return CAN_MOVE_JUMP_MY_BLOCK;
	}


	//もし目の前のブロックがあり、その上が空いてるならジャンプ可能 
	if (gz + 1 < STAGE_HEIGHT_MAX)
	{
		int above = grid[gz + 1][STAGE_GRID_HEIGHT - 1 - gy][gx];
		if (above == STAGE_BLOCK_EMPTY   ||
			above == STAGE_BLOCK_GHOST   ||
			above == STAGE_BLOCK_KEY     ||
			above == STAGE_BLOCK_DOOR    || 
			above == STAGE_BLOCK_Remain)
		{
			Debug::Log("ジャンプ可能", true);
			return CAN_MOVE_JUMP;
		}
	}
	else if (gz + 2 < STAGE_HEIGHT_MAX)//その上も空いていなかったらジャンプは不可能
	{
		int twoabove = grid[gz + 2][STAGE_GRID_HEIGHT - 1 - gy][gx];
		if (twoabove != STAGE_BLOCK_EMPTY && current != STAGE_BLOCK_EMPTY)
		{
			Debug::Log("ニマス以上を超えています");
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

	//どこまで下に探すかを指定
	int yEnd = yStart - STAGE_GRID_HEIGHT;

	//グリッドの範囲外(0)に出ないようにする
	if (yEnd < 0)
	{
		yEnd = 0;
	}


	for (int y = yStart; y >= yEnd; --y)
	{
		int current = grid[y][STAGE_GRID_HEIGHT - 1 - gy][gx];

		//以下のブロックは地面として扱う(ジャンプ可能)
		if (current == STAGE_BLOCK_GROUND ||    //地面
			current == STAGE_BLOCK_TURRET ||    //敵タレット
			current == STAGE_BLOCK_PLAYER_BLOCK)//自分のブロック
		{
			GROUND = y + 1;
			return;
		}
	}
}


XMFLOAT3 Player::GetInputDirection()
{
	PlayerBlock* pPlayerBlock = (PlayerBlock*)FindObject("PlayerBlock");

	//移動方向を初期化
	dir_ = { 0, 0, 0 };

	//左スティックの情報を取得
	XMFLOAT3 LeftStick = Input::GetPadStickL(0);

	//移動(奥)
	if      (Input::IsKey(DIK_W) || LeftStick.y >= STICK_DEADZONE)  dir_.z += MOVE_GRID;
	//移動(手前)
	else if (Input::IsKey(DIK_S) || LeftStick.y <= -STICK_DEADZONE) dir_.z -= MOVE_GRID;
	//移動(左)
	else if (Input::IsKey(DIK_A) || LeftStick.x <= -STICK_DEADZONE) dir_.x -= MOVE_GRID;
	//移動(右)
	else if (Input::IsKey(DIK_D) || LeftStick.x >= STICK_DEADZONE)  dir_.x += MOVE_GRID;

	//移動中・ジャンプ中 そして自分のブロック上ではない場合
	if (!IsWalkInterpolation && !IsJumpInterpolation && !onMyBlock_)
	{
		//ブロックを連続で設置できないようにする処理
		if (pPlayerBlock == nullptr || !pPlayerBlock->GetIsAnimation())
		{
			//ブロック設置
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
	//死亡アニメーションを更新
	DeadAnimation();
}

void Player::UpdateClear()
{
	//クリアアニメーションを更新
	ClearAnimation();
}

void Player::DeadAnimation()
{
	SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
	Remain* pRemain = (Remain*)FindObject("Remain");

	//isHitEnemy_がtrueならアニメーションを再生
	if (isHitEnemy_ == true)
	{
		SetPlayerAnimation(5);
	}

	animationDeadTimer_--;


	Audio::Play(SoundPlayerSE_[PLAYER_SE_DEAD]);

	if (animationDeadTimer_ <= 0)
	{
		isHitEnemy_ = false;

		// 残機を減らす
		int currentRemain = pSceneManager->GetPlayerRemain();
		if (currentRemain > 0)
		{
			pSceneManager->SetPlayerRemain(currentRemain - 1); // 残機を1減らす
		}

		//残機が1以上なら LoadScene に戻す
		if (pSceneManager->GetPlayerRemain() > 0)
		{
			pSceneManager->ChangeScene(SCENE_ID_LOAD);
		}
		else
		{
			//Sceneが切り変わる前に確実に残機をリセット
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
			// ステージを次に進める(値を+1する)
			pSceneManager->NextStageCountPlus();

			// 今のステージ番号を取得
			int currentStage = pSceneManager->GetStageNumber();

			if (currentStage > MaxStage)
			{
				// ステージが最大値を超えたらクリアシーンへ 
				pSceneManager->ChangeScene(SCENE_ID_CLEAR);

			}
			else
			{
				// 違うなら通常のロードシーンへ
				pSceneManager->ChangeScene(SCENE_ID_LOAD);
			}
		}
	}

}


MOVE_METHOD Player::PlayerBlockInstans()
{
	// 既存ブロックがあれば削除
	PlayerBlock* existingBlock = (PlayerBlock*)FindObject("PlayerBlock");
	if (existingBlock != nullptr)
	{
		//古いブロック(前)の位置を取得
		XMFLOAT3& oldPos = existingBlock->GetPosition();
		int oldGx = static_cast<int>(oldPos.x + STAGE_OFFSET_X);
		int oldGy = static_cast<int>(STAGE_OFFSET_Z - oldPos.z);
		int oldGz = static_cast<int>(oldPos.y);

		auto* stage = static_cast<Stage*>(FindObject("Stage"));
		if (stage)
		{
			auto& grid = stage->GetStageGrid();
			//前のブロックは削除(0)する
			grid[oldGz][STAGE_GRID_HEIGHT - 1 - oldGy][oldGx] = STAGE_BLOCK_EMPTY;//0に戻す
			StandingStage(transform_.position_);
		}

		existingBlock->KillMe();
	}

	// プレイヤーの位置と方向設定

	// プレイヤーの位置を取得
	XMVECTOR PlayerPos = XMLoadFloat3(&(transform_.position_));

	// プレイヤーの前方向を指定
	XMVECTOR FrontDirection = XMVectorSet(0.0f, BLOCK_PLACE_VERTICAL_OFFSET, -1.0f, 0.0f);

	// プレイヤーの回転を考慮して前方向を回転
	XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));

	// 前方向を回転させる
	FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

	// ブロックを置く位置を計算
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
	if (gx < 0 || gx >= STAGE_GRID_WIDTH  ||
		gy < 0 || gy >= STAGE_GRID_HEIGHT ||
		gz < 0 || gz >= STAGE_HEIGHT_MAX)
	{
		return CANT_MOVE;
	}

	PlayerBlock* pPlayerBlock = (PlayerBlock*)FindObject("PlayerBlock");


	//目の前にブロックがあるかチェック+アニメーションが終わっているか確認
	int cellValue = grid[gz][STAGE_GRID_HEIGHT - 1 - gy][gx];
	if (cellValue != STAGE_BLOCK_EMPTY && cellValue != STAGE_BLOCK_PLAYER_BLOCK &&
		cellValue != STAGE_BLOCK_GHOST && cellValue != STAGE_BLOCK_TURRET)
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

	//ここでブロック7を登録する
	grid[gz][STAGE_GRID_HEIGHT - 1 - gy][gx] = STAGE_BLOCK_PLAYER_BLOCK;

	//ブロックの設置音
	Audio::Play(SoundPlayerSE_[PLAYER_SE_SETTING]);

	return CANT_MOVE;
}

// プレイヤーの位置をグリッドに合わせて補正する
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
	//落下中か
	if (isFalling)
	{
		//重力を適用
		velocity.y -= PLAYER_GRAVITY;
		transform_.position_.y += velocity.y;

		//落下中のアニメーション
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

			//着地したら地面に合わせる
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

		//放物線補間
		float offsetY =
			PARABOLA_COEFFICIENT * // 放物線の係数
			JUMP_HEIGHT *          // ジャンプの最大高
			moveRatio *            // 開始 → 終了進行度（0.0~1.0）
			(1.0f - moveRatio);    // 進行度に対して対称な減少


		// 位置を更新
		StandingStage(transform_.position_);
		transform_.position_ = { horizontal.x, prepos.y + offsetY, horizontal.z };


		if (moveRatio >= 1.0f)
		{
			//ジャンプが終わったら
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
		isJumping = true;// ジャンプ中フラグを立てる
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
	//落下中じゃなければ通常の移動処理を行う
	if (!isFalling)
	{
		PlayerMoveMent();
		JumpParabola();
	}
	else if (!IsWalkInterpolation)
	{
	
		PlayerFallDown();	//落下の処理
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

	//カメラが回転中、またはジャンプ補間中は移動出来ない
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

			// 入力方向をカメラの回転に合わせて変換
			XMVECTOR inputVec = XMLoadFloat3(&inputDir);

			// カメラの回転行列を使って入力ベクトルを回転
			XMVECTOR rotatedVec = XMVector3TransformCoord(inputVec, camRotMatrix);

			// 回転後のベクトルをXMFLOAT3に変換
			XMStoreFloat3(&nextpos, rotatedVec);

			XMFLOAT3 target = AddXMFLOAT3(transform_.position_, nextpos);

			//移動可能かどうかを判定
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

			//補間が終わったら
			if (!IsWalkInterpolation)
			{
				//ズレ防止で補正する
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

	//プレイヤーの向きを更新
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

			// 残機が最大値に達していない場合、残機を増やす
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


