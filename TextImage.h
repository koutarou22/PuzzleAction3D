#pragma once
#include <string>
#include "Engine/GameObject.h"


using std::string;


enum TEXT_IMAGE_TYPE
{
    NO_KEY_TEXT,      //�J�M(�N���A����)��������ŁA�ړI��`�������
    GET_KEY_TEXT,     //�J�M���擾���A�S�[���֌��������Ƃ�`�������
    STAGE_CREAR_TEXT, //�N���A��������`�������
    MAX_TEXT,         
};

/// <summary>
///Clear�����������Ă���镶��UI
/// </summary>
class TextImage : public GameObject
{
	int hTextImage_[MAX_TEXT];  //���̎擾��Ԃ̉摜���i�[����z��

    float posX_, posY_, posZ_;  //�����ʒu

    bool GetChangeImageFlag_;   //���������Ă邩�����ĂȂ������肷��t���O

	bool GetClearFlag_;         //�N���A�����𖞂����Ă��邩�ǂ����̃t���O

	bool IsGetKey_;             //�v���C���[�N���X���献�������Ă��邩�ǂ����̃t���O���󂯎�邽�߂̕ϐ�

	string ImagePath_;          //�摜�̃p�X
	string ImageName_[MAX_TEXT];//�摜�̖��O

public:
    TextImage(GameObject* parent);

    ~TextImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

