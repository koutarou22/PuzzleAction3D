#include "Stage.h"
#include "Engine/Global.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Player.h"
#include <vector>
#include <DirectXMath.h>

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

    for (size_t i = 0; i < StagePosList_.size(); i++)
    {
        RayCastData data;
        data.start = pPlayer->GetRayStart();
        data.dir = XMFLOAT3(0, -1, 0);

        transform_.position_ = StagePosList_[i];
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
//Player* pPlayer = (Player*)FindObject("Player");
//
//for (size_t i = 0; i < StagePosList_.size(); i++)
//{
//    RayCastData data;
//    data.start = pPlayer->GetRayStart();
//    data.dir = XMFLOAT3(0, -1, 0);
//
//    transform_.position_ = StagePosList_[i];
//    Model::SetTransform(hStage_, transform_);
//    Model::RayCast(hStage_, &data);
//
//    pPlayer->SetonGround(false);
//    data.dist = 0;
//
//    if (data.hit)
//    {
//        if (data.dist - pPlayer->GetRayHeight() >= -1.0f && data.dist - pPlayer->GetRayHeight() <= 1.0f) {
//            data.dist = data.dist - pPlayer->GetRayHeight();
//            Debug::Log("レイがあたってます！", true);
//            pPlayer->SetonGround(true);
//            break;
//        }
//    }
//}

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
}

void Stage::Release()
{
    // 必要に応じてリソースを解放します
}
