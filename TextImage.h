#pragma once
#include "Engine/GameObject.h"

/// <summary>
///Clear条件を教えてくれるUI
/// </summary>
class TextImage : public GameObject
{
	int hTextImage_[3];//鍵の取得状態の画像を格納する配列

    float posX, posY, posZ;//初期位置

    bool GetChangeImageFlag;//鍵を持ってるか持ってないか判定するflag

	bool GetClearFlag;//クリア条件を満たしているかどうかのフラグ

	bool IsGetKey;//プレイヤークラスから鍵を持っているかどうかのフラグを受け取るための変数

public:
    TextImage(GameObject* parent);

    ~TextImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

