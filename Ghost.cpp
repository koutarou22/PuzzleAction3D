#include "Ghost.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Player.h"
#include "PlayerBlock.h"
#include "Engine/SceneManager.h"
#include "Remain.h"
#include "Stage.h"

namespace
{
    const int GRID_WIDTH = 10;
    const int GRID_HEIGHT = 10;
    const int MAX_STAGE_HEIGHT = 10;
    const int GRID_OFFSET_X = 5;
    const int GRID_OFFSET_Z = 4;

    float ENEMY_TURN = 180;

    XMFLOAT3 SCALE = { 1.5,1.5,1.5 };
    const float INITIALIZE_ROTATE_Y = -90;

    const int STAGEBLOCK = 5;

	float Direction = 0.05f; // ゴーストの移動速度

	XMFLOAT3 CollisionSize = { 0.5f, 0.5f, 0.5f };
}


MOVE_GHOST_METHOD Ghost::GhostCanMoveTo(const XMFLOAT3& pos)
{
    int gx = static_cast<int>(pos.x + GRID_OFFSET_X);
    int gy = static_cast<int>(GRID_OFFSET_Z - pos.z);
    int gz = static_cast<int>(pos.y);

    //画面外にいかない処理
    if (gx < 0 || gx >= GRID_WIDTH  || gy < 0 || gy >= GRID_HEIGHT || gz < 0 || gz >= MAX_STAGE_HEIGHT)
    {
        return CAN_MOVE_TURN;
    }

    auto* stage = static_cast<Stage*>(FindObject("Stage"));
    auto& grid = stage->GetStageGrid();

    int current = grid[gz][GRID_HEIGHT - 1 - gy][gx];

    //Stageに接触したら反転させる
    if (current == STAGEBLOCK && gz > 0)
    {
        return CAN_MOVE_TURN;
    }

  return CANT_TURN;
}


Ghost::Ghost(GameObject* parent) : GameObject(parent, "Ghost"),GhostDirection(Direction)
{
}

Ghost::~Ghost()
{
}

void Ghost::Initialize()
{
	hGhostModel_ = Model::Load("Model//Ghostlow.fbx");
	assert(hGhostModel_ >= 0);

	transform_.rotate_.y = INITIALIZE_ROTATE_Y;

    transform_.scale_ = { SCALE };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { CollisionSize });
	AddCollider(collision);

}

void Ghost::Update()
{
    XMFLOAT3 next = transform_.position_;
    next.x += GhostDirection;

    transform_.position_ = next;

    // 移動可否をチェック
    MOVE_GHOST_METHOD result = GhostCanMoveTo(next);

    if (result == CAN_MOVE_TURN)
    {
        GhostDirection = -GhostDirection;
        transform_.rotate_.y += ENEMY_TURN;
    }

}

void Ghost::Draw()
{
	Model::SetTransform(hGhostModel_, transform_);
	Model::Draw(hGhostModel_);
}

void Ghost::Release()
{
}

void Ghost::OnCollision(GameObject* parent)
{
    if (parent->GetObjectName() == "Player")
    {
        Player* pPlayer = (Player*)FindObject("Player");

        if (pPlayer != nullptr)
        {
            Debug::Log("エネミーとプレイヤーが接触した", true);
            pPlayer->SetHitEnmeyFlag(true);
        }
    }
}

void Ghost::GetReflectPosition()
{
    //当たったら反転処理
    GhostDirection = -GhostDirection;
    transform_.rotate_.y += ENEMY_TURN;
}
