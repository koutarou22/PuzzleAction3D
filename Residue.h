#pragma once
#include "Engine/GameObject.h"
/// <summary>
/// プレイヤーの残基クラス
/// </summary>
class Residue :
    public GameObject
{
    int hResidueImage_;
    int hNum_; //残基【数値】
    float posX1;//数値の位置
    float posX, posY, posZ;//初期位置

    Transform trs;
public:
    Residue(GameObject* parent);

    ~Residue();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

