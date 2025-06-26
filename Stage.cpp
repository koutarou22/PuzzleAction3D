#include "Stage.h"
#include "Engine/Model.h"
#include "NcsvReader.h"
#include <format>
#include "Engine/Camera.h"
#include "Ghost.h"
#include "TurretEnemy.h"
#include "KeyFlag.h"
#include "GoalDoor.h"
#include "ResidueItem.h"

namespace {
	const int STAGE_WIDTH { 10 };//ステージのサイズ(X)
	const int STAGE_HEIGHT{ 10 };//ステージのサイズ(Y)
	const int STAGE_LEVEL { 10 };//ステージのサイズ(Z)
	CsvReaderYZ r;

}

using std::vector;


Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hBlock_(-1), stageAlign_(STAGE_LEVEL,
		std::vector<std::vector<int>>(
			STAGE_HEIGHT,
			std::vector<int>(STAGE_WIDTH, 0)
		))
{
	hBlock_ = Model::Load("BoxGrass.fbx");
	r.Load("datas.csv");
}

void Stage::Initialize()
{
	Ghost       * pGhost   = nullptr;
	TurretEnemy * pTurret  = nullptr;

	KeyFlag     * pKey     = nullptr;
	GoalDoor    * pGoal    = nullptr;
	ResidueItem * pResudue = nullptr;


	for (int z = 0;z < STAGE_LEVEL; z++)
	{
		for (int j = 0;j < STAGE_HEIGHT;j++)
		{
			std::string nums = std::format("#{}Layer {}", z, j);
			bool t = r.GetParamLine(nums);
			for (int i = 0;i < STAGE_WIDTH;i++)
			{
				int value = r.PopParamInt();
				stageAlign_[z][j][i] = value;

				//ステージ上にオブジェクトを出現させる　
				//5は既に存在するため登録不可

				switch (value)
				{
				case 1: // ゴースト
					pGhost = Instantiate<Ghost>(this);
					pGhost->SetPosition(i - (STAGE_WIDTH / 2.0f), z, j - (STAGE_HEIGHT / 2.0f));
					break;

				case 2: // 砲台の敵
					pTurret = Instantiate<TurretEnemy>(this);
					pTurret->SetPosition(i - (STAGE_WIDTH / 2.0f), z, j - (STAGE_HEIGHT / 2.0f));
					break;

				case 3: // ゴール用のカギ
					pKey = Instantiate<KeyFlag>(this);
					pKey->SetPosition(i - (STAGE_WIDTH / 2.0f), z, j - (STAGE_HEIGHT / 2.0f));
					break;

				case 4: // ドア
					pGoal = Instantiate<GoalDoor>(this);
					pGoal->SetPosition(i - (STAGE_WIDTH / 2.0f), z, j - (STAGE_HEIGHT / 2.0f));
					break;
				case 6:
					pResudue = Instantiate<ResidueItem>(this);
					pResudue->SetPosition(i - (STAGE_WIDTH / 2.0f), z, j - (STAGE_HEIGHT / 2.0f));
					break;

				default: // デフォルト
					value = 0;
					break;
				}

			}
		}
	}
}

void Stage::Update()
{
	
}

void Stage::Draw()
{
	for (int z = 0; z < STAGE_LEVEL; z++)
	{
		for (int j = 0; j < STAGE_HEIGHT; j++)
		{
			for (int i = 0; i < STAGE_WIDTH; i++)
			{
				if (stageAlign_[z][j][i] == 5) {
					Transform t;
					t.position_ = { i - 5.0f, z - 0.0f, j - 5.0f };
					Model::SetTransform(hBlock_, t);
					Model::Draw(hBlock_);
				}
			}
		}
	}
}


void Stage::Release()
{
}
