#include "MoveEnemy.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Player.h"
#include "PlayerBlock.h"

MoveEnemy::MoveEnemy(GameObject* parent) : GameObject(parent, "MoveEnemy")
{
    MoveEnemyDirection = 0.05f;
}

MoveEnemy::~MoveEnemy()
{
}

void MoveEnemy::Initialize()
{
	hModel_ = Model::Load("Ghostlow.fbx");
	assert(hModel_ >= 0);

	transform_.rotate_.y = -90.0f;
    //transform_.rotate_.x = 90.0f;
	
    transform_.position_ = { 5.0,4.0,5.0 };
    transform_.scale_ = { 1.5,1.5,1.5 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.5, 0.5, 0.5 });
	AddCollider(collision);
}

void MoveEnemy::Update()
{
    transform_.position_.x += MoveEnemyDirection;
  
    CanMoveRenge();
}

void MoveEnemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void MoveEnemy::Release()
{
}

void MoveEnemy::OnCollision(GameObject* parent)
{
	if (parent ->GetObjectName() == "Player")
    {
	  Player* pPlayer = (Player*)FindObject("Player");

      if (pPlayer != nullptr)
      {
          Debug::Log("エネミーとプレイヤーが接触した", true);
          pPlayer->SetHitEnmeyFlag(true);
      }
    }

    if (parent->GetObjectName() == "PlayerBlock")
    {
        PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");

        if (pBlock != nullptr)
        {
            bool MoveBlock = pBlock->GetMoveHit();
            if (MoveBlock)
            {
                KillMe();
            }
            else
            {
                MoveEnemyDirection = -MoveEnemyDirection;
                transform_.rotate_.y += 180.0f;
            }
        }

    }
}

void MoveEnemy::CanMoveRenge()
{
    // X軸の範囲チェック
    if (transform_.position_.x < 0)
    {
        transform_.position_.x = 0; 
        MoveEnemyDirection = -MoveEnemyDirection;
        transform_.rotate_.y += 180.0f; 
    }
    else if (transform_.position_.x > MAX_RANGE)
    {
        transform_.position_.x = MAX_RANGE; 
        MoveEnemyDirection = -MoveEnemyDirection; 
        transform_.rotate_.y += 180.0f; 
    }

   
    if (transform_.position_.z < 0)
    {
        transform_.position_.z = 0;
        MoveEnemyDirection = -MoveEnemyDirection; 
        transform_.rotate_.y += 180.0f;
    }
    else if (transform_.position_.z > MAX_RANGE)
    {
        transform_.position_.z = MAX_RANGE;
        MoveEnemyDirection = -MoveEnemyDirection;
        transform_.rotate_.y += 180.0f;
    }
}



