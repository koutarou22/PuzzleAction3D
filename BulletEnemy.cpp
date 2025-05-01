#include "BulletEnemy.h"
#include "Player.h"
#include "Engine/Debug.h"
#include "Engine/Model.h"
#include "Bullet.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "PlayerBlock.h"

namespace
{
   int NextBullletCoolDown = 300;
}

BulletEnemy::BulletEnemy(GameObject* parent) : GameObject(parent, "BulletEnemy"),isAttack_(false)
{
    Timer_ = NextBullletCoolDown;
}

BulletEnemy::~BulletEnemy()
{
}

void BulletEnemy::Initialize()
{
    hModel_ = Model::Load("Cannon.fbx");
    assert(hModel_ >= 0);
    transform_.position_ = { 7.0,3.5,9.0 };

    //transform_.rotate_.y = -90;
    BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1,1,1 });
    AddCollider(collision);
}

void BulletEnemy::Update()
{
    if (Timer_ > 0)
    {
       Timer_--;
    }
   
    if (Timer_ == 0)
    {
        XMFLOAT3 cannonTopPos = Model::GetBonePosition(hModel_, "CannonPos");
        Bullet* pBullet = Instantiate<Bullet>(this->GetParent()->GetParent());
        pBullet->SetPosition(cannonTopPos);

        Timer_ = NextBullletCoolDown;
    }


}

void BulletEnemy::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

  /*  ImGui::Text("Cannon CoolTime %d",Timer_);*/
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
            pPlayer->KillMe();
        }
    }

    if (parent->GetObjectName() == "PlayerBlock")
    {
        KillMe();
    }
}
