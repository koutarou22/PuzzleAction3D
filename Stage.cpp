#include "Stage.h"
#include "Engine/Global.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Engine/CsvReader.h"
#include "Player.h"
#include <vector>
#include <DirectXMath.h>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "MoveEnemy.h"
#include "GoalDoor.h"
#include "KeyFlag.h"
#include "BulletEnemy.h"
#include "Engine/Text.h"
#include "Shadow.h"
#include "Bullet.h"


using namespace DirectX;
using std::string;



void Stage::SetBlockType(int BlockNum)
{
    switch (BlockNum)
    {
    case 0:
        hStage_ = Model::Load("BoxGrass2.fbx");
        break;

    case 1:
        hStage_ = Model::Load("BoxSand2.fbx");
        break;

    case 2:
        hStage_ = Model::Load("BoxBrick2.fbx");
        break;

    default:
        break;
    }
}

Stage::Stage(GameObject* parent)
    : GameObject(parent, "Stage"), Width(10), Height(10), SelectMode(0), SelectType(0)
{
    for (int layer = 0; layer < AllLayer; ++layer)
    {
        CsvReader csv;
        string filename = "layer" + std::to_string(layer) + ".csv";
        csv.Load(filename);

        vector<int> layerData;
        for (int h = 0; h < Height + 0.5; ++h)
        {
            for (int w = 0; w < Width; ++w)
            {
                layerData.push_back(csv.GetValue(w, h));
            }
        }
        Stagelayer.push_back(layerData);

        for (int h = 0; h < Height; h++)
        {
            for (int w = 0; w < Width; w++)
            {

                Player* pPlayer = nullptr;
                MoveEnemy* pMoveEnemy = nullptr;
                GoalDoor* pGoalDoor = nullptr;
                KeyFlag* pKeyFlag = nullptr;

                switch (csv.GetValue(w, h))
                {
                case 0:
                    break;
                case 1:
                    /*   pPlayer = Instantiate<Player>(GetParent());
                       pPlayer->SetPosition(w, h, layer);*/
                    break;
                case 2:
                    pMoveEnemy = Instantiate<MoveEnemy>(GetParent());
                    pMoveEnemy->SetPosition(w, h, layer);
                    break;
                case 3:
                    pGoalDoor = Instantiate<GoalDoor>(GetParent());
                    pGoalDoor->SetPosition(w, h, layer);
                    break;
                case 4:
                    pKeyFlag = Instantiate<KeyFlag>(GetParent());
                    pKeyFlag->SetPosition(w, h, layer);
                    break;
                }


            }
        }
    }



    //ここの処理でプレイヤーが地についている
    //２次元配列を用いて、層のように積み重ねていく
    for (int layer = 0; layer < AllLayer; layer++) 
    {
        for (int h = 0; h < Height; h++) 
        {
            for (int w = 0; w < Width; w++) 
            {
                int value = Stagelayer[layer][h * Width + w];
                if (value != 0)
                {
                    table[w][h].height = layer + 1.0;
                    table[w][h].type = value;
                }
            }
        }
    }
}
Stage::~Stage()
{
}

void Stage::Initialize()
{
    SetBlockType(0);//初期ブロックタイプ
}

void Stage::Update()
{

    PlayerRayHitStage();
  
}

void Stage::Draw()
{
    for (int layer = 0; layer < AllLayer; ++layer) 
    {
        for (int z = 0; z < Height; ++z) 
        {
            for (int x = 0; x < Width; ++x) 
            {
                int blockType = Stagelayer[layer][z * Width + x];

                //この処理だと他のオブジェクトを出した時もBlockがでてしまう
                if (blockType == 1 || blockType == 2 || blockType == 3 || blockType == 4 || blockType == 5)
                {
                    Transform trs;
                    trs.position_.x = x;
                    trs.position_.y = layer; 
                    trs.position_.z = z;

                    Model::SetTransform(hStage_, trs);
                    Model::Draw(hStage_);
                }

            }
        }
    }
}




void Stage::Release()
{
    
}

float Stage::GetGroundHeight(float x, float z)
{
    int X = x;
    int Z = z;

    if (X >= 0 && X < Width && Z >= 0 && Z < Height)
    {
        return (float)(table[X][Z].height);
    }

    return 0.0f;
}

void Stage::PlayerRayHitStage()
{
    Player* pPlayer = (Player*)FindObject("Player");
    MoveEnemy* pMoveEnemy = (MoveEnemy*)FindObject("MoveEnemy");
    Bullet* pBullet = (Bullet*)FindObject("Bullet");
    Shadow* pShadow = (Shadow*)FindObject("Shadow");


    //XMFLOAT3 MoveEnmeyRayStart = pMoveEnemy->GetRayStart();
    //XMFLOAT3 BulletRayStart = pBullet->GetRayStart();

    //XMFLOAT3 MoveEnemyPos = pMoveEnemy->GetPosition();
    //XMFLOAT3 BulletPos = pBullet->GetPosition();



    bool PlayerOnGround = false;

    if (pPlayer != nullptr )
    {

        //WidthとHeightを回して、全体を把握
        for (int x = 0; x <= Width; x++)
        {
            for (int z = 0; z <= Height; z++)
            {
            
                RayCastData PlayerRaydata;//
             
                //レイの開始地点はプレイヤーに設定
                PlayerRaydata.start = pPlayer->GetRayStart();

                //真下にレイを飛ばす
                PlayerRaydata.dir = XMFLOAT3(0, -1, 0);

                int PlayerX = pPlayer->GetPosition().x;
                int PlayerY = pPlayer->GetPosition().y;


                // 現在立っているマスの高さを把握し、その位置に配置
                float GroundHeight = table[PlayerX][PlayerY].height;
                transform_.position_ = XMFLOAT3(x, GroundHeight, z);


                //対象にレイキャストを行う
                Model::SetTransform(hStage_, transform_);
                Model::RayCast(hStage_, &PlayerRaydata);

             
                //ヒットしたなら処理を実行
                //現在は確認しかしていない
                if (PlayerRaydata.hit)
                {

                    //影をまだ呼び出してない && 影のリストがMaxじゃなければ(マジックナンバーは後で消そう)
                    if (!shadowCreated && ShadowHitPosition.size() < 5)
                    {
                        Shadow* p = Instantiate<Shadow>(this);
                        XMFLOAT3 PPos = { pPlayer->GetPosition().x - 9,  0.1, pPlayer->GetPosition().z - 9 };


                        XMFLOAT3 pos = PPos;
                        p->SetPosition(pos);

                        shadowCreated = true;
                        break;
                    }
                    else
                    {
                        XMFLOAT3 PPos = { pPlayer->GetPosition().x - 10,  0.1, pPlayer->GetPosition().z - 10 };


                        XMFLOAT3 pos = PPos;
                        pShadow->SetPosition(pos);
                    }


                    float RayHeight = pPlayer->GetRayHeight();
                    float distance = PlayerRaydata.dist - RayHeight;
                    
                   if (distance >= -1.0f && distance <= 0.0f)
                   {
                        Debug::Log("レイがあたってます！", true);

                        break;
                   }
                   else
                   {
                       Debug::Log("レイが範囲外です", true);
                       break;
                   }
                }
            }
        }
    
    }
}

//void Stage::UnderShadowRayDate
//(   
//    RayCastData name, XMFLOAT3 RayStart,
//    int dirX, int dirY, int dirZ,
//    XMFLOAT3 CharacterPos
//)
//{
//    Player* pPlayer = (Player*)FindObject("Player");
//    MoveEnemy* pMoveEnemy = (MoveEnemy*)FindObject("MoveEnemy");
//    Bullet* pBullet = (Bullet*)FindObject("Bullet");
//   Shadow* p = (Shadow*)FindObject("Shadow");
//
//
//   if (pPlayer != nullptr || pMoveEnemy != nullptr || pBullet != nullptr)
//   {
//       //レイの開始地点はプレイヤーに設定
//       name.start = RayStart;//pPlayer->GetRayStart();
//
//       //真下にレイを飛ばす
//       name.dir = XMFLOAT3(dirX, dirY, dirZ);
//
//       //対象にレイキャストを行う
//       Model::SetTransform(hStage_, transform_);
//       Model::RayCast(hStage_, &name);
//
//       //ヒットしたなら処理を実行
//             //現在は確認しかしていない
//       if (name.hit)
//       {
//
//           if (!shadowCreated)
//           {
//               Shadow* p = Instantiate<Shadow>(this);
//
//
//               XMFLOAT3 pos = CharacterPos;
//               p->SetPosition(pos);
//
//               shadowCreated = true;
//           }
//           else
//           {
//
//               XMFLOAT3 pos = CharacterPos;
//               p->SetPosition(pos);
//           }
//
//       }
//   }
//
//}
