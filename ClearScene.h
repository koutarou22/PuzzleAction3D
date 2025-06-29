#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// ClearScene
/// </summary>
class ClearScene : public GameObject
{
	int hImage_;
	int hClearBGM_;//ClearBGM‚ð“o˜^
public:
	
	ClearScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
};