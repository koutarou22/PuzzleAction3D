#pragma once
#include "Engine/GameObject.h"
#include "Shadow.h"

#include <string>

using std::string;

// 移動を判定する処理
enum MOVE_METHOD
{
    CAN_MOVE_WALK,   // 移動可
    CAN_MOVE_JUMP,   // ジャンプ可
    CANT_MOVE,       // 移動不可
    CANT_JUMP,       // ジャンプ不可
    CAN_MOVE_FALL,   // 落下可能

	CAN_MOVE_JUMP_MY_BLOCK, // 自分のブロック上でジャンプ可能
	CANT_MOVE_MY_BLOCK,     // 自分のブロック上で移動不可

    MAX_MOVE_METHOD, // 例外
};

// プレイヤーの状態
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
    ANIM_IDLE = 0,     // ０：待機モーション
    ANIM_MOVE,         // １：移動モーション
    ANIM_SETTING,      // ２：設置モーション
    ANIM_ATTACK,       // ３：攻撃モーション
    ANIM_JUMP,         // ４：ジャンプモーション
    ANIM_DEAD,         // ５：やられモーション
    ANIM_VICTORY,      // ６：勝利モーション
    ANIM_FALL,         // ７：落下中モーション
    ANIM_LANDING,      // ８：着地モーション
    ANIM_MAX
};


//音関係
enum PLAYER_SOUND_SE
{
	PLAYER_SE_WALK = 0, //歩きSE
    PLAYER_SE_SETTING,  //設置SE
    PLAYER_SE_LANDING,  // 着地SE
    PLAYER_SE_JUMP,     //ジャンプSE
    PLAYER_SE_GETITEM,  //アイテム取得SE
    PLAYER_SE_DEAD,     // やられSE
    PLAYER_SE_CLEAR,    // クリアSE
    PLAYER_SE_MAX,
};


class Player : public GameObject
{
    // -------------------- 定数定義 --------------------
	static constexpr float STICK_DEADZONE = 0.5f;// スティックのデッドゾーン
	static constexpr float MOVE_GRID = 1.0f; // プレイヤーの移動
	static constexpr float DEFAULT_GROUND_HEIGHT = 1.0f;// 初期の地面の高さ
	static constexpr int PLAYER_ANIMATION_COUNT = 10;// アニメーションの数
    // --------------------------------------------------

    // 入力処理関係
	MOVE_METHOD CanMoveTo(const XMFLOAT3& pos);// 移動可能かどうかを判定する
	XMFLOAT3 GetInputDirection();// 入力方向を取得する

    XMFLOAT3 velocity = { 0, 0, 0 };
    XMFLOAT3 dir_;     // 入力方向用

    bool IsJumpInterpolation = false; // ジャンプ補間中フラグ
    bool IsWalkInterpolation = false; // 歩き補間中フラグ
    bool deferFall = false;           // 落下を一時抑制

    void PlayerMoveMent();
    void PlayerFallDown();

	//ジャンプの放物線を計算する関数
    void JumpParabola();

	//ジャンプの処理
    void Jump(const XMFLOAT3& inputDir);

    PLAYER_STATE playerstate = PLAYER_STATE::MOVE;

    // ---------------- アニメーション ----------------
    int hPlayerModel_;                                // 現在使用中のプレイヤーモデル
    int hPlayerAnimeModel_[PLAYER_ANIMATION_COUNT];   // アニメーション別モデル配列

    // アニメーション制御
	int animationTimer_;           // アニメーションのタイマー
	int currentAnimIndex_;         // 現在のアニメーション
    bool isAnimationLocked_;       // ロック中は別動作不可

    int animationDeadTimer_;       // 敵接触時
    int animationVictoryTimer_;    // クリア時
    int animationLandingTimer_;    // 着地時
    // ------------------------------------------------------

    // 地面情報・状態
    int GROUND = DEFAULT_GROUND_HEIGHT; //初期の地面の高さ
    int Player_Residue;  // 残機数

    // 状態管理 
    bool isMoveCamera_= false;  //カメラが動いているかどうか
	bool isHitEnemy_  = false;  //敵に当たったかどうか
	bool ClearFlag_   = false;  //鍵を取ったかどうか
    bool openGoal_    = false;  //ゴールが開いたかどうか
	bool onMyBlock_   = false;  //自分のブロック上にいるかどうか
	bool GetRubyflag_ = false;  //ルビー(残機回復アイテム)を取得したかどうか

    //サウンド
	int SoundPlayerSE_[PLAYER_SE_MAX]; //SE情報を格納する配列

    // 状態を更新する用関数
    void UpdateMove();
    void UpdateDead();
    void UpdateClear();

    void DeadAnimation();
    void ClearAnimation();

    //プレイヤーが出すブロックの情報がまとまった関数
    MOVE_METHOD PlayerBlockInstans();

    //プレイヤーの位置がずれないようにする処理
    void PlayerGridCorrection();

public:

    Player(GameObject* parent);

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    void OnCollision(GameObject* parent) override;

	//setter、getter関数
    void SetClearFlag(bool ClearFlag) { ClearFlag_ = ClearFlag; }
    void SetHitEnmeyFlag(bool isHIt) { isHitEnemy_ = isHIt; }
    void SetHitGoalFlag(bool isGoal) { ClearFlag_ = isGoal; }
    bool GetClearFlag() { return ClearFlag_; }

    int GetGroundHeight() { return GROUND; }


	// ステージの上に立っているかどうか
    void StandingStage(const XMFLOAT3& pos);

    // アニメーション設定（0:待機?8:落下）
    void SetPlayerAnimation(int AnimeType);
};