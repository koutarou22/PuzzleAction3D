#pragma once
#include "Engine/GameObject.h"
class LoadScene :public GameObject
{
	int hImage_;
	int hBlackScreen_;//�ȒP�ȈÓ]

	int StageImage_;//���ƂŐ��l�͂������悤�ɂ��鎖

	int hStartSound_; //�J�n�������ꂽ��SE��炷

	int TimeFlame_ = 60;//���ԍ���Scene���ړ�������

	bool isNextScene = false;
public:

	LoadScene(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;


};

