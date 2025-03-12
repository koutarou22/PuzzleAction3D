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
#include "Ladder.h"

namespace
{
    const int MAX_RANGE = 9;//プレイヤーが行ける範囲
    const float MAX_MOVE_FRAME = 10;//プレイヤーが再度動けるまでのフレーム
    float MOVE_SPEED = 1.0f;//プレイヤーの移動速度
    float MOVE_AERIAL = 0.05f;//プレイヤーの移動速度
    const float GROUND = 1.0f;//初期位置(Y)
    const float GROUND_LIMIT = 1.0f;
    const float JUMP_HEIGHT = 1.2f;//ジャンプ力
    const float GRAVITY = 0.005f;//重力
    const float MAX_GRAVITY = 6.0f;
}

Player::Player(GameObject* parent) : GameObject(parent, "Player")
,ClearFlag_(false),onGround(true),isBlockCanOnly(false),Jump_Power(0.0f),hPlayerModel_(-1),MoveTimer_(MAX_MOVE_FRAME)
{


}

Player::~Player()
{
}

void Player::Initialize()
{

  /*  hPlayerTestModel_ = Model::Load("Player.fbx");
    assert(hPlayerTestModel_ >= 0);*/

    hPlayerAnimeModel_[0] = Model::Load("Animation//Idle.fbx");
    assert(hPlayerAnimeModel_[0] >= 0);

    Model::SetAnimFrame(hPlayerAnimeModel_[0], 0, 59, 1.0);

    hPlayerAnimeModel_[1] = Model::Load("Animation//Standard Walk.fbx");
    assert(hPlayerAnimeModel_[1] >= 0);

    Model::SetAnimFrame(hPlayerAnimeModel_[1], 0, 11, 1.0);

    hPlayerAnimeModel_[2] = Model::Load("Animation//Magic Heal.fbx");
    assert(hPlayerAnimeModel_[2] >= 0);

    Model::SetAnimFrame(hPlayerAnimeModel_[2], 0, 80, 1.0);

    hPlayerAnimeModel_[3] = Model::Load("Animation//Mma Kick.fbx");
    assert(hPlayerAnimeModel_[3] >= 0);

    Model::SetAnimFrame(hPlayerAnimeModel_[3], 0, 50, 1.0);


    transform_.position_ = { posX,posY,posZ };

    BoxCollider* collision = new BoxCollider({ 0, 0.55, 0 }, { 1, 1, 1 });
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
    transform_.scale_ = { 0.5,0.5,0.5 };
    Model::SetTransform(hPlayerModel_, transform_);
    Model::Draw(hPlayerModel_);

    {
        ImGui::Text("Player Position%5.2lf,%5.2lf,%5.2lf", transform_.position_.x, transform_.position_.y, transform_.position_.z);
        ImGui::Text("Player Jump Pawer%5.2lf", Jump_Power);
        ImGui::Text("Player Move CoolTime%5.2lf", MoveTimer_);

        {
            static float pos[3] = { posX,posY,posZ };
            ImGui::Separator();

            if (ImGui::InputFloat3("Player_Position", pos, "%.3f"))
            {
                transform_.position_ = { pos[0],pos[1], pos[2] };
            }
        }
    }
}

void Player::Release()
{
   
}

void Player::PlayerControl()
{
    XMVECTOR move = XMVectorZero();
    XMVECTOR newPosition;

    bool isMoving = false;


    //ジャンプの処理
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

    //左移動の処理
    if (Input::IsKey(DIK_A))
    {

        if (onGround)
        {
            newPosition = XMVectorSet(transform_.position_.x - MOVE_SPEED, transform_.position_.y + 0.01f, transform_.position_.z, 0.0f);
            if (!IsBlocked(newPosition))
            {
                MoveTimer_--;
                if (MoveTimer_ == 0)
                {
                    transform_.position_.x -= MOVE_SPEED;
                    move = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
                    MoveDirection = LEFT;
                    MoveTimer_ = MAX_MOVE_FRAME;
                    SetPlayerAnimation(1);
                    isMoving = true;
                }
            }
        }
        else
        {
            move = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
            newPosition = XMVectorSet(transform_.position_.x - MOVE_AERIAL, transform_.position_.y + 0.01f, transform_.position_.z, 0.0f);
            transform_.position_.x -= MOVE_AERIAL;
        }
    }

    //右移動の処理
    if (Input::IsKey(DIK_D))
    {
        if (onGround)
       {
            newPosition = XMVectorSet(transform_.position_.x + MOVE_SPEED, transform_.position_.y + 0.01f, transform_.position_.z, 0.0f);
            if (!IsBlocked(newPosition))
            {
                MoveTimer_--;
                if (MoveTimer_ == 0)
                {
                    transform_.position_.x += MOVE_SPEED;
                    move = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
                    MoveDirection = RIGHT;
                    MoveTimer_ = MAX_MOVE_FRAME;
                    SetPlayerAnimation(1);
                    isMoving = true;
                }
            }
        }
        else
        {
            move = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
            transform_.position_.x += MOVE_AERIAL;
        }

    }

    //奥移動の処理
    if (Input::IsKey(DIK_W))
    {
        if (onGround)
        {
            newPosition = XMVectorSet(transform_.position_.x, transform_.position_.y + 0.01f, transform_.position_.z + MOVE_SPEED, 0.0f);
            if (!IsBlocked(newPosition))
            {
                MoveTimer_--;
                if (MoveTimer_ == 0)
                {
                    transform_.position_.z += MOVE_SPEED;
                    move = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
                    MoveDirection = FORWARD;
                    MoveTimer_ = MAX_MOVE_FRAME;
                    SetPlayerAnimation(1);
                    isMoving = true;
                }

            }
        }
        else
        {
            move = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
            transform_.position_.z += MOVE_AERIAL;
        }

    }
    //手前移動の処理
    if (Input::IsKey(DIK_S))
    {
        if (onGround)
        {
            newPosition = XMVectorSet(transform_.position_.x, transform_.position_.y + 0.01f, transform_.position_.z - MOVE_SPEED, 0.0f);
            if (!IsBlocked(newPosition))
            {
                MoveTimer_--;
                if (MoveTimer_ == 0)
                {
                    transform_.position_.z -= MOVE_SPEED;
                    move = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
                    MoveDirection = BACKWARD;
                    MoveTimer_ = MAX_MOVE_FRAME;

                    isMoving = true;
                }
            }
        }
        else
        {
            move = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
            transform_.position_.z -= MOVE_AERIAL;
        }

    }

    if (!isMoving)
    {
      /*  SetPlayerAnimation(0); */
    }

    if (Input::IsKeyDown(DIK_L) && !isBlockCanOnly)
    {
        SetPlayerAnimation(2);
        PlayerBlockInstans();
        isBlockCanOnly = true;

    }

    Jump_Power -= GRAVITY;
    transform_.position_.y += Jump_Power;

   /* if (onGround)
    {
        transform_.position_.x = 1.0f;
    }*/

    //移動した方向に向く処理
    if (!XMVector3Equal(move, XMVectorZero()))
    {
        XMVECTOR pos = XMLoadFloat3(&(transform_.position_));

        XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(-XM_PIDIV2));
        XMVECTOR modifiedVec = XMVector3TransformNormal(move, rot);

        float angle = atan2(XMVectorGetX(move), XMVectorGetZ(move));

        transform_.rotate_.y = XMConvertToDegrees(angle);

        XMStoreFloat3(&(transform_.position_), pos);
    }


    if (onGround)
    {
        float gridSize = 1.0f; 
        float x = round(transform_.position_.x / gridSize) * gridSize;
        float y = round(transform_.position_.y / gridSize) * gridSize;
        float z = round(transform_.position_.z / gridSize) * gridSize;
        transform_.position_.x = x;
        transform_.position_.y = y;
        transform_.position_.z = z;
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
    XMVECTOR FrontDirection = XMVectorSet(0, 0.5, 1, 0);


    if (Input::IsKey(DIK_UP))
    {
        FrontDirection = XMVectorSet(0, 1.5, 1, 0);
    }

    if (Input::IsKey(DIK_DOWN))
    {
        FrontDirection = XMVectorSet(0, -0.5, 1, 0);
    }

    XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

    XMVECTOR blockPos = PlayerPos + FrontDirection * 1.0f;

    PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
    XMStoreFloat3(&(block->GetPosition()), blockPos);

    
}

void Player::SetPlayerAnimation(int AnimeType)
{
    switch (AnimeType)
    {
    case 0:
        hPlayerModel_ = hPlayerAnimeModel_[0];
        break;
    case 1:
        hPlayerModel_ = hPlayerAnimeModel_[1];
        break;
    case 2:
        hPlayerModel_ = hPlayerAnimeModel_[2];
        break;
    case 3:
        hPlayerModel_ = hPlayerAnimeModel_[3];
        break;
    default:
        break;
    }
}

void Player::OnCollision(GameObject* parent)
{
    PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");

    if (parent->GetObjectName() == "PlayerBlock")
    {
        XMVECTOR blockPos = XMLoadFloat3(&(pBlock->GetPosition()));
        float blockY = XMVectorGetY(blockPos);

        if (transform_.position_.y <= pBlock->GetPosition().y)
        {
            SetPlayerAnimation(3);
            if (MoveDirection == LEFT)
            {
                transform_.position_.x += MOVE_SPEED;
                pBlock->SetMoveLeft(true);
               
            }
            else if (MoveDirection == RIGHT)
            {
                transform_.position_.x -= MOVE_SPEED;
                pBlock->SetMoveRight(true);
            }
            else if (MoveDirection == FORWARD)
            {
                transform_.position_.z -= MOVE_SPEED;
                pBlock->SetMoveForward(true);
            }
            else if (MoveDirection == BACKWARD)
            {
                transform_.position_.z += MOVE_SPEED;
                pBlock->SetMoveBackwaed(true);
            }
        }

        if (transform_.position_.y > pBlock->GetPosition().y)
        {
            transform_.position_.y = pBlock->GetPosition().y +  0.45f;

            Jump_Power = 0.0f;
            onGround = true;
        }

        MoveDirection = NONE; 
    }

   /* Ladder* pLadder= (Ladder*)FindObject("Ladder");

    if (parent->GetObjectName() == "Ladder")
    {
        if (transform_.position_.y > pLadder->GetPosition().y)
        {
            transform_.position_.y = pLadder->GetPosition().y + 0.90f;

            Jump_Power = 0.0f;
        }
    }*/

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
        int GroundHeight = stage->GetGroundHeight(transform_.position_.x, transform_.position_.z);

        //高さが〇〇以上なら乗る
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
        int X = (float)(XMVectorGetX(Position));
        int Z = (float)(XMVectorGetZ(Position));

        if (X >= 0 && X < stage->GetWidth() && Z >= 0 && Z < stage->GetHeight())
        {
            float blockHeight = stage->GetBlockHeight(X, Z);

            // プレイヤーの高さとブロックの高さが同じ以上だったら
            if (blockHeight >= XMVectorGetY(Position) )
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
    MoveDirection = NONE;
}
