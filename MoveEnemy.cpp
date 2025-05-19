#include "MoveEnemy.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Player.h"
#include "PlayerBlock.h"
#include "Engine/SceneManager.h"
#include "Residue.h"

MoveEnemy::MoveEnemy(GameObject* parent) : GameObject(parent, "MoveEnemy")
{
    MoveEnemyDirection = 0.05f;
}

MoveEnemy::~MoveEnemy()
{
}

void MoveEnemy::Initialize()
{
	hModel_ = Model::Load("Ghostlow.fbx");
	assert(hModel_ >= 0);


	transform_.rotate_.y = -90.0f;
    //transform_.rotate_.x = 90.0f;
	
    transform_.position_ = { 5.0,2.0,3.0 };
    transform_.scale_ = { 1.5,1.5,1.5 };

	BoxCollider* collision = new BoxCollider({ 0, 0, 0 }, { 0.5, 0.5, 0.5 });
	AddCollider(collision);
}

void MoveEnemy::Update()
{

    transform_.position_.x += MoveEnemyDirection;
  
    CanMoveRenge();
}

void MoveEnemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void MoveEnemy::Release()
{
}
void MoveEnemy::OnCollision(GameObject* parent)
{
    if (parent->GetObjectName() == "Player")
    {
        Player* pPlayer = (Player*)FindObject("Player");
        SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

        if (pPlayer != nullptr && pSceneManager != nullptr)
        {
            Debug::Log("エネミーとプレイヤーが接触した", true);
            pPlayer->SetHitEnmeyFlag(true);

            // 残機を減らす
            int currentResidue = pSceneManager->GetPlayerResidue();
            if (currentResidue > 0)
            {
                pSceneManager->SetPlayerResidue(currentResidue - 1); // 残機を1減らす
            }

            // **残機が1以上なら LoadScene に戻す**
            if (pSceneManager->GetPlayerResidue() > 0)
            {
                pSceneManager->ChangeScene(SCENE_ID_LOAD);
            }
            // **残機が0なら GAMEOVER に移動**
            else
            {
                pSceneManager->ChangeScene(SCENE_ID_GAMEOVER);
            }
        }
    }

    if (parent->GetObjectName() == "PlayerBlock")
    {
        PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");
        if (pBlock != nullptr)
        {
            bool MoveBlock = pBlock->GetMoveHit();
            if (MoveBlock)
            {
                KillMe();
            }
            else
            {
                MoveEnemyDirection = -MoveEnemyDirection;
                transform_.rotate_.y += 180.0f;
            }
        }
    }
}


void MoveEnemy::CanMoveRenge()
{
    // X軸の範囲チェック
    if (transform_.position_.x < 0)
    {
        transform_.position_.x = 0; 
        MoveEnemyDirection = -MoveEnemyDirection;
        transform_.rotate_.y += 180.0f; 
    }
    else if (transform_.position_.x > MAX_RANGE)
    {
        transform_.position_.x = MAX_RANGE; 
        MoveEnemyDirection = -MoveEnemyDirection; 
        transform_.rotate_.y += 180.0f; 
    }

   
    if (transform_.position_.z < 0)
    {
        transform_.position_.z = 0;
        MoveEnemyDirection = -MoveEnemyDirection; 
        transform_.rotate_.y += 180.0f;
    }
    else if (transform_.position_.z > MAX_RANGE)
    {
        transform_.position_.z = MAX_RANGE;
        MoveEnemyDirection = -MoveEnemyDirection;
        transform_.rotate_.y += 180.0f;
    }
}

void MoveEnemy::AddShadow(XMFLOAT3 pos)
{
    if (shadows.size() < 1)
    {
        Shadow* pShadow = Instantiate<Shadow>(this);
        pShadow->SetPosition(pos);
        shadows.push_back(pShadow);
    }
}



