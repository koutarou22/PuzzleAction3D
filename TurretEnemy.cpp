#include "TurretEnemy.h"
#include "Player.h"
#include "Engine/Debug.h"
#include "Engine/Model.h"
#include "Bullet.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "PlayerBlock.h"

#include "Engine/Audio.h"
namespace
{
   int NextBullletCoolDown = 300;
}

TurretEnemy::TurretEnemy(GameObject* parent) : GameObject(parent, "TurretEnemy"),isAttack_(false)
{
    Timer_ = NextBullletCoolDown;
}

TurretEnemy::~TurretEnemy()
{
}

void TurretEnemy::Initialize()
{
    hModel_ = Model::Load("Cannon.fbx");
    assert(hModel_ >= 0);
    transform_.position_ = { 7.0,3.5,9.0 };

    //transform_.rotate_.y = -90;
    BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1,1,1 });
    AddCollider(collision);

	string TurretPath = "Sound//SE//EnemySE//Turret//";

	TurretSoundSE_[ENEMY_TURRET_SE_ATTACK] = Audio::Load(TurretPath + "CannonFire.wav");
	assert(TurretSoundSE_[ENEMY_TURRET_SE_ATTACK] >= 0);

}

void TurretEnemy::Update()
{
    if (Timer_ > 0)
    {
       Timer_--;
    }
   
    if (Timer_ == 0)
    {
        XMFLOAT3 cannonTopPos = Model::GetBonePosition(hModel_, "CannonPos");
        Bullet* pBullet = Instantiate<Bullet>(this->GetParent()->GetParent());
        cannonTopPos.z -= 0.5;
        pBullet->SetPosition(cannonTopPos);

		Audio::Play(TurretSoundSE_[ENEMY_TURRET_SE_ATTACK]);

        Timer_ = NextBullletCoolDown;
    }
}

void TurretEnemy::Draw()
{
    Model::SetTransform(hModel_, transform_);
    Model::Draw(hModel_);

}

void TurretEnemy::Release()
{
}

void TurretEnemy::OnCollision(GameObject* parent)
{
    if (parent->GetObjectName() == "PlayerBlock")
    {
        KillMe();
    }
}
