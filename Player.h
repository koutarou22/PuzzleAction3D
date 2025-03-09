#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"

class FBX;

/// <summary>
/// プレイヤーの情報クラス
/// </summary>
class Player :public GameObject
{

    enum Direction
    {
        NONE,
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD
    };

    Direction MoveDirection = NONE;

    bool isBlocked;//プレイヤーがステージにぶつかったら進めない判定
    bool isBlockCanOnly;//ブロックが一個出し切るまで再度設置できない判定
    FBX* pFbx;
    int hPlayerModel_;

    int hPlayerTestModel_;
    //１待機モーション
    //２移動モーション
    //３攻撃モーション
    int hPlayerAnimeModel_[4];

    int hWalk_;//歩くときのアニメーション
    int hJump_;//ジャンプ時のアニメーション
    int hAction_;//ブロックを置いた時のアニメーション

    bool onGround;//地面についているか確認用
    bool prevSpaceKey;//スペースキーが押されたか確認用
    bool ClearFlag_;//鍵に接触時にクリア条件を得る用
    float MoveTimer_;//押し続けたら移動

    bool PressKey_;

    float Jump_Power;//ジャンプ力

    float posX, posY, posZ;//初期位置
public:

    //コンストラクタ
    Player(GameObject* parent);
    //デストラクタ
    ~Player();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;

    /// <summary>
    /// Playerの動作
    /// </summary>
    void PlayerControl();
    /// <summary>
    /// Playerが行ける範囲を制限する
    /// </summary>
    void PlayerRange();

    

    /// <summary>
    /// Jumpの処理
    /// </summary>
    void Jump();

    /// <summary>
    /// プレイヤーの目の前にブロックを出現させる処理
    /// </summary>
    void PlayerBlockInstans();

    void SetPlayerAnimation(int AnimeType);
    

    XMFLOAT3 &GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    int GetModelHandle() { return hPlayerModel_; }

    float GetRayHeight() const { return 1.0f; }

    /// <summary>
    /// レイの開始位置
    /// </summary>
    /// <returns></returns>
    XMFLOAT3 GetRayStart() const 
    {
        XMFLOAT3 rayStart = transform_.position_; 
        rayStart.y += GetRayHeight();
        return rayStart;
    }

    void SetonGround(bool ground)
    {
        onGround = ground;
    }
    void OnCollision(GameObject* parent) override;


    void StageHeight();
    /// <summary>
    /// プレイヤーよりも高い壁があったら進めなくする条件
    /// </summary>
    /// <param name="newPosition"></param>
    /// <returns></returns>
    bool IsBlocked(XMVECTOR Position);

    void SetClearFlag(bool ClearFlag) { ClearFlag_ = ClearFlag; }
    void SetBlockAnimeEnd(bool EndAnimation) { isBlockCanOnly = EndAnimation; }
    bool GetClearFlag() { return ClearFlag_; }

};