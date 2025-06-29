#pragma once
#include "Engine/GameObject.h"
#include "Shadow.h"
#include <Vector>

using std::vector;
//移動を判定する処理
enum MOVE_GHOST_METHOD
{
    CAN_MOVE_TURN,   //ジャンプ可
    CANT_TURN,       //ジャンプ不可
};

class Ghost :public GameObject
{
    int hGhostModel_;
    /// <summary>
/// 移動可能か判定する処理
/// </summary>
/// <param name="pos">位置</param>
/// <returns></returns>
    MOVE_GHOST_METHOD GhostCanMoveTo(const XMFLOAT3& pos);

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

