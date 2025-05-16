#pragma once
#include "Engine/GameObject.h"
class LoadScene :public GameObject
{
	int hImage_;

	int hLoadSound_;//‰¹Šy
	int hStartSound_; //ŠJn‚ğ‰Ÿ‚³‚ê‚½‚çSE‚ğ–Â‚ç‚·

	int TimeFlame_ = 60;//ŠÔ·‚ÅScene‚ğˆÚ“®‚³‚¹‚é

	bool isNextScene = false;
public:

	LoadScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;


};

