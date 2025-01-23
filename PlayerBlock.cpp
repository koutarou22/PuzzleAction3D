#include "PlayerBlock.h"
#include "Engine/Model.h"
#include"Engine/Collider.h"
#include "Engine/Debug.h"
#include "Player.h"

PlayerBlock::PlayerBlock(GameObject* parent) : GameObject(parent, "PlayerBlock"), TimeCount_(60),isTimerZero(false),TimeFullVerdict(false)
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

	if (TimeCount_ > 0)
	{
		TimeCount_--;
	}

	if (TimeCount_ == 0)
	{
		isTimerZero = true;

		if (isTimerZero == true)
		{
			transform_.position_.y -= 0.05;
		}
	}



}

void PlayerBlock::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	{
		ImGui::Text("Block is Fall%5.2lf", TimeCount_);
	}
}

void PlayerBlock::Release()
{
}

void PlayerBlock::OnCollision(GameObject* parent)
{
}
