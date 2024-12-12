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

Stage::Stage(GameObject* parent):GameObject(parent, "Stage"), pFbx(), Width(10), Height(10), SelectMode(0), SelectType(0)
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
        //pFbx[i] = new Fbx;
        string path = fileName[i] + ".fbx";
        //pFbx[i]->Load(path);
        t[i] = Model::Load(path);
    }

    hStageModel_ = Model::Load("BoxDefault.fbx");
    assert(hStageModel_ >= 0);
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
                //pFbx[type]->Draw(trs,0);
                Model::SetTransform(t[type], trs);
                Model::Draw(t[type]);
                //Model::SetTransform(hStageModel_, transform_);
                //Model::Draw(hStageModel_);
            }
        }
    }
}

void Stage::Release()
{
    for (int i = 0; i < 5; i++)
    {
        pFbx[i]->Release();
        SAFE_DELETE(pFbx[i]);
    }
}
