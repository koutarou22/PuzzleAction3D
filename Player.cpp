#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/BoxCollider.h"
#include "Stage.h"
#include <iostream>
#include "Engine/Debug.h"
#include "PlayerBlock.h"

namespace
{
    const int MAX_RANGE = 9;//プレイヤーが行ける範囲
    float MOVE_SPEED = 1.0;//プレイヤーの移動速度
    const float GROUND = 1.0f;//初期位置(Y)
    const float GROUND_LIMIT = 1.0f;
    const float JUMP_HEIGHT = 1.5f;
    const float GRAVITY = 0.005f;
    const float MAX_GRAVITY = 6.0f;
}

Player::Player(GameObject* parent) : GameObject(parent, "Player")
{
    onGround = true;
    Jump_Power = 0.0;
}

Player::~Player()
{
}

void Player::Initialize()
{
    hModel_ = Model::Load("robot.fbx");
    assert(hModel_ >= 0);

    transform_.position_ = { 0, 0, 0 };
    transform_.position_.y = 5;

    BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1, 1, 1 });
    AddCollider(collision);
}

void Player::Update()
{
    PlayerControl();
    PlayerRange();
    StageHeight();

    Debug::Log(transform_.position_.x, false);
    Debug::Log(",");
    Debug::Log(transform_.position_.y, false);
    Debug::Log(",");
    Debug::Log(transform_.position_.z, true);
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
    XMVECTOR move = XMVectorZero();
    XMVECTOR newPosition;

    if (Input::IsKeyDown(DIK_A))
    {
        newPosition = XMVectorSet(transform_.position_.x - MOVE_SPEED, transform_.position_.y + 0.01f, transform_.position_.z, 0.0f);
        if (!IsBlocked(newPosition))
        {
            transform_.position_.x -= MOVE_SPEED;
            move = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
        }
    }
    if (Input::IsKeyDown(DIK_D))
    {
        newPosition = XMVectorSet(transform_.position_.x + MOVE_SPEED, transform_.position_.y + 0.01f, transform_.position_.z, 0.0f);
        if (!IsBlocked(newPosition))
        {
            transform_.position_.x += MOVE_SPEED;
            move = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
        }
    }
    if (Input::IsKeyDown(DIK_W))
    {
        newPosition = XMVectorSet(transform_.position_.x, transform_.position_.y + 0.01f, transform_.position_.z + MOVE_SPEED, 0.0f);
        if (!IsBlocked(newPosition))
        {
            transform_.position_.z += MOVE_SPEED;
            move = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        }
    }
    if (Input::IsKeyDown(DIK_S))
    {
        newPosition = XMVectorSet(transform_.position_.x, transform_.position_.y + 0.01f, transform_.position_.z - MOVE_SPEED, 0.0f);
        if (!IsBlocked(newPosition))
        {
            transform_.position_.z -= MOVE_SPEED;
            move = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
        }
    }

    if (Input::IsKeyDown(DIK_SPACE))
    {
        if (prevSpaceKey == false && onGround)
        {
            Jump();
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
    XMVECTOR playerPos = XMLoadFloat3(&(transform_.position_));
    XMVECTOR FrontDirection = XMVectorSet(0, 0, 1, 0);
    XMMATRIX rotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    FrontDirection = XMVector3TransformNormal(FrontDirection, rotationMatrix);

    XMVECTOR blockPos = playerPos + FrontDirection * 1.0f;

    PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
    XMStoreFloat3(&(block->GetTransform()), blockPos);
}

void Player::OnCollision(GameObject* parent)
{
    if (parent->GetObjectName() == "PlayerBlock")
    {
        PlayerBlock* block = static_cast<PlayerBlock*>(parent);
        XMVECTOR blockPos = XMLoadFloat3(&(block->GetTransform()));
        float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&transform_.position_) - blockPos));

        if (transform_.position_.y <= block->GetTransform().y)
        {
            Debug::Log("プレイヤーがブロックにぶつかった（壁）", true);
        }
        else
        {
            Debug::Log("プレイヤーがブロックの上に乗った（足場）", true);
            transform_.position_.y = block->GetTransform().y + 1.0f;
            onGround = true;
            Jump_Power = 0.0f;
        }
    }
}


void Player::StageHeight()
{
    Stage* stage = (Stage*)FindObject("Stage");

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
    PlayerBlock* block = (PlayerBlock*)FindObject("PlayerBlock");

    if (stage)
    {
        int X = static_cast<int>(XMVectorGetX(Position));
        int Z = static_cast<int>(XMVectorGetZ(Position));

        if (X >= 0 && X < stage->GetWidth() && Z >= 0 && Z < stage->GetHeight())
        {
            float blockHeight = stage->GetBlockHeight(X, Z);

            // プレイヤーの高さとブロックの高さが同じ、またはそれ以上の場合
            if (blockHeight >= XMVectorGetY(Position))
            {
                Debug::Log("Stageのブロックを感知", true);
                return true;
            }
        }
    }

    if (block)
    {
        XMVECTOR blockPos = XMLoadFloat3(&(block->GetTransform()));
        float distance = XMVectorGetX(XMVector3Length(Position - blockPos));

        // ブロックがプレイヤーの高さと同じ、またはそれ以上の場合
        if (distance < 1.0f && block->GetTransform().y >= XMVectorGetY(Position))
        {
            Debug::Log("PlayerBlockでブロックされています", true);
            return true;
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
