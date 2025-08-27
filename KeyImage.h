#pragma once
#include "Engine/GameObject.h"
#include <string>

using std::string;

enum KEY_IMAGE_TYPE
{
    KEY_IMAGE_NO_KEY = 0, //���������Ă��Ȃ����
    KEY_IMAGE_KEY,        //���������Ă�����
	KEY_IMAGE_MAX,        //���̏��
};

/// <summary>
/// ������肵�����킩��UI
/// </summary>
class KeyImage : public GameObject
{
	int hKeyImage_[KEY_IMAGE_MAX];  //���̎擾��Ԃ̉摜���i�[����z��
    float posX_, posY_, posZ_;       //�����ʒu
    bool GetChangeImageFlag_;        //���������Ă邩�����ĂȂ������肷��flag


	string ImagePath_;               //�摜�̃p�X
	string ImageName_[KEY_IMAGE_MAX];//�摜�̖��O


public:
    KeyImage(GameObject* parent);
  
    ~KeyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

