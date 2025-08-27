#include "Stage.h"
#include "Engine/Model.h"
#include "NcsvReader.h"
#include <format>
#include "Engine/Camera.h"
#include "Ghost.h"
#include "TurretEnemy.h"
#include "KeyFlag.h"
#include "GoalDoor.h"
#include "RemainItem.h"
#include "PlayerBlock.h"
#include "Player.h"

#include"Engine/SceneManager.h"

namespace
{
    const int STAGE_WIDTH = 10;      // ステージの横幅（X軸）
    const int STAGE_HEIGHT = 10;      // ステージの奥行き（Y軸）
    const int STAGE_LEVEL = 10;      // ステージの高さ（Z軸）

    const float BLOCK_POSITION_OFFSET_Y = 0.5f;          // Y軸補正（AdjustLevel_）
    const float BLOCK_SCALE_Y = 1.0f;          // Zレイヤー毎の高さスケール
    const float CENTER_OFFSET_X = STAGE_WIDTH / 2.0f;
    const float CENTER_OFFSET_Y = STAGE_HEIGHT / 2.0f;

    CsvReaderYZ r;
}

using std::vector;

Stage::Stage(GameObject* parent)
    : GameObject(parent, "Stage"),
    hBlock_(-1),
    AdjustLevel_(BLOCK_POSITION_OFFSET_Y),
    stageAlign_(
        STAGE_LEVEL,
        vector<vector<int>>(STAGE_HEIGHT, vector<int>(STAGE_WIDTH, 0))
    ), ChangeStageNumber_(-1)
{
    hBlock_ = Model::Load("BoxGrass.fbx");
	assert(hBlock_ >= 0);

    SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");

    if (pSceneManager != nullptr)
    {
		// ステージ番号を取得
        ChangeStageNumber_ = pSceneManager->GetStageNumber();

        switch (ChangeStageNumber_)
        {
        case STAGE1:
            r.Load("StageData\\datas.csv");
            break;
        case STAGE2:
            r.Load("StageData\\datas2.csv");
			break;
        case STAGE3:
            r.Load("StageData\\datas3.csv");
            break;
        case STAGE4:
            r.Load("StageData\\datas4.csv");
            break;

        case STAGE_MAX:
            // ステージ番号をリセット
            pSceneManager->ResetStageNumber();
            break;

        default:
       
            break;
        }
    }
}

void Stage::Initialize()
{
    
    Player     * pPlayer      = nullptr;
    Ghost      * pGhost       = nullptr;
    TurretEnemy* pTurret      = nullptr;
    KeyFlag    * pKey         = nullptr;
    GoalDoor   * pGoal        = nullptr;
    RemainItem* pResudue      = nullptr;
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

                //位置の調整
                float px = i - CENTER_OFFSET_X;
                float py = z + AdjustLevel_;
                float pz = j - CENTER_OFFSET_Y;

				// ステージのブロックを配置
                switch (value)
                {
                case ENEMY_GHOST:
                    pGhost = Instantiate<Ghost>(this);
                    pGhost->SetPosition(px, py, pz);
                    break;

                case ENEMY_TURRET:
                    pTurret = Instantiate<TurretEnemy>(this);
                    pTurret->SetPosition(px, py, pz);
                    break;

                case KEY:
                    pKey = Instantiate<KeyFlag>(this);
                    pKey->SetBasePosition(px, py, pz);
                    break;

                case GOAL:
                    pGoal = Instantiate<GoalDoor>(this);
                    pGoal->SetPosition(px, py, pz);
                    break;

                case REMAIN:
                    pResudue = Instantiate<RemainItem>(this);
                    pResudue->SetBasePosition(px, py, pz);
                    break;

                case PLAYER_BLOCK:
                    pPlayerBlock = Instantiate<PlayerBlock>(this);
                    pPlayerBlock->SetPosition(px, py, pz);
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
                    t.position_ = {
                        i - CENTER_OFFSET_X,
                        z * BLOCK_SCALE_Y + AdjustLevel_,
                        j - CENTER_OFFSET_Y
                    };
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
