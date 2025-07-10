#pragma once
#include "Engine/GameObject.h"

enum ENMEY_TURRET_SE
{
	ENEMY_TURRET_SE_ATTACK = 1,//���ˉ�
	ENEMY_TURRET_SE_MAX,
};
class TurretEnemy :public GameObject
{
    //�^���b�g�̃��f��������n���h��
    int hTurretModel_;

    //���̒e�ۂ𔭎˂���܂ł̃^�C�}�[
    int BulletTimer_;

    //�e�ۂ��͂��ō�����
    const int MAX_RANGE = 9;

    //�e�ۂ𔭎˂��Ă��玟�̒e�ۂ𔭎˂���܂ł̃N�[���_�E������
    int NextBullletCoolDown = 300;


    //�T�E���h
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

