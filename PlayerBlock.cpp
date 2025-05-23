#include "PlayerBlock.h"
#include "Engine/Model.h"
#include"Engine/Collider.h"
#include "Engine/Debug.h"
#include "Engine/Input.h"
#include "Player.h"

namespace
{
	const float MOVE_SPEED = 0.5f;
	const int MAX_RANGE = 9;
}
PlayerBlock::PlayerBlock(GameObject* parent) : GameObject(parent, "PlayerBlock"),isHitMoveRight_(false),isHitMoveLeft_(false),isHitMoveForward_(false)
,isHitMoveBackward_(false),MoveHitCheck_(false)
{
	hModel_ = Model::Load("BoxBrick.fbx");
	assert(hModel_ >= 0);
}

PlayerBlock::~PlayerBlock()
{
}

void PlayerBlock::Initialize()
{
	transform_.position_ = { 0, 0, 0 };
	//transform_.scale_ = { 0.1,0.1,0.1 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1.0,  1.0,  1.0 });

	AddCollider(collision);

	
}

void PlayerBlock::Update()
{
	Player* pPlayer = (Player*)FindObject("Player");

	if (pPlayer != nullptr)
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

			pPlayer->SetBlockAnimeEnd(false);
		}

		if (isHitMoveRight_ && Input::IsKey(DIK_E))
		{
			transform_.position_.x += MOVE_SPEED;
			MoveHitCheck_ = true;
		}
		if (isHitMoveLeft_ && Input::IsKey(DIK_E))
		{
			transform_.position_.x -= MOVE_SPEED;
			MoveHitCheck_ = true;
			
		}
		if (isHitMoveForward_ && Input::IsKey(DIK_E))
		{
			transform_.position_.z -= MOVE_SPEED;
			MoveHitCheck_ = true;
			
		}
		if (isHitMoveBackward_ && Input::IsKey(DIK_E))
		{
			transform_.position_.z += MOVE_SPEED;
			MoveHitCheck_ = true;
		}

		BlockRange();
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

void PlayerBlock::BlockRange()
{

	if (transform_.position_.x < 0)
	{
		transform_.position_.x = 0;
		transform_.rotate_.x = 0.0;
	}
	if (transform_.position_.x > MAX_RANGE)
	{
		transform_.position_.x = MAX_RANGE;
		transform_.rotate_.x = 0.0;
	}
	if (transform_.position_.z < 0)
	{
		transform_.position_.z = 0;
		transform_.rotate_.x = 0.0;
	}
	if (transform_.position_.z > MAX_RANGE)
	{
		transform_.position_.z = MAX_RANGE;
		transform_.rotate_.x = 0.0;
	}
}
