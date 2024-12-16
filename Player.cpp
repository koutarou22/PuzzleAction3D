#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/SphereCollider.h"
#include "Stage.h"
#include <iostream>
#include "Engine/Debug.h"

namespace
{
	const int MAX_RANGE = 9;
	float MOVE_SPEED = 1.0;
	const float GROUND = 5.0f;
	const float GROUND_LIMIT = 1.0f;
	const float JUMP_HEIGHT = 1.5f; 
	const float GRAVITY = 0.005f; 
	const float MAX_GRAVITY = 6.0f;//Limit Gravity
}
Player::Player(GameObject* parent) :GameObject(parent, "Player")
{
	onGround = true;
	Jump_Power = 0.0;
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("BoxTest.fbx");
	assert(hModel_ >= 0);

	/*pFbx = new FBX;
	pFbx->Load("Assets/oden3.fbx");*/
	transform_.position_ = { 0, 0, 0 };
	transform_.position_.y = GROUND;
	//transform_.position_.x = 4.0;
	//transform_.position_.z = 4.0;
	//transform_.position_.y = 10.5;
	//this->transform_.scale_.x = 3.0;
	//this->transform_.scale_.y = 3.0;
	//this->transform_.scale_.z = 3.0;

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1, 1, 1 });
	AddCollider(collision);
}

void Player::Update()
{
	PlayerControl();
	PlayerRange();
	GroundCheck();
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
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

	//if (transform_.position_.y <= GROUND +2)
	//{
	//	transform_.position_.y = GROUND + 2;
	//	onGround = true;
	//}


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
	if (transform_.position_.x > MAX_RANGE)
	{
		transform_.position_.x = MAX_RANGE;
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

void Player::Jump()
{
	Jump_Power = sqrtf(2 *GRAVITY * JUMP_HEIGHT);
	onGround = false;
}

void Player::GroundCheck()
{
	Debug::Log("test", true);
	Debug::Log(transform_.position_.x);
	Debug::Log(",");
	Debug::Log(transform_.position_.y);
	Debug::Log(",");
	Debug::Log(transform_.position_.z,true);
	Stage* pStage = (Stage*)FindObject("Stage");
	int* hStageModel = pStage->GetModelHandle();

	for (int x = 0; x < pStage->GetWidth(); x++)
	{
		for (int z = 0; z < pStage->GetHeight(); z++)
		{
			Stage::Data StageData = pStage->GetTableData(x, z);
			int type = StageData.type;

			for (int i = 0; i < 1; i++)
			{
				RayCastData data;
				data.start = transform_.position_;
				data.start.y = transform_.position_.y;//ここ怪しい
				data.dir = XMFLOAT3(0, -1, 0);

				//Debug::Log("ステージの位置 : (");
				//Debug::Log(x);
				//Debug::Log(",");
				//Debug::Log(z);
				//Debug::Log(")");
				//Debug::Log("タイプ :");


				Model::RayCast(hStageModel[type], &data);

				if (data.hit == true)
				{
					Debug::Log("レイキャストが当たった!",true);
					transform_.position_.y -= data.dist + 1.5f;
					Debug::Log("ステージの位置 : (");
					Debug::Log(x);
					Debug::Log(",");
					Debug::Log(z);
					Debug::Log(")");
					//Debug::Log("タイプ :");
					//触れた瞬間重力を0にする
				}
				else
				{
					//Debug::Log("レイキャストが当たってない...");
				}
			}
		}
	}
}

