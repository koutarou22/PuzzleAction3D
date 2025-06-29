#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// GameOverScene
/// </summary>
class GameOverScene : public GameObject
{
	int hImage_;
	int hGameOverBGM_;//GameOverBGM‚ð“o˜^
public:
	
	GameOverScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
};