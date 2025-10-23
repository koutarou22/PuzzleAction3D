#pragma once
#include "Engine/GameObject.h"
#include "Shadow.h"
#include <Vector>

using std::vector;
//ˆÚ“®‚ğ”»’è‚·‚éˆ—
enum MOVE_GHOST_METHOD
{
    CAN_MOVE_TURN,   //•ûŒü“]Š·‰Â
    CANT_TURN,       //•ûŒü“]Š·•s‰Â
};

class Ghost :public GameObject
{
    int hGhostModel_;
    /// <summary>
    /// ˆÚ“®‰Â”\‚©”»’è‚·‚éˆ—
    /// </summary>
    /// <param name="pos">ˆÊ’u</param>
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

	void GetReflectPosition();

    float GhostDirection;

    XMVECTOR Move = XMVectorZero();
	
};