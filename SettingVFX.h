#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>

using std::string;

using std::vector;

class SettingVFX
{
public:



	enum VFX_NAME
	{
		VFX_MAGIC,//魔法陣
		VFX_DUST, //砂埃
		VFX_STER, //星
		VFX_FLASH_R,//赤い光
		VFX_FLASH_B,//青い光
		VFX_FLASH_Y,//黄色い光
		VFX_MAX,
	};

	/*設定する必要がある項目(例)
	 
　　　　EmitterData data;//エミッターデータの構造体
		data.textureFileName = "cloudA.png";//テクスチャファイル名
		data.position = XMFLOAT3(-4, 4, 4);//位置
		data.positionRnd = XMFLOAT3(0.1, 0, 0.1);//位置のランダム幅
		data.delay = 5;//発生までの遅延時間(フレーム)
		data.number = 1;//発生数
		data.lifeTime = 60;//寿命(フレーム)
		data.gravity = -0.002f;//重力
		data.direction = XMFLOAT3(0, 1, 0);//方向
		data.directionRnd = XMFLOAT3(0, 0, 0);//方向のランダム幅
		data.speed = 0.01f;//速度
		data.speedRnd = 0.0;//速度のランダム幅
		data.size = XMFLOAT2(1.5, 1.5);//サイズ
		data.sizeRnd = XMFLOAT2(0.4, 0.4);//サイズのランダム幅
		data.scale = XMFLOAT2(1.01, 1.01);//拡大率
		data.color = XMFLOAT4(1, 1, 0, 1);//色
		data.deltaColor = XMFLOAT4(0, -0.03, 0, -0.02);//色の変化率
		VFX::Start(data);//エフェクトの開始
	
	*/


	//これを番号指定(位置指定やその他詳細情報は省く)だけでほぼ扱えるようにしたい
	static void SetVFX(int vfxnum, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT3 dir);

	static const std::string VFXName_[VFX_MAX];

	static void StopVFX(int handle);


};

