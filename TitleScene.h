#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// TitleScene
/// </summary>
class TitleScene : public GameObject
{
	int hBackTitleImage_;//�w�i�@
	int hBlackScreen_;   //�ȒP�ȈÓ]
	int hTitleSound_;    //���y

public:
	
	TitleScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;
	
	void Release() override;
};