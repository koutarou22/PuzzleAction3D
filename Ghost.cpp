#include "Ghost.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Player.h"
#include "PlayerBlock.h"
#include "Engine/SceneManager.h"
#include "Residue.h"
#include "Stage.h"

namespace
{
    const float SRATE = 0.07f;//補間速度
    const float GRAVITY = 0.005f;

    const int GRID_WIDTH = 10;
    const int GRID_HEIGHT = 10;
    const int MAX_STAGE_HEIGHT = 10;
    const int GRID_OFFSET_X = 5;
    const int GRID_OFFSET_Z = 4;


    int GROUND = 1.0f;
    float JUMP_HEIGHT = 1.0f;//ジャンプ力

    XMFLOAT3 prepos;  
    XMFLOAT3 nextpos;  
    float moveRatio = 0.0f;

    bool isJumping = false;
    bool onGround = true;
    bool isFalling = false;

    XMFLOAT3 AddXMFLOAT3(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return { a.x + b.x, a.y + b.y, a.z + b.z };
    }

    XMFLOAT3 MulXMFLOAT3(float t, const XMFLOAT3& b)
    {
        return { t * b.x, t * b.y, t * b.z };
    }

    bool IsZero(const XMFLOAT3& v)
    {
        return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f;
    }
}


MOVE_GHOST_METHOD Ghost::GhostCanMoveTo(const XMFLOAT3& pos)
{
    int gx = static_cast<int>(pos.x + GRID_OFFSET_X);
    int gy = static_cast<int>(GRID_OFFSET_Z - pos.z);
    int gz = static_cast<int>(pos.y);

    //画面外にいかない処理
    if (gx < 0 || gx >= GRID_WIDTH  || gy < 0 || gy >= GRID_HEIGHT || gz < 0 || gz >= MAX_STAGE_HEIGHT)
    {
        return CAN_MOVE_TURN;
    }

    auto* stage = static_cast<Stage*>(FindObject("Stage"));
    auto& grid = stage->GetStageGrid();

    int current = grid[gz][GRID_HEIGHT - 1 - gy][gx];

    if (current == 5 && gz > 0)
    {
        return CAN_MOVE_TURN;
    }

  return CANT_TURN;
}

void Ghost::EnemyMoveMent()
{
}

Ghost::Ghost(GameObject* parent) : GameObject(parent, "Ghost")
{
    GhostDirection = 0.05f;
}

Ghost::~Ghost()
{
}

void Ghost::Initialize()
{
	hModel_ = Model::Load("Ghostlow.fbx");
	assert(hModel_ >= 0);


	transform_.rotate_.y = -90.0f;
    //transform_.rotate_.x = 90.0f;
    transform_.scale_ = { 1.5,1.5,1.5 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.5, 0.5, 0.5 });
	AddCollider(collision);
}

void Ghost::Update()
{
    XMFLOAT3 next = transform_.position_;
    next.x += GhostDirection;

    transform_.position_ = next;

    // 移動可否をチェック
    MOVE_GHOST_METHOD result = GhostCanMoveTo(next);

    if (result == CAN_MOVE_TURN)
    {
        GhostDirection = -GhostDirection;
        transform_.rotate_.y += 180.0f;
    }
 /*   else if (result == CANT_TURN)
    {
        transform_.position_ = next;
    }*/

}

void Ghost::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ghost::Release()
{
}
void Ghost::OnCollision(GameObject* parent)
{
    if (parent->GetObjectName() == "Player")
    {
        Player* pPlayer = (Player*)FindObject("Player");

        if (pPlayer != nullptr)
        {
            Debug::Log("エネミーとプレイヤーが接触した", true);
            pPlayer->SetHitEnmeyFlag(true);
        }
    }

    if (parent->GetObjectName() == "PlayerBlock")
    {
        PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");
        if (pBlock != nullptr)
        {
            bool MoveBlock = pBlock->GetMoveHit();
            if (MoveBlock)
            {
                KillMe();
            }
            else
            {
                GhostDirection = -GhostDirection;
                transform_.rotate_.y += 180.0f;
            }
        }
    }
}


void Ghost::CanMoveRenge()
{
    // X軸の範囲チェック
    if (transform_.position_.x < 0)
    {
        transform_.position_.x = 0; 
        GhostDirection = -GhostDirection;
        transform_.rotate_.y += 180.0f; 
    }
    else if (transform_.position_.x > MAX_RANGE)
    {
        transform_.position_.x = MAX_RANGE; 
        GhostDirection = -GhostDirection; 
        transform_.rotate_.y += 180.0f; 
    }

   
    if (transform_.position_.z < 0)
    {
        transform_.position_.z = 0;
        GhostDirection = -GhostDirection; 
        transform_.rotate_.y += 180.0f;
    }
    else if (transform_.position_.z > MAX_RANGE)
    {
        transform_.position_.z = MAX_RANGE;
        GhostDirection = -GhostDirection;
        transform_.rotate_.y += 180.0f;
    }
}

void Ghost::AddShadow(XMFLOAT3 pos)
{
    Ghost* pEnemy = (Ghost*)FindObject("Ghost");
    if (shadows.size() < 1)
    {
        Shadow* pShadow = Instantiate<Shadow>(this);
        pShadow->SetPosition(pos);
        shadows.push_back(pShadow);

    }

}

