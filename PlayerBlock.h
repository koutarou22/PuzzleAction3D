#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"

class FBX;

/// <summary>
/// プレイヤーが使用するブロック
/// </summary>
class PlayerBlock :public GameObject
{
    FBX* pFbx;
    int hModel_;
    bool onGround;//地面についているか確認用
    bool prevSpaceKey;//スペースキーが押されたか確認用

    float Jump_Power;//ジャンプ力
public:

    //コンストラクタ
    PlayerBlock(GameObject* parent);
    //デストラクタ
    ~PlayerBlock();

    //初期化
    void Initialize() override;

    //更新
    void Update() override;

    //描画
    void Draw() override;

    //開放
    void Release() override;



    
    XMFLOAT3 &GetTransform() { return transform_.position_; }

    void OnCollision(GameObject* parent) override;

};

