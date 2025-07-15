#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// TitleScene
/// </summary>
class TitleScene : public GameObject
{
	int hBackTitleImage_;//背景　
	int hBlackScreen_;   //簡単な暗転
	int hTitleSound_;    //音楽
	int hStartSound_;    //開始を押されたらSEを鳴らす

	bool IsPlaySE_;// SEが鳴っているかどうか
public:
	
	TitleScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;
	
	void Release() override;
};