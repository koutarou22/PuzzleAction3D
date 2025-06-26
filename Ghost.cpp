#include "Ghost.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Player.h"
#include "PlayerBlock.h"
#include "Engine/SceneManager.h"
#include "Residue.h"

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
	
    transform_.position_ = { 5.0+ 4.0,2.0, 5.0 - 3.0 };
    transform_.scale_ = { 1.5,1.5,1.5 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.5, 0.5, 0.5 });
	AddCollider(collision);
}

void Ghost::Update()
{
    transform_.position_.x += GhostDirection;
   // CanMoveRenge();
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
            //pPlayer->SetHitEnmeyFlag(true);
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

