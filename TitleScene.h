#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// TitleScene
/// </summary>
class TitleScene : public GameObject
{
	int hImage_;

	int hTitleSound_;//‰¹Šy
	int hStartSound_; //ŠJŽn‚ð‰Ÿ‚³‚ê‚½‚çSE‚ð–Â‚ç‚·
public:
	
	TitleScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;
	
	void Release() override;
};