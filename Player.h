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
    int hModel;
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

    void Jump();

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

};

