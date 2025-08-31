#pragma once
#include "Engine/GameObject.h"
#include <string>

using std::string;


enum REMAIN_UI
{
    REMAIN_UI_TYPE    ,// 残機アイコンUI を表示
    REMAIN_NUMBER_TYPE,//    残機数UI    を表示
    REMAIN_MAX,       
};


/// <summary>
/// プレイヤーの残機UIクラス
/// </summary>
class Remain :
    public GameObject
{
private:
    //帽子の残機画像
	int hRemainImage_[REMAIN_MAX];

    //数値の位置
    float posX1;

    //初期位置
    float posX, posY, posZ;

    //残機数
	int LIFE_;

    //残機UIの透明度
	float RemaindueAlpha_;

    //数字UI用の位置情報
    Transform Number_transform_;

    string ImagePath_;//画像のパス
    string ImageName_[REMAIN_MAX];//画像の名前
public:
    Remain(GameObject* parent);

    ~Remain();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
    
    //残機数を他クラスで取得できる
    int GetLife() { return LIFE_; }

    //他クラスでも共有させる用
    void SetLife(int LifeNum) { LIFE_ = LifeNum; }

    //他クラスでも位置を変更できる用
    void SetPosition(float x, float y, float z);

    //残機UIの透明度を取得
    void SetAlpha(float alpha);
};

