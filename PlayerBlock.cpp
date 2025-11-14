#include "PlayerBlock.h"
#include "Engine/Model.h"
#include "Engine/Collider.h"
#include "Engine/Debug.h"
#include "Engine/Input.h"
#include "Player.h"
#include "Engine/VFX.h"
#include "Engine/Audio.h"
#include "TurretEnemy.h"
#include "Ghost.h"
#include "SettingVFX.h"

namespace
{
    constexpr float INITIAL_SCALE  = 0.1f;
    constexpr float FINAL_SCALE    = 1.0f;
    constexpr float SCALE_STEP     = 0.02f;
    constexpr float ROTATE_SPEED_Y = 2.0f;
    constexpr float COLLIDER_SIZE  = 1.0f;
}

PlayerBlock::PlayerBlock(GameObject* parent) : GameObject(parent, "PlayerBlock")
{
    hPlayerBlockModel_ = Model::Load("Model//BoxBrick.fbx");
    assert(hPlayerBlockModel_ >= 0);

	// プレイヤーブロックのSEを登録
    
	//ほかのオブジェクトと接触したときの反射音
	hPlayerBlockSE_[PLAYER_BLOCK_SE_REFLECT] = Audio::Load(PlayerBlockPath + "Reflect.wav");
	assert(hPlayerBlockSE_[PLAYER_BLOCK_SE_REFLECT] >= 0);
}

PlayerBlock::~PlayerBlock(){}

void PlayerBlock::Initialize()
{
    transform_.scale_ = { INITIAL_SCALE, INITIAL_SCALE, INITIAL_SCALE };
    isAnimation_ = true;

    BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { COLLIDER_SIZE, COLLIDER_SIZE, COLLIDER_SIZE });
    AddCollider(collision);
}

void PlayerBlock::Update()
{
    Player* pPlayer = static_cast<Player*>(FindObject("Player"));
    if (pPlayer)
    {
        AnimateBlock();
    }
}

void PlayerBlock::AnimateBlock()
{
    // 回転
    transform_.rotate_.y += ROTATE_SPEED_Y;

    // 拡大
    transform_.scale_.x += SCALE_STEP;
    transform_.scale_.y += SCALE_STEP;
    transform_.scale_.z += SCALE_STEP;


    // 一定サイズに達したら固定＆回転停止
    if (transform_.scale_.x > FINAL_SCALE)
    {
        transform_.scale_ = { FINAL_SCALE, FINAL_SCALE, FINAL_SCALE };
        transform_.rotate_.y = 0.0f;
        isAnimation_ = false; // アニメーション終了
    }


}

void PlayerBlock::Draw()
{
    Model::SetTransform(hPlayerBlockModel_, transform_);
    Model::Draw(hPlayerBlockModel_);

 
}

void PlayerBlock::Release() {}

void PlayerBlock::OnCollision(GameObject* parent) {

	TurretEnemy* pTurretEnemy = (TurretEnemy*)FindObject("TurretEnemy");
	Ghost* pGhost = (Ghost*)FindObject("Ghost");

	XMFLOAT2 VFXSize = { 1.5,1.5 };
	float  VFXLifeTime = 10.0f;
	float  VFXSpeed = 0.1f;
	int VFXNumber = 1;
	float VFXGravity = -0.002f;


	if (pTurretEnemy != nullptr)
	{
		// TurretEnemyに接触した場合、PlayerBlockを削除
		if (parent->GetObjectName() == "TurretEnemy")
		{

            SettingVFX::SetVFX(SettingVFX::VFX_FLASH_R, transform_.position_, { 1.0f, 1.0f }, { 0,1,0 });

			Audio::Play(hPlayerBlockSE_[PLAYER_BLOCK_SE_REFLECT]);
			pTurretEnemy->KillMe();

            
		}
	}
    
    if (pGhost != nullptr)
    {
        //Ghostに接触
        if (parent->GetObjectName() == "Ghost")
        {

            SettingVFX::SetVFX(SettingVFX::VFX_FLASH_B, transform_.position_, { 1.0f, 1.0f }, { 0,1,0 });

            Audio::Play(hPlayerBlockSE_[PLAYER_BLOCK_SE_REFLECT]);
            pGhost->GetReflectPosition(); // Ghostの位置を反射位置に更新
        }
    }
  
}
