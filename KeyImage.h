#pragma once
#include "Engine/GameObject.h"
#include <string>

using std::string;

enum KEY_IMAGE_TYPE
{
    KEY_IMAGE_NO_KEY = 0, //鍵を持っていない状態
    KEY_IMAGE_KEY,        //鍵を持っている状態
	KEY_IMAGE_MAX,        //鍵の状態
};

/// <summary>
/// 鍵を入手したかわかるUI
/// </summary>
class KeyImage : public GameObject
{
	int hKeyImage_[KEY_IMAGE_MAX];  //鍵の取得状態の画像を格納する配列
    float posX_, posY_, posZ_;       //初期位置
    bool GetChangeImageFlag_;        //鍵を持ってるか持ってないか判定するflag


	string ImagePath_;               //画像のパス
	string ImageName_[KEY_IMAGE_MAX];//画像の名前


public:
    KeyImage(GameObject* parent);
  
    ~KeyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

