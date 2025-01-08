#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"

class FBX;

enum CameraType
{

};

/// <summary>
/// プレイヤーの情報クラス
/// </summary>
class Player :public GameObject
{
    FBX* pFbx;
    int hModel_;
    bool onGround;//地面についているか確認用
    bool prevSpaceKey;//スペースキーが押されたか確認用

    float Jump_Power;//ジャンプ力
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
    /// Playerの操作
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
    /// 地面との判定を取る(レイキャスト)
    /// </summary>
    void GroundCheck();

    /// <summary>
    /// プレイヤーがブロックを出現させる処理
    /// </summary>
    void PlayerBlockInstans();

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    int GetModelHandle() { return hModel_; }

    float GetRayHeight() const { return 1.0f; }

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
    bool IsBlocked(XMVECTOR newPosition);
};

