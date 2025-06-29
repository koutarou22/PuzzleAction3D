#pragma once
#include "Engine/GameObject.h"
class LoadScene :public GameObject
{
	int hImage_;
	int hBlackScreen_;//簡単な暗転

	int StageImage_;//あとで数値はいじれるようにする事

	int hStartSound_; //開始を押されたらSEを鳴らす

	int TimeFlame_ = 60;//時間差でSceneを移動させる

	bool isNextScene = false;
public:

	LoadScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;


};

