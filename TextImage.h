#pragma once
#include <string>
#include "Engine/GameObject.h"


using std::string;


enum TEXT_IMAGE_TYPE
{
    NO_KEY_TEXT,      //カギ(クリア条件)が未入手で、目的を伝える役割
    GET_KEY_TEXT,     //カギを取得し、ゴールへ向かうことを伝える役割
    STAGE_CREAR_TEXT, //クリアした事を伝える役割
    MAX_TEXT,         
};

/// <summary>
///Clear条件を教えてくれる文章UI
/// </summary>
class TextImage : public GameObject
{
	int hTextImage_[MAX_TEXT];  //鍵の取得状態の画像を格納する配列

    float posX_, posY_, posZ_;  //初期位置

    bool GetChangeImageFlag_;   //鍵を持ってるか持ってないか判定するフラグ

	bool GetClearFlag_;         //クリア条件を満たしているかどうかのフラグ

	bool IsGetKey_;             //プレイヤークラスから鍵を持っているかどうかのフラグを受け取るための変数

	string ImagePath_;          //画像のパス
	string ImageName_[MAX_TEXT];//画像の名前

public:
    TextImage(GameObject* parent);

    ~TextImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

