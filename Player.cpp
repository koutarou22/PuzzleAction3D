#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Stage.h"

namespace
{
	float MOVE_SPEED = 1.0;
	const float GROUND = 1.0f;
	const float GROUND_LIMIT = 1.0f;
	const float JUMP_HEIGHT = 1.5f; 
	const float GRAVITY = 0.005f; 
	const float MAX_GRAVITY = 6.0f;//Limit Gravity
}
Player::Player(GameObject* parent) :GameObject(parent, "Player")
{
	onGround = true;
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel = Model::Load("BoxTest.fbx");
	assert(hModel < 1);

	/*pFbx = new FBX;
	pFbx->Load("Assets/oden3.fbx");*/
	transform_.position_.y = GROUND;
	/*transform_.position_.x = 4.0;
	transform_.position_.z = 4.0;
	transform_.position_.y = 10.5;*/
	//this->transform_.scale_.x = 3.0;
	//this->transform_.scale_.y = 3.0;
	//this->transform_.scale_.z = 3.0;
}

void Player::Update()
{
	
	PlayerControl();
	PlayerRange();

	//RayCastData data{};
	//data.start = StagePos;
	//data.dir = XMFLOAT3(0.0f, -1.0f, 0.0f); // 下方向にレイを飛ばす

	//if (data.hit == true)
	//{
	//	transform_.position_.y = -data.dist + 1.0;
	//	onGround = true;
	//}

	//if (transform_.position_.y < GROUND)
	//{
	//	transform_.position_.y = GROUND;
	//	onGround = true;
	//}
}

void Player::Draw()
{
	Model::SetTransform(hModel, transform_);
	Model::Draw(hModel);
}

void Player::Release()
{
}

void Player::PlayerControl()
{
	XMVECTOR vFront = { 0,0,1,0 };
	XMVECTOR move{ 0,0,0,0 };


	if (Input::IsKeyDown(DIK_A))
	{
		transform_.position_.x -= MOVE_SPEED;
		move = XMVECTOR{ -1,0,0,0 };
	}
	if (Input::IsKeyDown(DIK_D))
	{
		transform_.position_.x += MOVE_SPEED;
		move = XMVECTOR{ 1,0,0,0 };
	}
	if (Input::IsKeyDown(DIK_W))
	{
		transform_.position_.z += MOVE_SPEED;
		move = XMVECTOR{ 0,0,1,0 };
	}
	if (Input::IsKeyDown(DIK_S))
	{
		transform_.position_.z -= MOVE_SPEED;
		move = XMVECTOR{ 0,0,-1,0 };
	}

	if (Input::IsKeyDown(DIK_SPACE))
	{
		if (prevSpaceKey == false)
		{
			if (onGround)
			{
				Jump();
			}
		}
		prevSpaceKey = true;
	}
	else
	{
		prevSpaceKey = false;
	}

	Jump_Power -= GRAVITY; // 重力を適用
	transform_.position_.y += Jump_Power; 

	
	if (transform_.position_.y <= GROUND)
	{
		transform_.position_.y = GROUND;
		onGround = true;
	}


	XMVECTOR pos = XMLoadFloat3(&(transform_.position_));//	現在の位置を保存
	//XMVECTOR posTmp = XMVectorZero();

	if (!XMVector3Equal(move, XMVectorZero()))
	{
		XMStoreFloat3(&(transform_.position_), pos);

		XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(-XM_PIDIV2));
		XMVECTOR modifiedVec = XMPlaneTransform(move, rot);

		XMVECTOR vdot = XMVector3Dot(vFront, move);

		assert(XMVectorGetX(vdot) <= 1 && XMVectorGetX(vdot) >= -1);

		float angle = atan2(XMVectorGetX(move), XMVectorGetZ(move));

		transform_.rotate_.y = XMConvertToDegrees(angle);
	}
}

void Player::PlayerRange()
{
	if (transform_.position_.x < 0)
	{
		transform_.position_.x = 0;
	}
	if (transform_.position_.x > 9)
	{
		transform_.position_.x = 9;
	}
	if (transform_.position_.z < 0)
	{
		transform_.position_.z = 0;
	}
	if (transform_.position_.z > 9)
	{
		transform_.position_.z = 9;
	}
}

void Player::Jump()
{
	Jump_Power = sqrtf(2 *GRAVITY * JUMP_HEIGHT);
	onGround = false;
}
