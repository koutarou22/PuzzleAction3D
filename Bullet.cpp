#include "Bullet.h"
#include "Engine/Model.h"
#include "Player.h"
#include "Engine/Debug.h"
#include "ENgine/SphereCollider.h"

namespace
{
    const int BulletSpeed = 0.5f;
}

Bullet::Bullet(GameObject* parent)
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
	hModel_ = Model::Load("Bullet.fbx");
	assert(hModel_ >= 0);

    SphereCollider* collision = new SphereCollider({ 0.5, 0.5, 0.5 }, 0.0f);
    AddCollider(collision);
}

void Bullet::Update()
{
  
	transform_.position_.z += BulletSpeed;
    //��X������؂�ւ�����ł��鏈��������
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
            Debug::Log("�u���b�N�ƃv���C���[���ڐG����", true);
            pPlayer->KillMe();
        }
    }
}
