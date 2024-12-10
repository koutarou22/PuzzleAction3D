#pragma once
#include "Engine/GameObject.h"
/// <summary>
/// カメラを操作するクラス
/// </summary>
class CameraController :public GameObject
{
    Transform transform_;
    XMVECTOR target_;
    XMVECTOR position_;
public:
    //コンストラクタ
    CameraController(GameObject* parent);

    //デストラクタ
    ~CameraController();

    //初期化
    void Initialize();

    //更新
    void Update();

    //描画
    void Draw();

    //開放
    void Release();
};

