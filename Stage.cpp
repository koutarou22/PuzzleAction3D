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
#include "PlayerBlock.h"

namespace {
	const int STAGE_WIDTH{ 10 };//ステージのサイズ(X)
	const int STAGE_HEIGHT{ 10 };//ステージのサイズ(Y)
	const int STAGE_LEVEL{ 10 };//ステージのサイズ(Z)

	CsvReaderYZ r;

}

using std::vector;

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), hBlock_(-1), AdjustLevel_(0.5f), stageAlign_(STAGE_LEVEL,
		std::vector<std::vector<int>>(
			STAGE_HEIGHT,
			std::vector<int>(STAGE_WIDTH, 0)
		))
{
	hBlock_ = Model::Load("BoxGrass.fbx");
	r.Load("datas2.csv");
}

void Stage::Initialize()
{
	
	Ghost* pGhost = nullptr;
	TurretEnemy* pTurret = nullptr;

	KeyFlag* pKey = nullptr;
	GoalDoor* pGoal = nullptr;
	ResidueItem* pResudue = nullptr;
	PlayerBlock* pPlayerBlock = nullptr;


	for (int z = 0; z < STAGE_LEVEL; z++)
	{
		for (int j = 0; j < STAGE_HEIGHT; j++)
		{
			std::string nums = std::format("#{}Layer {}", z, j);
			bool t = r.GetParamLine(nums);
			for (int i = 0; i < STAGE_WIDTH; i++)
			{
				int value = r.PopParamInt();
				stageAlign_[z][j][i] = value;

				//ステージ上にオブジェクトを出現させる処理
				switch (value)
				{
				case EMPTY:
					break;
				case ENEMY_GHOST: // ゴースト
					pGhost = Instantiate<Ghost>(this);
					pGhost->SetPosition(i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f));
					break;

				case ENEMY_TURRET: // 砲台の敵
					pTurret = Instantiate<TurretEnemy>(this);
					pTurret->SetPosition(i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f));
					break;

				case KEY: // ゴール用のカギ
					pKey = Instantiate<KeyFlag>(this);
					pKey->SetBasePosition(i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f));

					break;

				case GOAL: // ドア
					pGoal = Instantiate<GoalDoor>(this);
					pGoal->SetPosition(i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f));
					break;
				case STAGE_BLOCK:
					break;
				case RESIDUE:
					pResudue = Instantiate<ResidueItem>(this);
					pResudue->SetBasePosition(i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f));
					break;

				//PLAYER_BLOCKのみ少し特殊
				case PLAYER_BLOCK:
					pPlayerBlock = Instantiate<PlayerBlock>(this);
					pPlayerBlock->SetPosition(i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f));
					break;

				default: 
					value = EMPTY;
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
				int value = stageAlign_[z][j][i];

				if (value == STAGE_BLOCK)
				{
					Transform t;
					t.position_ = { i - 5.0f, z * 1.0f + AdjustLevel_, j - 5.0f };
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
