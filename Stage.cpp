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

        vector<int> layerData;//レイヤーデータを作成し
        for (int h = 0; h < Height + 0.5; ++h) 
        {
            for (int w = 0; w < Width; ++w) 
            {
                layerData.push_back(csv.GetValue(w, h));//詰める
            }
        }
        Stagelayer.push_back(layerData);//StageLayerに格納する


        //Block以外の敵やオブジェクトの情報の指定
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
                case 2://動く敵
                    pMoveEnemy = Instantiate<MoveEnemy>(GetParent());
                    pMoveEnemy->SetPosition(w, layer, h);
                    break;
                case 3://ゴール用のオブジェクト
                    pGoalDoor = Instantiate<GoalDoor>(GetParent());
                    pGoalDoor->SetPosition(w, layer, h );
                    break;
                case 4://ゴール条件を満たすためのオブジェクト
                    pKeyFlag = Instantiate<KeyFlag>(GetParent());
                    pKeyFlag->SetPosition(w, layer, h);
                    break;
                case 5:
                    SetBlockType(0);//地面は草になる
                    break;
                case 6:
                    SetBlockType(1);//地面は砂になる
                    break;

                case 7:
                    SetBlockType(2);//地面は岩になる
                    break;

                default:
                    break;


                }
            }
        }
    }

    
    //２次元配列を用いて、層のように積み重ねていく
    for (int layer = 0; layer < AllLayer; layer++) {
        for (int h = 0; h < Height; h++) {
            for (int w = 0; w < Width; w++) {
                int value = Stagelayer[layer][h * Width + w];
                if (value != 0)
                {
                    table[w][h].height = layer + 1;
                    table[w][h].type = value;
               
                }
            }
        }


        /*
        table[3][9].height = 6;
        table[3][8].height = 6;

        table[4][9].height = 6;
        table[4][8].height = 6;

        table[4][7].height = 6;
        table[3][7].height = 6;

        table[5][9].height = 6;
        table[5][8].height = 6;

        table[2][2].height = 2;
        table[2][3].height = 3;
        table[3][2].height = 2;
        table[3][3].height = 3;

        table[5][4].height = 4;
        table[2][4].height = 4;
        table[1][4].height = 4;
        table[1][5].height = 4;
        table[3][4].height = 4;
        table[5][3].height = 3;
        table[7][5].height = 4;

        table[6][5].height = 4;
        table[5][5].height = 4;

        table[0][9].height = 4;
        table[0][8].height = 4;
        table[1][9].height = 4;
        table[1][8].height = 4;

        table[2][9].height = 4;
        table[2][8].height = 4;
        */


    }
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    SetBlockType(0);
}

void Stage::Update()
{

    PlayerRayHitStage();
  
}

void Stage::Draw()
{
    for (int layer = 0; layer < AllLayer; ++layer) {
        for (int z = 0; z < Height; ++z) {
            for (int x = 0; x < Width; ++x) {
                int blockType = Stagelayer[layer][z * Width + x];

                //この処理だと他のオブジェクトを出した時もBlockがでてしまう

                //if (blockType != 0) {  // 空白でない場合のみ処理
                if (blockType != 0)
                {
                    Transform trs;
                    trs.position_.x = x;
                    trs.position_.y = layer;  // 層の高さ
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
    int Y = z;

    if (X >= 0 && X < Width && Y >= 0 && Y < Height)
    {
        return (float)(table[X][Y].height);
    }

    return 0.0f; 
}

void Stage::PlayerRayHitStage()
{
    Player* pPlayer = (Player*)FindObject("Player");

    bool PlayerOnGround = false;

    if (pPlayer != nullptr)
    {
        for (int x = 0; x < Width; x++)
        {
            for (int z = 0; z < Height; z++)
            {
                RayCastData data;
                data.start = pPlayer->GetRayStart();
                data.dir = XMFLOAT3(0, -1, 0);


                float GroundHeight = table[x][z].height;
                transform_.position_ = XMFLOAT3(x, GroundHeight, z);

                Model::SetTransform(hStage_, transform_);
                Model::RayCast(hStage_, &data);

                if (data.hit)
                {
                    float RayHeight = pPlayer->GetRayHeight();
                    float distance = data.dist - RayHeight;

                    if (distance >= -1.0f && distance <= 0.0f)
                    {
                        Debug::Log("レイがあたってます！", true);
                        break;
                    }
                    else
                    {
                        Debug::Log("レイが範囲外です", true);
                    }
                }
            }
        }
    }

}
