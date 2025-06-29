#pragma once
#include "Engine/GameObject.h"
#include "Shadow.h"
#include <Vector>

using std::vector;
//�ړ��𔻒肷�鏈��
enum MOVE_GHOST_METHOD
{
    CAN_MOVE_TURN,   //�W�����v��
    CANT_TURN,       //�W�����v�s��
};

class Ghost :public GameObject
{
    int hModel_;
    const int MAX_RANGE = 9;

    float floatingTime_      = 0.0f;      // �o�ߎ���
    float floatingAmplitude_ = 0.01f; // �㉺�̐U�ꕝ
    float floatingSpeed_     = 0.5f;     // �U���̑���


    /// <summary>
/// �ړ��\�����肷�鏈��
/// </summary>
/// <param name="pos">�ʒu</param>
/// <returns></returns>
    MOVE_GHOST_METHOD GhostCanMoveTo(const XMFLOAT3& pos);

    void EnemyMoveMent();
public:

   
    Ghost(GameObject* parent);
    
    ~Ghost();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

    float GhostDirection;

    XMVECTOR Move = XMVectorZero();
	
};

