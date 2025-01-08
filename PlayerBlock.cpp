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
	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.9, 0.9, 0.9 });

	AddCollider(collision);
}

void PlayerBlock::Update()
{
}

void PlayerBlock::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void PlayerBlock::Release()
{
}

void PlayerBlock::OnCollision(GameObject* other)
{
	if (other->GetObjectName() == "Player")
	{
		Player* pPlayer = static_cast<Player*>(other);
		Debug::Log("ブロックとプレイヤーが接触した", true);
		pPlayer->OnCollision(GetParent()); 
	}
}
