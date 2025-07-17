#include "TurretEnemy.h"
#include "Player.h"
#include "Engine/Debug.h"
#include "Engine/Model.h"
#include "Bullet.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "PlayerBlock.h"
#include "Engine/Audio.h"

TurretEnemy::TurretEnemy(GameObject* parent) : GameObject(parent, "TurretEnemy")
{
    BulletTimer_ = NextBullletCoolDown;
}

TurretEnemy::~TurretEnemy()
{
}

void TurretEnemy::Initialize()
{
    hTurretModel_ = Model::Load("Cannon.fbx");
    assert(hTurretModel_ >= 0);

    //当たり判定のサイズ
    BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 1,1,1 });
    AddCollider(collision);

    //サウンド登録
	string TurretPath = "Sound//SE//EnemySE//Turret//";

	TurretSoundSE_[ENEMY_TURRET_SE_ATTACK] = Audio::Load(TurretPath + "CannonFire.wav");
	assert(TurretSoundSE_[ENEMY_TURRET_SE_ATTACK] >= 0);

}

void TurretEnemy::Update()
{
	//タイマー減少処理
    if (BulletTimer_ > 0)
    {
       BulletTimer_--;
    }
   

    if (BulletTimer_ == 0)
    {
        //砲台の位置を取得し、その位置からBULLETを呼び出す
        XMFLOAT3 cannonTopPos = Model::GetBonePosition(hTurretModel_, "CannonPos");
        Bullet* pBullet = Instantiate<Bullet>(this->GetParent()->GetParent());
        cannonTopPos.z -= 0.5;
        pBullet->SetPosition(cannonTopPos);

        //弾丸発射音を再生
		Audio::Play(TurretSoundSE_[ENEMY_TURRET_SE_ATTACK]);

        //タイマーの値を元に戻す
        BulletTimer_ = NextBullletCoolDown;
    }
}

void TurretEnemy::Draw()
{
    Model::SetTransform(hTurretModel_, transform_);
    Model::Draw(hTurretModel_);
}

void TurretEnemy::Release()
{
}

void TurretEnemy::OnCollision(GameObject* parent)
{
   
}
