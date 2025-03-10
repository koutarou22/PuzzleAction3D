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

	transform_.rotate_.y = 180.0f;
    //transform_.rotate_.x = 90.0f;
	
    transform_.position_ = { 5.0,5.0,5.0 };
    transform_.scale_ = { 1.5,1.5,1.5 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.5, 0.5, 0.5 });
	AddCollider(collision);
}

void MoveEnemy::Update()
{
	//transform_.rotate_.y += 4.0;
	//transform_.rotate_.x += 4.0;
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
          Debug::Log("ブロックとプレイヤーが接触した", true);
          pPlayer->KillMe();
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
            }
        }

    }
}

void MoveEnemy::CanMoveRenge()
{
    if (transform_.position_.x < 0)
    {
        transform_.position_.x = 0;
        MoveEnemyDirection = -MoveEnemyDirection;
    }
    if (transform_.position_.x > MAX_RANGE)
    {
        transform_.position_.x = MAX_RANGE;
        MoveEnemyDirection = -MoveEnemyDirection;
    }
    if (transform_.position_.z < 0)
    {
        transform_.position_.z = 0;
    }
    if (transform_.position_.z > MAX_RANGE)
    {
        transform_.position_.z = MAX_RANGE;
    }
}


