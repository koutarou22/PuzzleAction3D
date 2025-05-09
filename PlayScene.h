#pragma once
#include "Engine//GameObject.h"

/// <summary>
/// PlayScene
/// </summary>
class PlayScene : public GameObject
{

    int hBackImage_;

    int hPlaySound_; //開始を押されたらSEを鳴らす
public:
    PlayScene(GameObject* parent);
    ~PlayScene();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

