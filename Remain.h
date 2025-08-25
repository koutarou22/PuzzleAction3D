#pragma once
#include "Engine/GameObject.h"
/// <summary>
/// プレイヤーの残機クラス
/// </summary>
class Remain :
    public GameObject
{
    //帽子の残機画像
	int hRemainImage_;

    //残機【数値】
    int hNum_; 

    //数値の位置
    float posX1;

    //初期位置
    float posX, posY, posZ;

    //残機数
	int LIFE_;

    //残機UIの透明度
	float RemaindueAlpha_;

    Transform trs;
public:
    Remain(GameObject* parent);

    ~Remain();

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

