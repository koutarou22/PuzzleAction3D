#pragma once
#include "Engine/GameObject.h"

enum ENMEY_TURRET_SE
{
	ENEMY_TURRET_SE_ATTACK = 1,//発射音
	ENEMY_TURRET_SE_MAX,
};
class TurretEnemy :public GameObject
{
    //タレットのモデルを入れるハンドル
    int hTurretModel_;

    //次の弾丸を発射するまでのタイマー
    int BulletTimer_;

    //弾丸が届く最高距離
    const int MAX_RANGE = 9;

    //弾丸を発射してから次の弾丸を発射するまでのクールダウン時間
    int NextBullletCoolDown = 300;


    //サウンド
	int TurretSoundSE_[ENEMY_TURRET_SE_MAX];
public:

    TurretEnemy(GameObject* parent);

    ~TurretEnemy();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;
};

