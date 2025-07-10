#pragma once
#include "Engine/GameObject.h"
/// <summary>
/// �v���C���[�̎c��N���X
/// </summary>
class Residue :
    public GameObject
{
    //�X�q�̎c��摜
	int hResidueImage_;

    //�c��y���l�z
    int hNum_; 

    //���l�̈ʒu
    float posX1;

    //�����ʒu
    float posX, posY, posZ;

    //�c�@��
	int LIFE_;

    //�c��UI�̓����x
	float ResiduedueAlpha_;

    Transform trs;
public:
    Residue(GameObject* parent);

    ~Residue();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
    
    //�c�@����Ԃ�
    int GetLife() { return LIFE_; }

    //���N���X�ł����L������悤
    void SetLife(int LifeNum) { LIFE_ = LifeNum; }

    void SetPosition(float x, float y, float z);

    //�c�@UI�̓����x���擾
    void SetAlpha(float alpha);


};

