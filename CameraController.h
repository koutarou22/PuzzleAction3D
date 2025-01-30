#pragma once
#include "Engine/GameObject.h"


/// <summary>
/// ƒJƒƒ‰‚ğ‘€ì‚·‚éƒNƒ‰ƒX
/// </summary>
class CameraController :public GameObject
{
    Transform transform_;
    XMVECTOR target_;
    XMVECTOR position_;

    int CamState_;
public:
  
    CameraController(GameObject* parent);

    ~CameraController();

    void Initialize();

    void Update();

    void Draw();

    void Release();

    void DefaultComera();
};

