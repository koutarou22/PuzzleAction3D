#pragma once
#include "Engine/GameObject.h"

//振動する値の管理
namespace
{
	//アニメーション用定数
	const float MOVE_AMPLITUDE = 0.1f;//振れ幅
	const float MOVE_FREQUENCY_SPEED = 1.0f;// 揺れの速さを調整
	const float DELTATIME = 1.0f / 60.0f;

	//初期角度の指定
	const float OBJECT_ROTATE_X = 30;
	const float OBJECT_ROTATE_Y = 0.3f;

	//オブジェクトの最初の位置
	const float FAST_POSITION_X = 9.0f;
	const float FAST_POSITION_Y = 6.5f;
	const float FAST_POSITION_Z = 9.0f;

    //オブジェクトの最初のサイズ
    const float FAST_SCALE_X = 0.5f;
    const float FAST_SCALE_Y = 0.5f;
    const float FAST_SCALE_Z = 0.5f;

}
/// <summary>
/// 残基をアップさせるための処理
/// </summary>
class ResidueItem: public GameObject
{
    int hResidue_;

    //初期位置
    float posX, posY, posZ;

    //簡単な縦揺れのアニメーション用変数

    float totalTime_;  //合計の時間を格納する用
    float deltaTime;   //1フレームの時間(秒で換算予定)

    float amplitude_;  // 振動の振れ幅
    float frequency_;  // 揺れの速度
    float offsetY_;    //最終的な計算を格納する用

public:

    ResidueItem(GameObject* parent);

    ~ResidueItem();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

    void ResidueVibrationAnimation();

    void SetBasePosition(float x, float y, float z);
};

