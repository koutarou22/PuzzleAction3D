#pragma once
#include "Engine/GameObject.h"


enum LOAD_SCENE_STAGE_NUMBER
{
	LOAD_SCENE_STAGE1 = 1,
	LOAD_SCENE_STAGE2,
	LOAD_SCENE_STAGE3,
	LOAD_SCENE_STAGE4,
	LOAD_SCENE_MAX,
};


class LoadScene :public GameObject
{
	int hImage_;
	int hBlackScreen_;//簡単な暗転

	int StageNumber_;//持ってきたステージ番号取得用変数

	int StageImage_[LOAD_SCENE_MAX];//あとで数値はいじれるようにする事

	int StageNumberCurrent_;//StaggeImage_を格納

	int hStartSound_; //開始を押されたらSEを鳴らす

	int TimeFlame_ = 60;//時間差でSceneを移動させる

	bool isNextScene = false;//次のシーンに移動するかどうかのフラグ
public:

	LoadScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;


};

