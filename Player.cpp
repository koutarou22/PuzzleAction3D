#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/BoxCollider.h"
#include "Stage.h"
#include <iostream>
#include "Engine/Debug.h"
#include "PlayerBlock.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "KeyFlag.h"

namespace
{
    const int MAX_RANGE = 9;//プレイヤーが行ける範囲
    float MOVE_SPEED = 1.0f;//プレイヤーの移動速度
    const float GROUND = 1.0f;//初期位置(Y)
    const float GROUND_LIMIT = 1.0f;
    const float JUMP_HEIGHT = 1.5f;
    const float GRAVITY = 0.005f;
    const float MAX_GRAVITY = 6.0f;
}

Player::Player(GameObject* parent) : GameObject(parent, "Player"),ClearFlag_(false),onGround(true),Jump_Power(0.0f)
{
}

Player::~Player()
{
}

void Player::Initialize()
{
    hModel_ = Model::Load("DebugBox.fbx");
    assert(hModel_ >= 0);

    transform_.position_ = { 0, 0, 0 };

    BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1, 1, 1 });
    AddCollider(collision);
}

void Player::Update()
{
    PlayerControl();
    PlayerRange();
    StageHeight();
}

void Player::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

    {
        ImGui::Text("Player Position%5.2lf,%5.2lf,%5.2lf", transform_.position_.x, transform_.position_.y, transform_.position_.z);
        ImGui::Text("Player Jump Pawer%5.2lf", Jump_Power);
    }
}

void Player::Release()
{
   
}

void Player::PlayerControl()
{
    XMVECTOR move = XMVectorZero();
    XMVECTOR newPosition;

    if (Input::IsKeyDown(DIK_A))
    {
        newPosition = XMVectorSet(transform_.position_.x - MOVE_SPEED, transform_.position_.y + 0.01f, transform_.position_.z, 0.0f);
        if (!IsBlocked(newPosition))
        {
            transform_.position_.x -= MOVE_SPEED;
            move = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
            MoveDirection = LEFT;
        }
    }
    if (Input::IsKeyDown(DIK_D))
    {
        newPosition = XMVectorSet(transform_.position_.x + MOVE_SPEED, transform_.position_.y + 0.01f, transform_.position_.z, 0.0f);
        if (!IsBlocked(newPosition))
        {
            transform_.position_.x += MOVE_SPEED;
            move = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
            MoveDirection = RIGHT;
        }
    }
    if (Input::IsKeyDown(DIK_W))
    {
        newPosition = XMVectorSet(transform_.position_.x, transform_.position_.y + 0.01f, transform_.position_.z + MOVE_SPEED, 0.0f);
        if (!IsBlocked(newPosition))
        {
            transform_.position_.z += MOVE_SPEED;
            move = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
            MoveDirection = FORWARD;
        }
    }
    if (Input::IsKeyDown(DIK_S))
    {
        newPosition = XMVectorSet(transform_.position_.x, transform_.position_.y + 0.01f, transform_.position_.z - MOVE_SPEED, 0.0f);
        if (!IsBlocked(newPosition))
        {
            transform_.position_.z -= MOVE_SPEED;
            move = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
            MoveDirection = BACKWARD;
        }
    }

    if (Input::IsKeyDown(DIK_SPACE))
    {
        if (prevSpaceKey == false && onGround)
        {
            Jump();
            MoveDirection = NONE; 
        }
        prevSpaceKey = true;
    }
    else
    {
        prevSpaceKey = false;
    }



    if (Input::IsKeyDown(DIK_L))
    {
        PlayerBlockInstans();
    }

    Jump_Power -= GRAVITY;
    transform_.position_.y += Jump_Power;

    if (!XMVector3Equal(move, XMVectorZero()))
    {
        XMVECTOR pos = XMLoadFloat3(&(transform_.position_));

        XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(-XM_PIDIV2));
        XMVECTOR modifiedVec = XMVector3TransformNormal(move, rot);

        float angle = atan2(XMVectorGetX(move), XMVectorGetZ(move));

        transform_.rotate_.y = XMConvertToDegrees(angle);

        XMStoreFloat3(&(transform_.position_), pos);
    }
}

void Player::PlayerBlockInstans()
{
    PlayerBlock* existingBlock = (PlayerBlock*)FindObject("PlayerBlock");

    if (existingBlock != nullptr)
    {
      existingBlock ->KillMe();
    }

    XMVECTOR PlayerPos = XMLoadFloat3(&(transform_.position_));
    XMVECTOR FrontDirection = XMVectorSet(0, 0, 1, 0);
    XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

    XMVECTOR blockPos = PlayerPos + FrontDirection * 1.0f;

    PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
    XMStoreFloat3(&(block->GetTransform()), blockPos);
}

void Player::OnCollision(GameObject* parent)
{
    PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");

    if (parent->GetObjectName() == "PlayerBlock")
    {
        XMVECTOR blockPos = XMLoadFloat3(&(pBlock->GetTransform()));
        float blockY = XMVectorGetY(blockPos);

        if (transform_.position_.y <= pBlock->GetTransform().y)
        {
            if (MoveDirection == LEFT)
            {
                transform_.position_.x += MOVE_SPEED;
            }
            else if (MoveDirection == RIGHT)
            {
                transform_.position_.x -= MOVE_SPEED;
            }
            else if (MoveDirection == FORWARD)
            {
                transform_.position_.z -= MOVE_SPEED;
            }
            else if (MoveDirection == BACKWARD)
            {
                transform_.position_.z += MOVE_SPEED;
            }
        }

        if (transform_.position_.y > pBlock->GetTransform().y)
        {
            transform_.position_.y = pBlock->GetTransform().y + 1.0f;
            onGround = true;
            Jump_Power = sqrtf(3 * GRAVITY * JUMP_HEIGHT);
        }
        else
        {
            onGround = false;
        }

        MoveDirection = NONE; 
    }

    KeyFlag* pKey = (KeyFlag*)FindObject("KeyFlag");

    if (parent->GetObjectName() == "KeyFlag")
    {
        pKey->KillMe();
        ClearFlag_ = true;
    }
}

void Player::StageHeight()
{
    Stage* stage = (Stage*)FindObject("Stage");
    PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");
    if (stage)
    {
        float GroundHeight = stage->GetGroundHeight(transform_.position_.x, transform_.position_.z);

        if (transform_.position_.y <= GroundHeight)
        {
            transform_.position_.y = GroundHeight;
            onGround = true;
            Jump_Power = 0.0f;
        }
        else
        {
            onGround = false;
        }
    }
}

bool Player::IsBlocked(XMVECTOR Position)
{
    Stage* stage = (Stage*)FindObject("Stage");

    if (stage)
    {
        int X = (int)(XMVectorGetX(Position));
        int Z = (int)(XMVectorGetZ(Position));

        if (X >= 0 && X < stage->GetWidth() && Z >= 0 && Z < stage->GetHeight())
        {
            float blockHeight = stage->GetBlockHeight(X, Z);

            // プレイヤーの高さとブロックの高さが同じ以上だったら
            if (blockHeight >= XMVectorGetY(Position))
            {
                Debug::Log("ステージのブロックに接触", true);
                return true;
            }
        }
    }

    return false;
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
    Jump_Power = sqrtf(2 * GRAVITY * JUMP_HEIGHT);
    onGround = false;
}
