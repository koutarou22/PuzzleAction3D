#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// �����J�������p
/// </summary>
class KeyFlag : public GameObject
{
    int hKey_;
    float posX,posY,posZ;//�����ʒu

    //�ȒP�ȏc�h��̃A�j���[�V����
    float totalTime_;//���v�̎��Ԃ��i�[����p
    float deltaTime;//1�t���[���̎���(�b�Ŋ��Z�\��)
    

    float amplitude_;  // �U���̐U�ꕝ
    float frequency_;  // �h��̑��x
    float offsetY_;    //�ŏI�I�Ȍv�Z���i�[����p

    bool isGetKey_;//������肵����

public:
   
    KeyFlag(GameObject* parent);
    
    ~KeyFlag();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    void OnCollision(GameObject* parent) override;

    void KeyVibrationAnimation();

    void SetBasePosition(float x, float y, float z);
};

