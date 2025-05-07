#pragma once
#include "Engine//GameObject.h"

/// <summary>
/// PlayScene
/// </summary>
class PlayScene : public GameObject
{

    int hBackImage_;

    int hPlaySound_; //ŠJŽn‚ð‰Ÿ‚³‚ê‚½‚çSE‚ð–Â‚ç‚·
public:
    PlayScene(GameObject* parent);
    ~PlayScene();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

