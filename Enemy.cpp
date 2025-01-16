#include "Enemy.h"
#include "Engine/Model.h"

Enemy::Enemy(GameObject* parent)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("Enemy.fbx");
	assert(hModel_ >= 0);

	transform_.rotate_.y = 180.0f;
	transform_.position_.y = 1.0;
	transform_.position_.x = 5.0;
	//transform_.position_.z = 0.0;

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1, 1, 1 });
	AddCollider(collision);
}

void Enemy::Update()
{
	transform_.rotate_.y += 2.0;
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
}
