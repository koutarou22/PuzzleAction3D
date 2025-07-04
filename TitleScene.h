#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// TitleScene
/// </summary>
class TitleScene : public GameObject
{
	int hBackTitleImage_;//wi@
	int hBlackScreen_;   //ÈPÈÃ]
	int hTitleSound_;    //¹y
	int hStartSound_;    //Jnð³ê½çSEðÂç·
public:
	
	TitleScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;
	
	void Release() override;
};