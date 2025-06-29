#include "PlayerBlock.h"
#include "Engine/Model.h"
#include "Engine/Collider.h"
#include "Engine/Debug.h"
#include "Engine/Input.h"
#include "Player.h"
#include "Engine/VFX.h"

namespace
{
    constexpr float INITIAL_SCALE = 0.1f;
    constexpr float FINAL_SCALE = 1.0f;
    constexpr float SCALE_STEP = 0.02f;
    constexpr float ROTATE_SPEED_Y = 2.0f;
    constexpr float COLLIDER_SIZE = 1.0f;
}

PlayerBlock::PlayerBlock(GameObject* parent) : GameObject(parent, "PlayerBlock")
{
    hPlayerBlockModel_ = Model::Load("BoxBrick.fbx");
    assert(hPlayerBlockModel_ >= 0);
}

PlayerBlock::~PlayerBlock() {}

void PlayerBlock::Initialize()
{
    transform_.scale_ = { INITIAL_SCALE, INITIAL_SCALE, INITIAL_SCALE };

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
    // ‰ñ“]
    transform_.rotate_.y += ROTATE_SPEED_Y;

    // Šg‘å
    transform_.scale_.x += SCALE_STEP;
    transform_.scale_.y += SCALE_STEP;
    transform_.scale_.z += SCALE_STEP;

    // ˆê’èƒTƒCƒY‚É’B‚µ‚½‚çŒÅ’è•‰ñ“]’âŽ~
    if (transform_.scale_.x > FINAL_SCALE)
    {
        transform_.scale_ = { FINAL_SCALE, FINAL_SCALE, FINAL_SCALE };
        transform_.rotate_.y = 0.0f;
    }
}

void PlayerBlock::Draw()
{
    Model::SetTransform(hPlayerBlockModel_, transform_);
    Model::Draw(hPlayerBlockModel_);
}

void PlayerBlock::Release() {}

void PlayerBlock::OnCollision(GameObject* parent) {}
