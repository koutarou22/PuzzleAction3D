#pragma once
#include"Engine//GameObject.h"
#include"Engine//Fbx.h"
#include<string>

using std::string;

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"

enum PLAYER_BLOCK_SE
{
	PLAYER_BLOCK_SE_REFLECT = 0, //���ˉ�
	PLAYER_BLOCK_SE_MAX,
};
/// <summary>
/// �v���C���[���g�p����u���b�N
/// </summary>
class PlayerBlock :public GameObject
{

	int hPlayerBlockModel_;//�v���C���[�u���b�N�̃��f���n���h��

    void AnimateBlock();//�ȒP�Ȑ���Animation

	bool isAnimation_ = false; //�A�j���[�V���������ǂ���

    int RefrectSoundHandle = -1;

	// �v���C���[�u���b�N��SE�n���h��
    int hPlayerBlockSE_[PLAYER_BLOCK_SE_MAX];
	// �v���C���[�u���b�N��SE�p�X
	string PlayerBlockPath = "Sound//SE//PlayerBlockSE//";
   
public:

    PlayerBlock(GameObject* parent);

    ~PlayerBlock();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;

    XMFLOAT3 &GetPosition() { return transform_.position_; }
    XMFLOAT3 &GetScale()    { return transform_.position_; }

    void OnCollision(GameObject* parent) override;

	bool GetIsAnimation() const { return isAnimation_; };
};

