#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"
#include<string>

using std::string;

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

enum PLAYER_BLOCK_SE
{
	PLAYER_BLOCK_SE_REFLECT = 0, //反射音
	PLAYER_BLOCK_SE_MAX,
};

/// <summary>
/// プレイヤーが使用するブロック
/// </summary>
class PlayerBlock :public GameObject
{

	int hPlayerBlockModel_;//プレイヤーブロックのモデルハンドル

    void AnimateBlock();//簡単な生成Animation

	bool isAnimation_ = false; //アニメーション中かどうか

    int RefrectSoundHandle = -1;

	// プレイヤーブロックのSEハンドル
    int hPlayerBlockSE_[PLAYER_BLOCK_SE_MAX];
	// プレイヤーブロックのSEパス
	string PlayerBlockPath = "Sound//SE//PlayerBlockSE//";

    //VFXのサイズ
    XMFLOAT3 VfxDir = { 0,1,0 };
    XMFLOAT2 VfxScale = { 1.0f,1.0f };

   
public:

    PlayerBlock(GameObject* parent);

    ~PlayerBlock();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 &GetPosition() { return transform_.position_; }
    XMFLOAT3 &GetScale()    { return transform_.position_; }

    void OnCollision(GameObject* parent) override;

	bool GetIsAnimation() const { return isAnimation_; };
};

