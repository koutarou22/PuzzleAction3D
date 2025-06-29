#pragma once
#include "Engine/GameObject.h"

//移動を判定する処理
enum MOVE_METHOD
{
	CAN_MOVE_WALK,   //移動可
	CAN_MOVE_JUMP,   //ジャンプ可
	CANT_MOVE,       //移動不可
	CANT_JUMP,       //ジャンプ不可
	CAN_MOVE_FALL,   //落下可能
	MAX_MOVE_METHOD, //例外
};

//プレイヤーの状態
enum PLAYER_STATE
{
	MOVE, JUMP, FALL, DEAD, CLEAR
};

class Player : public GameObject
{


	//入力に関する処理
	//入力可能な範囲
	const float STICK_DEADZONE = 0.5f;
	//移動距離
	const float MOVE_GRID = 1.0f;

	/// <summary>
	/// 移動可能か判定する処理
	/// </summary>
	/// <param name="pos">位置</param>
	/// <returns></returns>
	MOVE_METHOD CanMoveTo(const XMFLOAT3& pos);

	/// <summary>
	/// ステージの上に乗れる処理
	/// </summary>
	/// <param name="pos"></param>
	void StandingStage(const XMFLOAT3& pos);


	/// <summary>
	///入力処理
	/// </summary>
	/// <returns></returns>
	XMFLOAT3 GetInputDirection();

	XMFLOAT3 velocity = { 0, 0, 0 };

	bool IsJumpInterpolation; // ジャンプ補間中フラグ
	bool IsWalkInterpolation; // 歩き補間中フラグ
	bool deferFall = false;   //落下に切り替わるのを遅らす用

	XMFLOAT3 dir_; // 入力方向用

	void PlayerMoveMent();
	void PlayerFallDown();

	void JumpParabola();
	void Jump(const XMFLOAT3& inputDir);

	PLAYER_STATE playerstate = PLAYER_STATE::MOVE;

	//-----------------モデル登録用--------------------------
	
	int hPlayerModel_;//アニメーションのモデルを格納する変数

	//０待機モーション
	//１移動モーション
	//２設置モーション
	//３攻撃モーション
	//４ジャンプモーション
	//５やられモーション
	//６勝利モーション
	//７落下中モーション
	//８落下モーション

	int hPlayerAnimeModel_[10];//アニメーションのモデル配列
	//-------------------------------------------------------

	// 共通タイマー
	int animationTimer_;      
	int currentAnimIndex_;    // 現在のアニメインデックス
	bool isAnimationLocked_;  // フレームが0になるまで他の動作を受け付けない場合用

	//特殊なアニメーションタイマー
	int animationDeadTimer_;//敵接触時のタイマー
	int animationVictoryTimer_;//クリア時のアニメーションタイマー
	int animationLandingTimer_;//着地時のアニメーション


	//プレイヤーの残機
	int Player_Residue;

	//カメラが動いているのか確認
	bool isMoveCamera_;

	//敵と接触してるのか判定用
	bool isHitEnemy_;

	//鍵に接触時にクリア条件を得る用
	bool ClearFlag_;

	//Goalに接触したとき
	bool openGoal_;

	bool GetRubyflag_;//Rubyを入手したか判定用
	void UpdateMove();
	void UpdateDead();
	void UpdateClear();

	void DeadAnimation();   //敵接触時のAnimation
	void ClearAnimation();  //ゴール時のAnimation

	MOVE_METHOD PlayerBlockInstans();
public:
	Player(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* parent) override;

	//Set・Get関数　影響が怖いので保留

	void SetClearFlag(bool ClearFlag) { ClearFlag_ = ClearFlag; }
	//void SetBlockAnimeEnd(bool EndAnimation) { isBlockCanOnly = EndAnimation; }
	void SetHitEnmeyFlag(bool isHIt) { isHitEnemy_ == isHIt; }

	void SetHitGoalFlag(bool isGoal) { ClearFlag_ == isGoal; }
	bool GetClearFlag() { return ClearFlag_; }
	//カメラが動かしているときプレイヤーは動くことが出来ない処理
	void SetMoveCamera(bool moveCamera) { isMoveCamera_ = moveCamera; }
	bool GetMoveCamera() { return isMoveCamera_; }
	int GetPlayerModel() { return hPlayerModel_; }



  //０待機モーション
  //１移動モーション
  //２設置モーション
  //３攻撃モーション
  //４ジャンプモーション
  //５やられモーション
  //６勝利モーション
  void SetPlayerAnimation(int AnimeType);


};