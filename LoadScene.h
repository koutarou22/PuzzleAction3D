#pragma once
#include "Engine/GameObject.h"
class LoadScene :public GameObject
{
	int hImage_;

	int hLoadSound_;//���y
	int hStartSound_; //�J�n�������ꂽ��SE��炷
public:

	LoadScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;
};

