#pragma once
#include "Engine/GameObject.h"
#include "Engine/SceneManager.h"

//�Ó]���鏈�����܂Ƃ܂��Ă�N���X
class Fadein : public GameObject
{
	int hBlackScreen_;   //�ȒP�ȈÓ]

	bool isFadingOut;    // �t�F�[�h�A�E�g�����ǂ���      
	bool isLoading;      // ���[�h�����ǂ���
public:

	Fadein(GameObject* parent);

	void Initialize() override;

	void Update() override;

	void Draw() override;

	void Release() override;

	/// <summary>
	/// ��ʂ��Ó]�����鏈��
	/// </summary>
	/// <param name="scene">�ύX�������V�[����</param>
	void FadeIn(SCENE_ID scene);

	//�t�F�[�h�A�E�g�̏�Ԃ𑼃N���X�ő���
	void SetFadeOut(bool fadeout) { isFadingOut = fadeout; }
	bool GetFadeOut() const { return isFadingOut; }

};

