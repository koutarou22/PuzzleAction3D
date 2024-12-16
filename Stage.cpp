#include "Stage.h"
#include "iostream"
#include "Engine/Global.h"
#include "Engine/Model.h"
#include "Engine/Debug.h"
#include "Player.h"

using std::string;

Stage::Stage(GameObject* parent):GameObject(parent, "Stage"), hStage_(), Width(10), Height(10), SelectMode(0), SelectType(0)
{
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Height; z++)
        {
            table[x][z].height = 1;
            table[x][z].type = 2;
        }
    }
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    string fileName[] = { "BoxDefault", "BoxBrick", "BoxGrass", "BoxSand", "BoxWater" };

    for (int i = 0; i < 5; i++)
    {
        string path = fileName[i] + ".fbx";
        hStage_[i] = Model::Load(path);
    }


}

void Stage::Update()
{
    Player* pPlayer = (Player*)FindObject("Player");
    int hPlayerModel = pPlayer->GetModelHandle();

    RayCastData data;
    data.start = transform_.position_;
    data.start.y = 0;
    data.dir = XMFLOAT3(0, -1, 0);
    Model::RayCast(hPlayerModel, &data);
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

                int type = table[x][z].type;
                
                Model::SetTransform(hStage_[type], trs);
                Model::Draw(hStage_[type]);
            }
        }
    }
}

void Stage::Release()
{
   /* for (int i = 0; i < 5; i++)
    {
        SAFE_DELETE(hStage_[i]);
        DELETE(hStage_)
    }*/
}
