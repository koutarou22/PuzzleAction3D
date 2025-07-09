#pragma once
#include "Engine/GameObject.h"
#include <string>

#include "Engine/VFX.h"

using std::string;

enum DOOR_SE
{
    DOOR_OPEN_SE,//開いた音
    DOOR_MAX,//最大

};

/// <summary>
/// クリア条件用の扉
/// </summary>
class GoalDoor: public GameObject
{

    EmitterData Door;

    int hDoorModel_; 
    int hOpenLight_;
    bool GoalFlag_;//プレイヤークラスのClearFlagを保存する用

    float posX, posY, posZ;

    //回転が完了したかどうか
    bool isRotationComplete;

    //サウンド

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

