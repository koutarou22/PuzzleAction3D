#include "BulletEnemy.h"
#include "Player.h"
#include "Engine/Debug.h"
#include "Engine/Model.h"

BulletEnemy::BulletEnemy(GameObject* parent) : GameObject(parent, "BulletEnemy")
{
}

BulletEnemy::~BulletEnemy()
{
}

void BulletEnemy::Initialize()
{
}

void BulletEnemy::Update()
{
}

void BulletEnemy::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);
}

void BulletEnemy::Release()
{
}

void BulletEnemy::OnCollision(GameObject* parent)
{
    if (parent->GetObjectName() == "Player")
    {
        Player* pPlayer = (Player*)FindObject("Player");

        if (pPlayer != nullptr)
        {
            Debug::Log("ブロックとプレイヤーが接触した", true);
            pPlayer->KillMe();
        }
    }

    if (parent->GetObjectName() == "PlayerBlock")
    {

    }
}
