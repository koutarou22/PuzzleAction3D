#include "Stage.h"
#include "iostream"
#include "Engine/Global.h"
#include "Engine/Model.h"

using std::string;

//Stage::Stage() : pFbx(), Width(10), Height(10), SelectMode(0), SelectType(0)
//{
//    for (int x = 0; x < Width; x++)
//    {
//        for (int z = 0; z < Height; z++)
//        {
//            table[x][z].height = 1;
//            table[x][z].type = 0;
//        }
//    }
//}

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
