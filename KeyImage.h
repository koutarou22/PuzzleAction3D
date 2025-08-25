#pragma once
#include "Engine/GameObject.h"

enum KEY_IMAGE_TYPE
{
    KEY_IMAGE_NO_KEY = 0, //���������Ă��Ȃ����
    KEY_IMAGE_KEY,      //���������Ă�����
	KEY_IMAGE_MAX,      //���̏��
};

/// <summary>
/// ������肵�����킩��UI
/// </summary>
class KeyImage : public GameObject
{
	int hKeyImage_[KEY_IMAGE_MAX];//���̎擾��Ԃ̉摜���i�[����z��
    float posX, posY, posZ;//�����ʒu
    bool GetChangeImageFlag;//���������Ă邩�����ĂȂ������肷��flag
public:
    KeyImage(GameObject* parent);
  
    ~KeyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

