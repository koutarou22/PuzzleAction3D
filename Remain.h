#pragma once
#include "Engine/GameObject.h"
#include <string>

using std::string;


enum REMAIN_UI
{
    REMAIN_UI_TYPE    ,// �c�@�A�C�R��UI ��\��
    REMAIN_NUMBER_TYPE,//    �c�@��UI    ��\��
    REMAIN_MAX,       
};


/// <summary>
/// �v���C���[�̎c�@UI�N���X
/// </summary>
class Remain :
    public GameObject
{
private:
    //�X�q�̎c�@�摜
	int hRemainImage_[REMAIN_MAX];

    //���l�̈ʒu
    float posX1;

    //�����ʒu
    float posX, posY, posZ;

    //�c�@��
	int LIFE_;

    //�c�@UI�̓����x
	float RemaindueAlpha_;

    //����UI�p�̈ʒu���
    Transform Number_transform_;

    string ImagePath_;//�摜�̃p�X
    string ImageName_[REMAIN_MAX];//�摜�̖��O
public:
    Remain(GameObject* parent);

    ~Remain();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
    
    //�c�@���𑼃N���X�Ŏ擾�ł���
    int GetLife() { return LIFE_; }

    //���N���X�ł����L������p
    void SetLife(int LifeNum) { LIFE_ = LifeNum; }

    //���N���X�ł��ʒu��ύX�ł���p
    void SetPosition(float x, float y, float z);

    //�c�@UI�̓����x���擾
    void SetAlpha(float alpha);
};

