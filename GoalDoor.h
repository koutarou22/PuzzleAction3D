#pragma once
#include "Engine/GameObject.h"
#include <string>

#include "Engine/VFX.h"

using std::string;

enum DOOR_SE
{
    DOOR_OPEN_SE,//�J������
    DOOR_MAX,//�ő�

};

/// <summary>
/// �N���A�����p�̔�
/// </summary>
class GoalDoor: public GameObject
{

    EmitterData Door;

    int hDoorModel_; 
    int hOpenLight_;
    bool GoalFlag_;//�v���C���[�N���X��ClearFlag��ۑ�����p

    float posX, posY, posZ;

    //��]�������������ǂ���
    bool isRotationComplete;

    //�T�E���h

    string DoorPath = "Sound/SE/ClearConditionsSE/";
    int hDoorSound_[DOOR_MAX];

public:

    GoalDoor(GameObject* parent);

    ~GoalDoor();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

    void SetHitGoalFlag(bool isHitPlayer) { GoalFlag_ = isHitPlayer; }


};

