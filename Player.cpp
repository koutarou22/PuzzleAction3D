#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/BoxCollider.h"
#include "Stage.h"
#include <iostream>
#include "Engine/Debug.h"
#include "PlayerBlock.h"

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "KeyFlag.h"
#include "Ladder.h"
#include "MoveEnemy.h"
#include "Engine/SceneManager.h"
#include "Engine/Camera.h"

namespace
{
    const int MAX_RANGE = 9;//プレイヤーが行ける範囲
    const float MAX_MOVE_FRAME = 10;//プレイヤーが再度動けるまでのフレーム
    float MOVE_SPEED = 1.0f;//プレイヤーの移動速度
    float MOVE_AERIAL = 0.05f;//プレイヤーの移動速度
    const float GROUND = 1.0f;//初期位置(Y)
    const float GROUND_LIMIT = 1.0f;
    const float JUMP_HEIGHT = 1.2f;//ジャンプ力
    const float GRAVITY = 0.005f;//重力
    const float MAX_GRAVITY = 6.0f;
}

namespace AnimaFrame
{
    const int IDOL_ANIMATION_FRAME = 59;//待機アニメーション

    const int MOVE_ANIMATION_FRAME = 11;//移動アニメーションのフレーム

    const int SETTING_ANIMATION_FRAME = 80;//Block設置時のアニメーションのフレーム

    const int ATTACK_ANIMATION_FRAME = 129;//Block攻撃時のアニメーションのフレーム

    const int JUMP_ANIMATION_FRAME = 57;//ジャンプアニメーションのフレーム

    const int DAMAGE_ANIMATION_FRAME = 104;//やられアニメーションのフレーム

    const int VICTORY_ANIMATION_FRAME = 50;//ゴールアニメーションのフレーム
}

Player::Player(GameObject* parent) : GameObject(parent, "Player")
, ClearFlag_(false), onGround(true), isBlockCanOnly(false), onMyBlock(false), Jump_Power(0.0f), hPlayerModel_(-1), MoveTimer_(MAX_MOVE_FRAME),isHitEnemyFlag(false),openGoal_(false)
{

  
}

Player::~Player()
{
}

void Player::Initialize()
{
    transform_.scale_ = { 0.5, 0.5, 0.5 };

    // 待機状態
    hPlayerAnimeModel_[0] = Model::Load("Animation//Idle.fbx");
    assert(hPlayerAnimeModel_[0] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[0], 0, AnimaFrame::IDOL_ANIMATION_FRAME, 1.0);

    // 歩くモーション
    hPlayerAnimeModel_[1] = Model::Load("Animation//Standard Walk.fbx");
    assert(hPlayerAnimeModel_[1] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[1], 0, AnimaFrame::MOVE_ANIMATION_FRAME, 1.0);

    // Blockを作った時
    hPlayerAnimeModel_[2] = Model::Load("Animation//Magic Heal.fbx");
    assert(hPlayerAnimeModel_[2] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[2], 0, AnimaFrame::SETTING_ANIMATION_FRAME, 1.0);

    // Blockを押し出したとき
    hPlayerAnimeModel_[3] = Model::Load("Animation//Standing 2H Magic Attack 129.fbx");
    assert(hPlayerAnimeModel_[3] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[3], 0, AnimaFrame::ATTACK_ANIMATION_FRAME, 1.0);

    // ジャンプしたとき
    hPlayerAnimeModel_[4] = Model::Load("Animation//Jumping57.fbx");
    assert(hPlayerAnimeModel_[4] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[4], 10, AnimaFrame::JUMP_ANIMATION_FRAME, 1.2);

    // やられたとき
    hPlayerAnimeModel_[5] = Model::Load("Animation//Down104.fbx");
    assert(hPlayerAnimeModel_[5] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[5], 0, AnimaFrame::DAMAGE_ANIMATION_FRAME, 1.0);

    // ゴールしたとき
    hPlayerAnimeModel_[6] = Model::Load("Animation//Victory Idle50.fbx");
    assert(hPlayerAnimeModel_[6] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[6], 0, AnimaFrame::VICTORY_ANIMATION_FRAME, 1.0);

    // プレイヤー初期位置
    transform_.position_ = { posX, posY, posZ };

    // コライダーの追加
    BoxCollider* collision = new BoxCollider({ 0, 0.55, 0 }, { 1, 1, 1 });
    AddCollider(collision);
}


void Player::Update()
{
    PlayerControl();
    PlayerRange();
    StageHeight();
}

void Player::Draw()
{
    Model::SetTransform(hPlayerModel_, transform_);
    Model::Draw(hPlayerModel_);

    {
        ImGui::Text("Player Position%5.2lf,%5.2lf,%5.2lf", transform_.position_.x, transform_.position_.y, transform_.position_.z);
        ImGui::Text("Player Jump Pawer%5.2lf", Jump_Power);
        ImGui::Text("Player Move CoolTime%5.2lf", MoveTimer_);

        {
            static float pos[3] = { posX,posY,posZ };
            ImGui::Separator();

            if (ImGui::InputFloat3("Player_Position", pos, "%.3f"))
            {
                transform_.position_ = { pos[0],pos[1], pos[2] };
            }
        }
    }
}

void Player::Release()
{
   
}

void Player::PlayerControl()
{
    //左スティックの処理
    XMFLOAT3 LeftStick = Input::GetPadStickL(0);
    XMVECTOR move = XMVectorZero();
    XMVECTOR newPosition;

    bool isMoving = false;

    //ジャンプの処理
    if (Input::IsKeyDown(DIK_SPACE)||Input::IsPadButton(XINPUT_GAMEPAD_A))
    {
        if (prevSpaceKey == false && onGround)
        {
            Jump();
          
        }
        prevSpaceKey = true;
    }
    else
    {
        prevSpaceKey = false;

    }

    //左移動の処理
    if (Input::IsKey(DIK_A)|| LeftStick.x <= -0.3f)
    {

        if (onGround)
        {
            newPosition = XMVectorSet(transform_.position_.x - MOVE_SPEED, transform_.position_.y + 0.01f, transform_.position_.z, 0.0f);
            if (!IsBlocked(newPosition))
            {
                MoveTimer_--;
                if (MoveTimer_ == 0)
                {
                    transform_.position_.x -= MOVE_SPEED;
                    move = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
                    MoveDirection = LEFT;
                    MoveTimer_ = MAX_MOVE_FRAME;
                    moveAnimationTimer_ = AnimaFrame::MOVE_ANIMATION_FRAME;
                    SetPlayerAnimation(1);
                    isMoving = true;
                }
            }
        }
        else
        {
            move = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
            transform_.position_.x -= MOVE_AERIAL;

            // 次の位置を予測
            XMVECTOR NextPosition = XMVectorSet(
                transform_.position_.x - MOVE_AERIAL,
                transform_.position_.y,             
                transform_.position_.z,             
                0.0f
            );

            // 衝突判定
            if (IsBlocked(NextPosition)) {
                transform_.position_.x += MOVE_AERIAL; 

            }
        }


    }


    //右移動の処理
    if (Input::IsKey(DIK_D)|| LeftStick.x >= 0.3)
    {
        if (onGround)
       {
            newPosition = XMVectorSet(transform_.position_.x + MOVE_SPEED, transform_.position_.y + 0.01f, transform_.position_.z, 0.0f);
            if (!IsBlocked(newPosition))
            {
                MoveTimer_--;
                if (MoveTimer_ == 0)
                {
                    transform_.position_.x += MOVE_SPEED;
                    move = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
                    MoveDirection = RIGHT;
                    MoveTimer_ = MAX_MOVE_FRAME;
                    moveAnimationTimer_ = AnimaFrame::MOVE_ANIMATION_FRAME;
                    SetPlayerAnimation(1);
                    isMoving = true;
                }
            }
        }
        else
        {
            move = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
            transform_.position_.x += MOVE_AERIAL;


            XMVECTOR NextPosition = XMVectorSet(
                transform_.position_.x + MOVE_AERIAL + 1.0,//何かずれるんで1.0増やす
                transform_.position_.y,            
                transform_.position_.z,            
                0.0f
            );

            if (IsBlocked(NextPosition)) {
                transform_.position_.x -= MOVE_AERIAL;
            }
        }

    }

    //奥移動の処理
    // 奥移動（前進）の処理
    if (Input::IsKey(DIK_W) || LeftStick.y >= 0.3f)

    {
        if (onGround)
        {
            newPosition = XMVectorSet(transform_.position_.x, transform_.position_.y + 0.01f, transform_.position_.z + MOVE_SPEED, 0.0f);
            if (!IsBlocked(newPosition))
            {
                MoveTimer_--;
                if (MoveTimer_ == 0)
                {
                    transform_.position_.z += MOVE_SPEED;
                    move = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
                    MoveDirection = FORWARD;
                    MoveTimer_ = MAX_MOVE_FRAME;
                    moveAnimationTimer_ = AnimaFrame::MOVE_ANIMATION_FRAME;
                    SetPlayerAnimation(1);
                    isMoving = true;
                }
            }
        }
        else
        {
            move = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
            transform_.position_.z += MOVE_AERIAL;

            // 次の位置を予測
            XMVECTOR NextPosition = XMVectorSet(
                transform_.position_.x,              
                transform_.position_.y,              
                transform_.position_.z + MOVE_AERIAL + 1.0,
                0.0f
            );

            // 衝突判定
            if (IsBlocked(NextPosition)) {
                transform_.position_.z -= MOVE_AERIAL;
            }
        }
    }
    //手前移動の処理
    if (Input::IsKey(DIK_S) || LeftStick.y <= -0.3)
    {
        if (onGround)
        {
            newPosition = XMVectorSet(transform_.position_.x, transform_.position_.y + 0.01f, transform_.position_.z - MOVE_SPEED, 0.0f);
            if (!IsBlocked(newPosition))
            {
                MoveTimer_--;
                if (MoveTimer_ == 0)
                {
                    transform_.position_.z -= MOVE_SPEED;
                    move = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
                    MoveDirection = BACKWARD;
                    MoveTimer_ = MAX_MOVE_FRAME;
                    moveAnimationTimer_ = AnimaFrame::MOVE_ANIMATION_FRAME;
                    SetPlayerAnimation(1);

                    isMoving = true;
                }
            }
        }
        else
        {
            move = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
            transform_.position_.z -= MOVE_AERIAL;

            XMVECTOR NextPosition = XMVectorSet(
                transform_.position_.x,
                transform_.position_.y,
                transform_.position_.z - MOVE_AERIAL,
                0.0f
            );

   
            if (IsBlocked(NextPosition)) {
  
                transform_.position_.z += MOVE_AERIAL;
            }
        }


    }

    if (openGoal_)
    {

        // ゴールアニメーションの進行
        if (victoryAnimationTimer_ > 0)
        {
            victoryAnimationTimer_--; // タイマーを減らす

            if (victoryAnimationTimer_ == 0)
            {
                // シーンを切り替える
                SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
                pSceneManager->ChangeScene(SCENE_ID_CLEAR);
            }
        }
    }

    if (moveAnimationTimer_ > 0)
    {
        moveAnimationTimer_--; // アニメーションタイマーを減らす
        if (moveAnimationTimer_ == 0)
        {
            SetPlayerAnimation(0); // 待機アニメーションに戻す
        }
    }


    //敵に接触してしまったときの処理
    if (isHitEnemyFlag)
    {
        if (moveAnimationTimer_ <= 0)
        {
            SetPlayerAnimation(5); // ダメージアニメーションを開始
            moveAnimationTimer_ = AnimaFrame::DAMAGE_ANIMATION_FRAME; // アニメーション持続時間
        }
        else
        {
            moveAnimationTimer_--; // タイマーを減らす

            if (moveAnimationTimer_ == 0)
            {
                KillMe(); // キャラクター削除
                isHitEnemyFlag = false; // フラグをリセット
            }
        }
    }





    if (Input::IsKeyDown(DIK_L)|| Input::IsPadButton(XINPUT_GAMEPAD_B) && !isBlockCanOnly)
    {
        SetPlayerAnimation(2);
        PlayerBlockInstans();
        isBlockCanOnly = true;
    }

    Jump_Power -= GRAVITY;
    transform_.position_.y += Jump_Power;

   /* if (onGround)
    {
        transform_.position_.x = 1.0f;
    }*/

    //移動した方向に向く処理
    if (!XMVector3Equal(move, XMVectorZero()))
    {
        XMVECTOR pos = XMLoadFloat3(&(transform_.position_));

        XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(-XM_PIDIV2));
        XMVECTOR modifiedVec = XMVector3TransformNormal(move, rot);

        float angle = atan2(XMVectorGetX(move), XMVectorGetZ(move));

        transform_.rotate_.y = XMConvertToDegrees(angle);

        XMStoreFloat3(&(transform_.position_), pos);
    }


    if (onGround && !onMyBlock)
    {
        float gridSize = 1.0f;
        float x = round((transform_.position_.x ) / gridSize) * gridSize;
        float y = round((transform_.position_.y ) / gridSize) * gridSize;
        float z = round((transform_.position_.z ) / gridSize) * gridSize;
        transform_.position_.x = x;
        transform_.position_.y = y;
        transform_.position_.z = z;
    }


}

void Player::PlayerBlockInstans()
{
    PlayerBlock* existingBlock = (PlayerBlock*)FindObject("PlayerBlock");

    if (existingBlock != nullptr)
    {
      existingBlock ->KillMe();
    }
   
    XMVECTOR PlayerPos = XMLoadFloat3(&(transform_.position_));
    XMVECTOR FrontDirection = XMVectorSet(0, 0.5, -1, 0);


    if (Input::IsKey(DIK_UP))
    {
        FrontDirection = XMVectorSet(0, 1.5, -1, 0);
    }

    if (Input::IsKey(DIK_DOWN))
    {
        FrontDirection = XMVectorSet(0, -0.5, -1, 0);
    }

    XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

    XMVECTOR blockPos = PlayerPos + FrontDirection * 1.0f;

    PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
    XMStoreFloat3(&(block->GetPosition()), blockPos);

    
}

void Player::SetPlayerAnimation(int AnimeType)
{
    switch (AnimeType)
    {
    case 0:
        hPlayerModel_ = hPlayerAnimeModel_[0];
        break;
    case 1:
        hPlayerModel_ = hPlayerAnimeModel_[1];
        break;
    case 2:
        hPlayerModel_ = hPlayerAnimeModel_[2];
        break;
    case 3:
        hPlayerModel_ = hPlayerAnimeModel_[3];
        break;
    case 4:
        hPlayerModel_ = hPlayerAnimeModel_[4];
        break;
    case 5:
        hPlayerModel_ = hPlayerAnimeModel_[5];
        break;
    case 6:
        hPlayerModel_ = hPlayerAnimeModel_[6];
        break;
    case 7:
        hPlayerModel_ = hPlayerAnimeModel_[7];
        break;
    default:
        break;
    }
}

void Player::OnCollision(GameObject* parent)
{
    PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");

    if (parent->GetObjectName() == "PlayerBlock")
    {
        XMVECTOR blockPos = XMLoadFloat3(&(pBlock->GetPosition()));
        float blockY = XMVectorGetY(blockPos);

        if (transform_.position_.y <= pBlock->GetPosition().y)
        {
            SetPlayerAnimation(3);
            if (MoveDirection == LEFT)
            {
                transform_.position_.x += MOVE_SPEED;
                pBlock->SetMoveLeft(true);
               
            }
            else if (MoveDirection == RIGHT)
            {
                transform_.position_.x -= MOVE_SPEED;
                pBlock->SetMoveRight(true);
            }
            else if (MoveDirection == FORWARD)
            {
                transform_.position_.z -= MOVE_SPEED;
                pBlock->SetMoveForward(true);
            }
            else if (MoveDirection == BACKWARD)
            {
                transform_.position_.z += MOVE_SPEED;
                pBlock->SetMoveBackwaed(true);
            }
        }

        if (transform_.position_.y > pBlock->GetPosition().y)
        {
            transform_.position_.y = pBlock->GetPosition().y +  0.45f;

            Jump_Power = 0.0f;

            float gridSize = 1.0f;
            float x = round((transform_.position_.x) / gridSize) * gridSize;
            float z = round((transform_.position_.z) / gridSize) * gridSize;
            transform_.position_.x = x;
            transform_.position_.z = z;
            onGround = true;
            onMyBlock = true;
        }

        MoveDirection = NONE; 
    }

   /* Ladder* pLadder= (Ladder*)FindObject("Ladder");

    if (parent->GetObjectName() == "Ladder")
    {
        if (transform_.position_.y > pLadder->GetPosition().y)
        {
            transform_.position_.y = pLadder->GetPosition().y + 0.90f;

            Jump_Power = 0.0f;
        }
    }*/

    KeyFlag* pKey = (KeyFlag*)FindObject("KeyFlag");

    if (parent->GetObjectName() == "KeyFlag")
    {
        ClearFlag_ = true;
    }

    MoveEnemy* pEnemy = (MoveEnemy*)FindObject("MoveEnemy");

    if (parent->GetObjectName() == "MoveEnemy")
    {
        isHitEnemyFlag = true;
    }

    if (parent->GetObjectName() == "GoalFlag")
    {
        openGoal_ = true;
        if (ClearFlag_)
        {
            if (victoryAnimationTimer_ <= 0)
            {
                SetPlayerAnimation(6); // 勝利アニメーションを開始
                victoryAnimationTimer_ = AnimaFrame::VICTORY_ANIMATION_FRAME; // アニメーション持続時間を設定
            }
        }
    }


}

void Player::StageHeight()
{
    Stage* stage = (Stage*)FindObject("Stage");
    PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");
    if (stage)
    {
        float gridSize = 1.0f;
        float snappedX = round(transform_.position_.x / gridSize) * gridSize;
        float snappedZ = round(transform_.position_.z / gridSize) * gridSize;

        int GroundHeight = stage->GetGroundHeight(snappedX, snappedZ);

        //高さが〇〇以上なら乗る
        if (transform_.position_.y <= GroundHeight)
        {
            transform_.position_.y = GroundHeight;
            onGround = true;
            Jump_Power = 0.0f;
        }
        else
        {
            onGround = false;
        }
    }
}

bool Player::IsBlocked(XMVECTOR Position)
{
    Stage* stage = (Stage*)FindObject("Stage");

    if (stage)
    {
        int X = (float)(XMVectorGetX(Position));
        int Z = (float)(XMVectorGetZ(Position));

        if (X >= 0 && X < stage->GetWidth() && Z >= 0 && Z < stage->GetHeight())
        {
            float blockHeight = stage->GetBlockHeight(X, Z);

            // プレイヤーの高さとブロックの高さが同じ以上だったら
            if (blockHeight >= XMVectorGetY(Position) )
            {
                Debug::Log("ステージのブロックに接触", true);
                return true;
            }
        }
    }

    return false;
}

void Player::PlayerRange()
{
    if (transform_.position_.x < 0)
    {
        transform_.position_.x = 0;
    }
    if (transform_.position_.x > MAX_RANGE)
    {
        transform_.position_.x = MAX_RANGE;
    }
    if (transform_.position_.z < 0)
    {
        transform_.position_.z = 0;
    }
    if (transform_.position_.z > MAX_RANGE)
    {
        transform_.position_.z = MAX_RANGE;
    }
}

void Player::Jump()
{
    Jump_Power = sqrtf(2 * GRAVITY * JUMP_HEIGHT);
    onGround = false;
    onMyBlock = false;
    MoveDirection = NONE;

    moveAnimationTimer_ = AnimaFrame::JUMP_ANIMATION_FRAME;

    SetPlayerAnimation(4);
}
