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
    PLAYER_SE_JUMP,     //ジャンプSE
    PLAYER_SE_LANDING,  // 着地SE
    PLAYER_SE_GETITEM,  //アイテム取得SE

    PLAYER_SE_DEAD,     // やられSE
    PLAYER_SE_CLEAR,    // クリアSE
    PLAYER_SE_MAX,
};


class Player : public GameObject
{
    // -------------------- 定数定義 --------------------
    static constexpr float STICK_DEADZONE = 0.5f;
    static constexpr float MOVE_GRID = 1.0f;
    static constexpr float DEFAULT_GROUND_HEIGHT = 1.0f;
    static constexpr int PLAYER_ANIMATION_COUNT = 10;
    // --------------------------------------------------

    //クリアしたかの確認
    int ClearCount = 0;

    // 入力処理関係
    MOVE_METHOD CanMoveTo(const XMFLOAT3& pos);
    XMFLOAT3 GetInputDirection();

    XMFLOAT3 velocity = { 0, 0, 0 };
    XMFLOAT3 dir_;     // 入力方向用

    bool IsJumpInterpolation = false; // ジャンプ補間中フラグ
    bool IsWalkInterpolation = false; // 歩き補間中フラグ
    bool deferFall = false;           // 落下を一時抑制

    void PlayerMoveMent();
    void PlayerFallDown();

    void JumpParabola();
    void Jump(const XMFLOAT3& inputDir);

    PLAYER_STATE playerstate = PLAYER_STATE::MOVE;

    // ---------------- アニメーション ----------------
    int hPlayerModel_;                                // 現在使用中のプレイヤーモデル
    int hPlayerAnimeModel_[PLAYER_ANIMATION_COUNT];   // アニメーション別モデル配列

    // アニメーション制御
    int animationTimer_;
    int currentAnimIndex_;
    bool isAnimationLocked_;       // ロック中は別動作不可

    int animationDeadTimer_;       // 敵接触時
    int animationVictoryTimer_;    // クリア時
    int animationLandingTimer_;    // 着地時
    // ------------------------------------------------------

    // 地面情報・状態
    int GROUND = static_cast<int>(DEFAULT_GROUND_HEIGHT); // 接地高さ
    int Player_Residue;  // 残機数



    // 状態管理 
    bool isMoveCamera_= false;  
    bool isHitEnemy_  = false;
    bool ClearFlag_   = false;
    bool openGoal_    = false;
    bool onMyBlock_   = false;
    bool GetRubyflag_ = false;

    //サウンド
    int SoundPlayerSE_[PLAYER_SE_MAX];

    // 処理ブロック
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

	// プレイヤーのクリア数を共有
	int GetClearCount() { return ClearCount; }

    // ステージに立つ処理
    void StandingStage(const XMFLOAT3& pos);

    // アニメーション設定（0:待機?8:落下）
    void SetPlayerAnimation(int AnimeType);
};