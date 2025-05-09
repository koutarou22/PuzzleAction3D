#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// TitleScene
/// </summary>
class TitleScene : public GameObject
{
	int hImage_;

	int hTitleSound_;//音楽
	int hStartSound_; //開始を押されたらSEを鳴らす
public:
	
	TitleScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;
	
	void Release() override;
};