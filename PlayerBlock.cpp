#include "PlayerBlock.h"
#include "Engine/Model.h"
#include "Engine/Collider.h"
#include "Engine/Debug.h"
#include "Engine/Input.h"
#include "Player.h"
#include "Engine/VFX.h"
#include "Engine/Audio.h"
#include "TurretEnemy.h"
#include "Ghost.h"

namespace
{
    constexpr float INITIAL_SCALE  = 0.1f;
    constexpr float FINAL_SCALE    = 1.0f;
    constexpr float SCALE_STEP     = 0.02f;
    constexpr float ROTATE_SPEED_Y = 2.0f;
    constexpr float COLLIDER_SIZE  = 1.0f;
}

PlayerBlock::PlayerBlock(GameObject* parent) : GameObject(parent, "PlayerBlock")
{
    hPlayerBlockModel_ = Model::Load("Model//BoxBrick.fbx");
    assert(hPlayerBlockModel_ >= 0);

	// �v���C���[�u���b�N��SE��o�^
    
	//�ق��̃I�u�W�F�N�g�ƐڐG�����Ƃ��̔��ˉ�
	hPlayerBlockSE_[PLAYER_BLOCK_SE_REFLECT] = Audio::Load(PlayerBlockPath + "Reflect.wav");
	assert(hPlayerBlockSE_[PLAYER_BLOCK_SE_REFLECT] >= 0);
}

PlayerBlock::~PlayerBlock() {}

void PlayerBlock::Initialize()
{
    transform_.scale_ = { INITIAL_SCALE, INITIAL_SCALE, INITIAL_SCALE };
    isAnimation_ = true;

    BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { COLLIDER_SIZE, COLLIDER_SIZE, COLLIDER_SIZE });
    AddCollider(collision);
}

void PlayerBlock::Update()
{
    Player* pPlayer = static_cast<Player*>(FindObject("Player"));
    if (pPlayer)
    {
        AnimateBlock();
    }
}

void PlayerBlock::AnimateBlock()
{
    // ��]
    transform_.rotate_.y += ROTATE_SPEED_Y;

    // �g��
    transform_.scale_.x += SCALE_STEP;
    transform_.scale_.y += SCALE_STEP;
    transform_.scale_.z += SCALE_STEP;


    // ���T�C�Y�ɒB������Œ聕��]��~
    if (transform_.scale_.x > FINAL_SCALE)
    {
        transform_.scale_ = { FINAL_SCALE, FINAL_SCALE, FINAL_SCALE };
        transform_.rotate_.y = 0.0f;
        isAnimation_ = false; // �A�j���[�V�����I��
    }
}

void PlayerBlock::Draw()
{
    Model::SetTransform(hPlayerBlockModel_, transform_);
    Model::Draw(hPlayerBlockModel_);
}

void PlayerBlock::Release() {}

void PlayerBlock::OnCollision(GameObject* parent) {

	TurretEnemy* pTurretEnemy = (TurretEnemy*)FindObject("TurretEnemy");
	Ghost* pGhost = (Ghost*)FindObject("Ghost");

	if (pTurretEnemy != nullptr)
	{
		// TurretEnemy�ɐڐG�����ꍇ�APlayerBlock���폜
		if (parent->GetObjectName() == "TurretEnemy")
		{
			Audio::Play(hPlayerBlockSE_[PLAYER_BLOCK_SE_REFLECT]);
			pTurretEnemy->KillMe();

            EmitterData data;
            data.textureFileName = "PaticleAssets//flashA_R.png";
            data.position = XMFLOAT3(pTurretEnemy->GetPosition());
            data.positionRnd = XMFLOAT3(0.1, 0, 0.1);
            data.delay = 0;
            data.speed = 0.001f;
            data.number = 1;
            data.lifeTime = 10;
            data.gravity = -0.002f;
            data.direction = XMFLOAT3(0, 0, 0);
            data.size = XMFLOAT2(1.5, 1.5);
            VFX::Start(data);
		}
	}
    
    if (pGhost != nullptr)
    {
        //Ghost�ɐڐG
        if (parent->GetObjectName() == "Ghost")
        {
            Audio::Play(hPlayerBlockSE_[PLAYER_BLOCK_SE_REFLECT]);
            pGhost->GetReflectPosition(); // Ghost�̈ʒu�𔽎ˈʒu�ɍX�V

                EmitterData data;
                data.textureFileName = "PaticleAssets//flashB_B.png";
                data.position = XMFLOAT3(pGhost->GetPosition());
                data.positionRnd = XMFLOAT3(0.1, 0, 0.1);
                data.delay = 0;
                data.speed = 0.001f;
                data.number = 1;
                data.lifeTime = 10;
                data.gravity = -0.002f;
                data.direction = XMFLOAT3(0, 0, 0);
                data.size = XMFLOAT2(1.5, 1.5);
                VFX::Start(data);
        }
    }
  
}
