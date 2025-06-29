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
#include "Player.h"

namespace {
	const int STAGE_WIDTH{ 10 };//ステージのサイズ(X)
	const int STAGE_HEIGHT{ 10 };//ステージのサイズ(Y)
	const int STAGE_LEVEL{ 10 };//ステージのサイズ(Z)

	const int GRID_WIDTH = 10;
	const int GRID_HEIGHT = 10;
	const int MAX_STAGE_HEIGHT = 10;
	const int GRID_OFFSET_X = 5;
	const int GRID_OFFSET_Z = 4;


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
	r.Load("datas.csv");
}

void Stage::Initialize()
{

	Player* pPlayer = nullptr;
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
				case ENEMY_GHOST:
				{
					pGhost = Instantiate<Ghost>(this);
					XMFLOAT3 pos = { i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f) };
					pGhost->SetPosition(pos.x, pos.y, pos.z);
					break;
				}

				case ENEMY_TURRET:
				{
					pTurret = Instantiate<TurretEnemy>(this);
					XMFLOAT3 pos = { i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f) };
					pTurret->SetPosition(pos.x, pos.y, pos.z);
					break;
				}

				case KEY:
				{
					pKey = Instantiate<KeyFlag>(this);
					XMFLOAT3 pos = { i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f) };
					pKey->SetBasePosition(pos.x, pos.y, pos.z);
					break;
				}

				case GOAL:
				{
					pGoal = Instantiate<GoalDoor>(this);
					XMFLOAT3 pos = { i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f) };
					pGoal->SetPosition(pos.x, pos.y, pos.z);
					break;
				}

				case RESIDUE:
				{
					pResudue = Instantiate<ResidueItem>(this);
					XMFLOAT3 pos = { i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f) };
					pResudue->SetBasePosition(pos.x, pos.y, pos.z);

					break;
				}

				case PLAYER_BLOCK:
				{
					pPlayerBlock = Instantiate<PlayerBlock>(this);
					XMFLOAT3 pos = { i - (STAGE_WIDTH / 2.0f), z + AdjustLevel_, j - (STAGE_HEIGHT / 2.0f) };
					pPlayerBlock->SetPosition(pos.x, pos.y, pos.z);

					break;
				}


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

float Stage::GetGroundHeight(float x, float z)
{
	int gx = static_cast<int>(x + GRID_OFFSET_X);
	int gy = static_cast<int>(GRID_OFFSET_Z - z);

	if (gx < 0 || gx >= GRID_WIDTH || gy < 0 || gy >= GRID_HEIGHT)
		return 0.0f;

	for (int y = MAX_STAGE_HEIGHT - 1; y >= 0; --y)
	{
		int current = stageAlign_[y][GRID_HEIGHT - 1 - gy][gx];
		if (current == 5 || current == 2 || current == 7)
		{
			return y;
		}
	}
	return 0.0f;
}
