#pragma once
#include "Engine/GameObject.h"
/// <summary>
/// プレイヤーの残基クラス
/// </summary>
class Residue :
    public GameObject
{
    //帽子の残基画像
	int hResidueImage_;

    //残基【数値】
    int hNum_; 

    //数値の位置
    float posX1;

    //初期位置
    float posX, posY, posZ;

    //残機数
	int LIFE_;

    //残基UIの透明度
	float ResiduedueAlpha_;

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
    void SetLife(int LifeNum) { LIFE_ = LifeNum; }

    void SetPosition(float x, float y, float z);

    //残機UIの透明度を取得
    void SetAlpha(float alpha);


};

