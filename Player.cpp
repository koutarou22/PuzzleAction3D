#include "Player.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/BoxCollider.h"
#include "Stage.h"
#include <iostream>7
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
#include "CameraController.h"
#include "ScoreItem.h"



namespace
{
    //移動をフレームで補間用 (テスト)
    //ここの数値で移動(見た目)速度を操作する
    const int PLAYER_MOVE_INTERPOLATION = 10;

    const int MAX_RANGE = 9;//プレイヤーが行ける範囲
    const float MAX_MOVE_FRAME = 10;//プレイヤーが再度動けるまでのフレーム
    float MOVE_SPEED = 1.0f / PLAYER_MOVE_INTERPOLATION;//プレイヤーの移動速度
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

    const int MOVE_ANIMATION_FRAME = 40;//移動アニメーションのフレーム

    const int SETTING_ANIMATION_FRAME = 80;//Block設置時のアニメーションのフレーム

    const int ATTACK_ANIMATION_FRAME = 129;//Block攻撃時のアニメーションのフレーム

    const int JUMP_ANIMATION_FRAME = 57;//ジャンプアニメーションのフレーム

    const int DAMAGE_ANIMATION_FRAME = 104;//やられアニメーションのフレーム

    const int VICTORY_ANIMATION_FRAME = 50;//ゴールアニメーションのフレーム
}

Player::Player(GameObject* parent) : GameObject(parent, "Player")
, ClearFlag_(false), onGround(true), isBlockCanOnly(false), onMyBlock(false), Jump_Power(0.0f), hPlayerModel_(-1), MoveTimer_(MAX_MOVE_FRAME),isHitEnemy_(false),openGoal_(false),GetRubyflag(false)
,isMoveCamera_(false)
{

    isMove_interpolation = false;

    isGoMove = false;
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
    hPlayerAnimeModel_[3] = Model::Load("Animation//Standing 2H Magic Attack.fbx");
    assert(hPlayerAnimeModel_[3] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[3], 0, AnimaFrame::ATTACK_ANIMATION_FRAME, 1.0);

    // ジャンプしたとき
    hPlayerAnimeModel_[4] = Model::Load("Animation//Jumping.fbx");
    assert(hPlayerAnimeModel_[4] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[4], 10, AnimaFrame::JUMP_ANIMATION_FRAME, 1.2);

    // やられたとき
    hPlayerAnimeModel_[5] = Model::Load("Animation//Down.fbx");
    assert(hPlayerAnimeModel_[5] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[5], 0, AnimaFrame::DAMAGE_ANIMATION_FRAME, 1.0);

    // ゴールしたとき
    hPlayerAnimeModel_[6] = Model::Load("Animation//Victory Idle.fbx");
    assert(hPlayerAnimeModel_[6] >= 0);
    Model::SetAnimFrame(hPlayerAnimeModel_[6], 0, AnimaFrame::VICTORY_ANIMATION_FRAME, 1.0);

    // プレイヤー初期位置
    transform_.position_ = { 1.0f, posY, posZ };

    // コライダーの追加
    BoxCollider* collision = new BoxCollider({ 0, 0.55, 0 }, { 1, 1, 1 });
    AddCollider(collision);


    SetPlayerAnimation(0);

    isMove_ = PLAYER_MOVE_INTERPOLATION;

    MoveDirection = NONE;
  
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
        ImGui::Text("Player Move CoolTime %d", isMove_);
        ImGui::Text("Player interpolation %d", progress);

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
    // カメラオブジェクトを取得
    CameraController* pCamera = (CameraController*)FindObject("CameraController");
    XMFLOAT3 LeftStick = Input::GetPadStickL(0);
    XMVECTOR move = XMVectorZero();

    XMVECTOR BaseMove = XMVectorZero();
    XMVECTOR NextPosition = XMVectorZero();

    if (!isMoveCamera_ || isHitEnemy_) // カメラが動いていなければOK
    {
        // ジャンプの処理
        if (Input::IsKey(DIK_SPACE) || Input::IsPadButton(XINPUT_GAMEPAD_A))
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

        // フレームでの移動
        if (isMove_interpolation)
        {
            isMove_--;

            // 進捗を計算
            float progress = (float)isMove_ / PLAYER_MOVE_INTERPOLATION;


            switch (MoveDirection)
            {
            case LEFT:
                PlayerMove(BaseMove, NextPosition, -1.0f, 0.0f, 0.0f);
              
                break;
            case RIGHT:
                PlayerMove(BaseMove, NextPosition, 1.0f, 0.0f, 0.0f);
                break;
            case FORWARD:
                PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, -1.0f);
                break;
            case BACKWARD:
                PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, 1.0f);
                break;
            default:
                break;
            }

           

            if (progress <= 0.0f)
            {
                isGoMove = true;
                isMove_interpolation = false;

                MoveDirection = NONE;
                //到着したらここで一回停止し初期化
                PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, 0.0f);
                isMove_ = PLAYER_MOVE_INTERPOLATION;
                //停止後ずれないようにブロックの中心に修正
                PlayerGridCorrection();

               
            }
            else
            {
                isGoMove = false;
            }
        }

        // 左移動の処理
        if (onGround && (Input::IsKey(DIK_A) || LeftStick.x <= -0.5f))
        {
            isMove_interpolation = true;
            MoveDirection = LEFT;

           
        }
        else if (!onGround && (Input::IsKey(DIK_A) || LeftStick.x <= -0.3f))
        {
            PlayerMove(BaseMove, NextPosition, -1.0f, 0.0f, 0.0f);
            isMove_ = PLAYER_MOVE_INTERPOLATION;
            
        }

        // 右移動の処理
        if (onGround && (Input::IsKey(DIK_D) || LeftStick.x >= 0.3f))
        {
            isMove_interpolation = true;
            MoveDirection = RIGHT;
           
        }
        else if (!onGround && (Input::IsKey(DIK_D) || LeftStick.x >= 0.3f))
        {
            PlayerMove(BaseMove, NextPosition, 1.0f, 0.0f, 0.0f);
      
        }

        // 奥移動の処理
        if (onGround && (Input::IsKey(DIK_W) || LeftStick.y >= 0.3f))
        {
            isMove_interpolation = true;
            MoveDirection = BACKWARD;
           
        }
        else if (!onGround && (Input::IsKey(DIK_W) || LeftStick.y >= 0.3f))
        {
            PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, 1.0f);
         
        }

        // 手前移動の処理
        if (onGround && (Input::IsKey(DIK_S) || LeftStick.y <= -0.3f))
        {
            isMove_interpolation = true;
            MoveDirection = FORWARD;
          
        }
        else if (!onGround && (Input::IsKey(DIK_S) || LeftStick.y <= -0.3f))
        {
            PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, -1.0f);
         
        }

        // ブロック生成
        if (Input::IsKeyDown(DIK_L) || Input::IsPadButton(XINPUT_GAMEPAD_B) && !isBlockCanOnly)
        {
            PlayerBlockInstans();
            isBlockCanOnly = true;
        }
    }

    // ゴールアニメーションの進行
    if (openGoal_)
    {
        if (victoryAnimationTimer_ > 0)
        {
            victoryAnimationTimer_--; // タイマーを減少

            if (victoryAnimationTimer_ == 0)
            {
                // シーンを切り替える
                SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
                pSceneManager->ChangeScene(SCENE_ID_CLEAR);
            }
        }
    }

    // アニメーションタイマーの進行
    if (moveAnimationTimer_ > 0)
    {
        moveAnimationTimer_--;
        if (moveAnimationTimer_ == 0)
        {
            SetPlayerAnimation(0);
        }
    }

    // 敵に接触したときの処理
    if (isHitEnemy_)
    {
        if (moveAnimationTimer_ <= 0)
        {
            SetPlayerAnimation(5);
            moveAnimationTimer_ = AnimaFrame::DAMAGE_ANIMATION_FRAME;

            PlayerMove(BaseMove, NextPosition, 0.0f, 0.0f, 0.0f);//移動を完全停止
        }
        else
        {
            moveAnimationTimer_--;

            if (moveAnimationTimer_ == 0)
            {
                KillMe();
                isHitEnemy_ = false;
            }
        }
    }

    // 重力処理（落下）
    Jump_Power -= GRAVITY;
    transform_.position_.y += Jump_Power;

   // StageGridCorrection();

}


void Player::PlayerMove(XMVECTOR BaseMove, XMVECTOR NextPos, float x, float y, float z)
{
    // カメラを取得
    CameraController* pCamera = (CameraController*)FindObject("CameraController");

    XMVECTOR move = XMVectorZero();

    // カメラの回転行列を取得
    XMMATRIX cameraRotation = pCamera->GetRotationMatrix();

    BaseMove = XMVectorSet(x, y, z, 0.0f); // 左方向ベクトル
    move = XMVector3TransformCoord(BaseMove, cameraRotation); // カメラ基準で補正

    bool isMoving = false;

    if (onGround)
    {
        // 地上移動処理
        BaseMove = XMVectorSet(
            transform_.position_.x + XMVectorGetX(move) * MOVE_SPEED,
            transform_.position_.y + 0.01f,
            transform_.position_.z + XMVectorGetZ(move) * MOVE_SPEED,
            0.0f
        );
        if (!IsBlocked(BaseMove))
        {
            transform_.position_.x += XMVectorGetX(move) * MOVE_SPEED;
            transform_.position_.z += XMVectorGetZ(move) * MOVE_SPEED;
            MoveTimer_ = MAX_MOVE_FRAME;
            moveAnimationTimer_ = AnimaFrame::MOVE_ANIMATION_FRAME;
            SetPlayerAnimation(1);
            isMoving = true;
        }
    }
    else
    {
        // 空中移動処理
        move = XMVector3TransformCoord(BaseMove, cameraRotation);
        transform_.position_.x += XMVectorGetX(move) * MOVE_AERIAL;
        transform_.position_.z += XMVectorGetZ(move) * MOVE_AERIAL;

        NextPos = XMVectorSet(
            transform_.position_.x + XMVectorGetX(move) * MOVE_AERIAL,
            transform_.position_.y,
            transform_.position_.z + XMVectorGetZ(move) * MOVE_AERIAL,
            0.0f
        );

        if (IsBlocked(NextPos))
        {
            transform_.position_.x -= XMVectorGetX(move) * MOVE_AERIAL;
            transform_.position_.z -= XMVectorGetZ(move) * MOVE_AERIAL;
        }
    }

    // プレイヤーが移動した方向に向く処理
    if (!XMVector3Equal(move, XMVectorZero()))
    {
        XMVECTOR pos = XMLoadFloat3(&(transform_.position_));

        XMMATRIX rot = XMMatrixRotationY(XMConvertToRadians(XM_PIDIV2));
        XMVECTOR modifiedVec = XMVector3TransformNormal(move, rot);

        float angle = atan2(XMVectorGetX(move), XMVectorGetZ(move));

        transform_.rotate_.y = XMConvertToDegrees(angle);

        XMStoreFloat3(&(transform_.position_), pos);

        // 反転
        angle += XM_PI;

        //  計算した角度を度に変換して反映
        transform_.rotate_.y = XMConvertToDegrees(angle);
    }
    // プレイヤーの移動方向に基づいて向きを設定
    //    if (!XMVector3Equal(move, XMVectorZero()))
    //    {
    //         回転行列を取得
    //        XMMATRIX rotationMatrix = pCamera->GetRotationMatrix();
    //
    //         ベクトル変換で移動方向をカメラ基準で補正
    //        XMVECTOR adjustedMove = XMVector3TransformCoord(move, rotationMatrix);
    //
    //         atan2で角度を計算
    //        float angle = atan2(XMVectorGetX(adjustedMove), XMVectorGetZ(adjustedMove));
    //
    //         反転処理を追加（180度補正）
    //        angle += XM_PI;
    //
    //         計算した角度を度に変換して反映
    //        transform_.rotate_.y = XMConvertToDegrees(angle);
    //    }
    //}

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


    if (Input::IsKey(DIK_UP)|| Input::IsPadButton(XINPUT_GAMEPAD_LEFT_SHOULDER))
    {
        FrontDirection = XMVectorSet(0, 1.5, -1, 0);
    }

    if (Input::IsKey(DIK_DOWN)|| Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER))
    {
        FrontDirection = XMVectorSet(0, -0.5, -1, 0);
    }

    XMMATRIX RotationMatrix = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
    FrontDirection = XMVector3TransformNormal(FrontDirection, RotationMatrix);

    XMVECTOR blockPos = PlayerPos + FrontDirection * 1.0f;

    PlayerBlock* block = Instantiate<PlayerBlock>(GetParent());
    XMStoreFloat3(&(block->GetPosition()), blockPos);

    SetPlayerAnimation(2);
    moveAnimationTimer_ = AnimaFrame::SETTING_ANIMATION_FRAME; // ダメージアニメーション持続時間
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
          
            if (MoveDirection == LEFT)
            {
                transform_.position_.x += MOVE_SPEED;
                pBlock->SetMoveLeft(true);

                SetPlayerAnimation(3);
                moveAnimationTimer_ = AnimaFrame::ATTACK_ANIMATION_FRAME;
            }
            else if (MoveDirection == RIGHT)
            {
                transform_.position_.x -= MOVE_SPEED;
                pBlock->SetMoveRight(true);

                SetPlayerAnimation(3);
                moveAnimationTimer_ = AnimaFrame::ATTACK_ANIMATION_FRAME; 
            }
            else if (MoveDirection == FORWARD)
            {
                transform_.position_.z -= MOVE_SPEED;
                pBlock->SetMoveForward(true);

                SetPlayerAnimation(3);
                moveAnimationTimer_ = AnimaFrame::ATTACK_ANIMATION_FRAME;
            }
            else if (MoveDirection == BACKWARD)
            {
                transform_.position_.z += MOVE_SPEED;
                pBlock->SetMoveBackwaed(true);

                SetPlayerAnimation(3);
                moveAnimationTimer_ = AnimaFrame::ATTACK_ANIMATION_FRAME; 
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
       isHitEnemy_ = true;
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
                victoryAnimationTimer_ = 0;
            }
        }
    }


    ScoreItem* pScoreItem = (ScoreItem*)FindObject("ScoreItem");
    if (parent->GetObjectName() == "ScoreItem")
    {
        GetRubyflag = true;
        pScoreItem->KillMe();
    }


}

void Player::StageHeight()
{
    Stage* pstage = (Stage*)FindObject("Stage");
    PlayerBlock* pBlock = (PlayerBlock*)FindObject("PlayerBlock");
    if (pstage != nullptr)
    {
        float gridSize = 1.0f;
        float snappedX = round(transform_.position_.x / gridSize) * gridSize;
        float snappedZ = round(transform_.position_.z / gridSize) * gridSize;

        int GroundHeight = pstage->GetGroundHeight(snappedX, snappedZ);

        if (GroundHeight > 0 && transform_.position_.y <= GroundHeight) {  // 高さがある場合のみ処理
            transform_.position_.y = GroundHeight;
            onGround = true;
            Jump_Power = 0.0f;
        }
        else {
            onGround = false;
        }

        //しかしもし２ブロック以上の高さがあるなら.....
        //その下を通れる処理も実装したい

    }
}

bool Player::IsBlocked(XMVECTOR Position) {
    Stage* stage = (Stage*)FindObject("Stage");

    if (stage) {
        int X = XMVectorGetX(Position);
        int Z = XMVectorGetZ(Position);

        if (X >= 0 && X < stage->GetWidth() && Z >= 0 && Z < stage->GetHeight()) {
            float blockHeight = stage->GetBlockHeight(X, Z);

            
            if (blockHeight > 0 && blockHeight >= XMVectorGetY(Position)) {
                Debug::Log("ステージのブロックに接触", true);
                return true; 
            }

            //ブロックがプレイヤーより上に２つ以上あるなら通れるような処理を実装したい
            if (X < stage->GetHeight() + 2.0f)
            {
                if (blockHeight > 0 && blockHeight <= XMVectorGetY(Position)) {
                    Debug::Log("ステージのブロックはどうやら２ブロック以上あるようだ", true);
                    return false;
                }
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


    SetPlayerAnimation(4);
    moveAnimationTimer_ = AnimaFrame::JUMP_ANIMATION_FRAME;

    if (onGround)
    {
        SetPlayerAnimation(1);
    }
}

void Player::PlayerGridCorrection()
{
    // 地上でのグリッド位置補正
    if (onGround && !onMyBlock)
    {
        float gridSize = 1.0f;
        float x = round((transform_.position_.x) / gridSize) * gridSize;
        float y = round((transform_.position_.y) / gridSize) * gridSize;
        float z = round((transform_.position_.z) / gridSize) * gridSize;
        transform_.position_.x = x;
        transform_.position_.y = y;
        transform_.position_.z = z;
    }
}
