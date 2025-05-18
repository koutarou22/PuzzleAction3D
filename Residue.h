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

    int LIFE_;

    Transform trs;
public:
    Residue(GameObject* parent);

    ~Residue();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
    
    //残機数を返す
    int GetLife() { return LIFE_; }

    //他クラスでも共有させるよう
    void SetLife(int LifeNum) { LifeNum = LIFE_; }
};

