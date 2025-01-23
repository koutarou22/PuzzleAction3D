#include "Enemy.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Player.h"

Enemy::Enemy(GameObject* parent)
{
    MoveEnemyDirection = 0.05f;
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("BoxSand.fbx");
	assert(hModel_ >= 0);

	transform_.rotate_.y = 90.0f;
    transform_.rotate_.x = 90.0f;
	
    transform_.position_ = { 5.0,4.0,5.0 };
    transform_.scale_ = { 0.5,0.5,0.5 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.5, 0.5, 0.5 });
	AddCollider(collision);
}

void Enemy::Update()
{
	transform_.rotate_.y += 4.0;
	transform_.rotate_.x += 4.0;
    transform_.position_.x += MoveEnemyDirection;
  
    CanMoveRenge();
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}

void Enemy::OnCollision(GameObject* parent)
{
	if (parent ->GetObjectName() == "Player")
    {
	  Player* pPlayer = (Player*)FindObject("Player");
	  Debug::Log("ブロックとプレイヤーが接触した", true);
	  pPlayer->KillMe(); 
    }
}

void Enemy::CanMoveRenge()
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


