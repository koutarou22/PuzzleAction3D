#include "Bullet.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Engine/Debug.h"
#include "ENgine/SphereCollider.h"

namespace
{
    const float BulletSpeed = 0.1f;
    const float MAX_RANGE = -11;

    XMFLOAT3 ColliderSize = { 0.3,  0.3,  0.3 };
}

Bullet::Bullet(GameObject* parent):GameObject(parent,"Bullet")
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
	hModel_ = Model::Load("Bullet.fbx");
	assert(hModel_ >= 0);

    BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { ColliderSize });
    
    AddCollider(collision);

   
}

void Bullet::Update()
{
  
	transform_.position_.z -= BulletSpeed;

    if (transform_.position_.z < MAX_RANGE)
    {
        KillMe();
    }
    //ŒãX•ûŒü‚ðØ‚è‘Ö‚¦‚½‚è‚Å‚«‚éˆ—‚ðŽÀ‘•—\’è
}

void Bullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bullet::Release()
{
}

void Bullet::OnCollision(GameObject* parent)
{
    if (parent->GetObjectName() == "Player")
    {
        Player* pPlayer = (Player*)FindObject("Player");

        if (pPlayer != nullptr)
        {
            pPlayer->SetHitEnmeyFlag(true);
        }
    }
    if (parent->GetObjectName() == "PlayerBlock")
    {
        KillMe();
    }

}
