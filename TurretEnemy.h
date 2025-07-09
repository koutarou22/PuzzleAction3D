#pragma once
#include "Engine/GameObject.h"

enum ENMEY_TURRET_SE
{
	ENEMY_TURRET_SE_ATTACK = 1,
	ENEMY_TURRET_SE_MAX,
};
class TurretEnemy :public GameObject
{
    int hModel_;

    int Timer_;
    bool isAttack_;
    const int MAX_RANGE = 9;

    //ƒTƒEƒ“ƒh
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

