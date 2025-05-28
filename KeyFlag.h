#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// �����J�������p
/// </summary>
class KeyFlag : public GameObject
{
    int hModel_;
    float posX,posY,posZ;//�����ʒu

    int Random;
    XMFLOAT3 KeyPos[2][2][2];

    //�ȒP�ȏc�h��̃A�j���[�V����
    float totalTime_;//���v�̎��Ԃ��i�[����p
    float deltaTime;//1�t���[���̎���(�b�Ŋ��Z�\��)
    

    float amplitude_;  // �U���̐U�ꕝ
    float frequency_; // �h��̑��x
    float yOffset_;//�ŏI�I�Ȍv�Z���i�[����p

    bool isGetKey_;//������肵����

public:
   
    KeyFlag(GameObject* parent);
    
    ~KeyFlag();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 GetPosition() { return transform_.position_; }
    XMFLOAT3 GetRotation() { return transform_.rotate_; }

    void OnCollision(GameObject* parent) override;

   

    void VibrationAnimation();
};

