#pragma once
#include "Engine/GameObject.h"

/// <summary>
///Clear�����������Ă����UI
/// </summary>
class TextImage : public GameObject
{
	int hTextImage_[3];//���̎擾��Ԃ̉摜���i�[����z��

    float posX, posY, posZ;//�����ʒu

    bool GetChangeImageFlag;//���������Ă邩�����ĂȂ������肷��flag

	bool GetClearFlag;//�N���A�����𖞂����Ă��邩�ǂ����̃t���O

	bool IsGetKey;//�v���C���[�N���X���献�������Ă��邩�ǂ����̃t���O���󂯎�邽�߂̕ϐ�

public:
    TextImage(GameObject* parent);

    ~TextImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

