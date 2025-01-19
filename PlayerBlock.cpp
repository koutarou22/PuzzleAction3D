#include "PlayerBlock.h"
#include "Engine/Model.h"
#include"Engine/Collider.h"
#include "Engine/Debug.h"
#include "Player.h"

PlayerBlock::PlayerBlock(GameObject* parent) : GameObject(parent, "PlayerBlock")
{
	hModel_ = Model::Load("BoxWater.fbx");
	assert(hModel_ >= 0);
}

PlayerBlock::~PlayerBlock()
{
}

void PlayerBlock::Initialize()
{
	transform_.position_ = { 0, 0, 0 };
	transform_.scale_ = { 0.1,0.1,0.1 };
	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.9, 0.9, 0.9 });

	AddCollider(collision);
}

void PlayerBlock::Update()
{
	transform_.rotate_.y += 2.0f;


    transform_.scale_.x += 0.02f;
	transform_.scale_.y += 0.02f;
	transform_.scale_.z += 0.02f;

	if (transform_.scale_.x > 1.0)
	{
		transform_.scale_.x = 1.0f;
		transform_.scale_.y = 1.0f;
		transform_.scale_.z = 1.0f;
		transform_.rotate_.y = 0.0f;
	}
}

void PlayerBlock::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void PlayerBlock::Release()
{
}

void PlayerBlock::OnCollision(GameObject* parent)
{
}
