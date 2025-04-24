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
    //０待機モーション
    //１移動モーション
    //２設置モーション
    //３攻撃モーション
    //４ジャンプモーション
    //５やられモーション
    //６勝利モーション
    int hPlayerAnimeModel_[7];
    int moveAnimationTimer_ = 0; // アニメーション持続用タイマー
    int victoryAnimationTimer_ = 0;//クリア
   


    int currentFrame_ = 0; // 現在のアニメーションフレーム
    int maxFrame_ = 0;     // アニメーションの最大フレーム
    bool isAnimationFinished = false; // アニメーションが終了したかどうか
    int currentAnimation_ = 0;        // 現在のアニメーションタイプ

    bool onGround;//地面についているか確認用
    bool onMyBlock;//自分で出したブロックの上に乗っているか
    bool prevSpaceKey;//スペースキーが押されたか確認用
    bool ClearFlag_;//鍵に接触時にクリア条件を得る用
    bool openGoal_;//Goalに接触したとき

    bool GetRubyflag;//Rubyを入手したか判定用

    float MoveFlagTimer_;
    float MoveTimer_;//押し続けたら移動

    bool PressKey_;

    float Jump_Power;//ジャンプ力

    float posX, posY, posZ;//初期位置


    bool isHitEnemy_;//敵と接触してしまったFlag

    //カメラが動いてるかの処理
    bool isMoveCamera_;

    /// <summary>
    /// 移動処理してる時補間したい!....用の変数たち
    /// </summary>
    int isMove_;//一時的な移動フレームを格納する判定変数
    bool isMove_interpolation;//入力が入れば減少し続ける処理
    bool isGoMove;//0になったら動いていいよ！の変数

    bool GoLeft;

    float progress;

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

    void PlayerMove(XMVECTOR BaseMove , XMVECTOR NextPos, float x, float y , float z );

    

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
    float GetRayWide() const { return 1.0f; }

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

    //名前は後で直すこと
    XMFLOAT3 GetRayStartX() const
    {
        XMFLOAT3 rayStart = transform_.position_;
        rayStart.x += GetRayWide();
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
    void SetHitEnmeyFlag(bool isHIt) { isHitEnemy_ == isHIt; }

    void SetHitGoalFlag(bool isGoal) { ClearFlag_ == isGoal; }
    bool GetClearFlag() { return ClearFlag_; }

    bool GetScoreFlag() { return GetRubyflag; }

    void SetJumpPower(float Jump) { Jump_Power = Jump; }



    //カメラが動かしているときプレイヤーは動くことが出来ない処理
    void SetMoveCamera(bool MoveCamera) { isMoveCamera_ = MoveCamera; }
    bool GetMoveCamera() { return isMoveCamera_; }


    /// <summary>
    /// プレイヤーが地面に着く・または行動終了後にマス目の中央に補正する処理
    /// </summary>
    void PlayerGridCorrection();

    int GetPlayerModel() { return hPlayerModel_; }


    ////四方向にレイを飛ばす
    //void CheckHitForDirectionRayCast();


    void Animation();

};