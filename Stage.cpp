#include "Stage.h"
#include "Engine/Global.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Player.h"
#include <vector>
#include <DirectXMath.h>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

using namespace DirectX;
using std::string;

Stage::Stage(GameObject* parent)
    : GameObject(parent, "Stage"), Width(10), Height(10), SelectMode(0), SelectType(0)
{
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Height; z++)
        {
            table[x][z].height = 1;
            table[x][z].type = 2;
        }
    }
    table[3][9].height = 6;
    table[3][8].height = 6;

    table[4][9].height = 6;
    table[4][8].height = 6;

    table[5][9].height = 6;
    table[5][8].height = 6;
    table[2][2].height = 2;
    table[2][3].height = 3;
    table[4][4].height = 4;

    float startX = 0.0f;
    float startY = 1.0f; 
    float startZ = 0.0f;
    float offset = 1.0f;

    for (int x = 0; x < 10; ++x)
    {
        for (int z = 0; z < 10; ++z)
        {
            StagePosList_.emplace_back(startX + x * offset, startY, startZ + z * offset);
        }
    }
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    string fileName = "BoxDefault";
    string path = fileName + ".fbx";
    hStage_ = Model::Load(path);
}

void Stage::Update()
{
    Player* pPlayer = (Player*)FindObject("Player");

    bool PlayerOnGround = false;

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
                    pPlayer->SetonGround(true);
                    PlayerOnGround = true;
                    break;
                }
                else
                {
                    Debug::Log("レイが範囲外です", true);
                }
            }

            if (!PlayerOnGround)
            {
                pPlayer->SetonGround(false);
            }
        }
    }
}

void Stage::Draw()
{
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Height; z++)
        {
            for (int y = 0; y < table[x][z].height; y++)
            {
                Transform trs;
                trs.position_.x = x;
                trs.position_.y = y;
                trs.position_.z = z;

                Model::SetTransform(hStage_, trs);
                Model::Draw(hStage_);
            }
        }
    }

    {
        //ImGui::ShowDemoWindow();
      /*  ImGui::Text("This is My Original Shader");*/
        ImGui::Text("Stage Position%5.2lf,%5.2lf,%5.2lf", transform_.position_.x, transform_.position_.y, transform_.position_.z);
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