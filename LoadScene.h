#pragma once
#include "Engine/GameObject.h"
class LoadScene :public GameObject
{
	int hImage_;

	int hLoadSound_;//‰¹Šy
	int hStartSound_; //ŠJŽn‚ð‰Ÿ‚³‚ê‚½‚çSE‚ð–Â‚ç‚·
public:

	LoadScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
};

