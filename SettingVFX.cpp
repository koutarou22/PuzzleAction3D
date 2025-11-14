#include "SettingVFX.h"
#include "Engine/VFX.h"


const std::string SettingVFX::VFXName_[VFX_MAX] = {
    "PaticleAssets//magic_A.png",  //–‚–@w
    "PaticleAssets//cloudD.png",   //»šº•—
    "PaticleAssets//star.png",     //¯
    "PaticleAssets//flashB_R.png", //Ô‚¢Œõ
    "PaticleAssets//flashB_B.png", //Â‚¢Œõ
    "PaticleAssets//flashB_Y.png", //‰©F‚¢Œõ
};


void SettingVFX::SetVFX(int vfxnum, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT3 dir)
{
    assert(vfxnum >= 0 && vfxnum < VFX_MAX);//”O‚Ì‚½‚ß“ü‚ê‚Ä‚¨‚­

    EmitterData data;
    data.textureFileName = VFXName_[vfxnum];
    data.position = pos;
    data.direction = dir;
    data.size = size;

    // ‹¤’ÊÝ’è
    data.lifeTime = 60;
    data.number = 1;
    data.color = { 1, 1, 1, 1 };
	data.delay = 0;
    data.lifeTime = 20;

    switch (vfxnum)
    {
    case VFX_MAGIC:
        data.speed = 0.01f;
        break;
    case VFX_DUST:
        data.speed = 0.005f;
        break;
    case VFX_STER:
        data.speed = 0.02f;
        break;
    case VFX_FLASH_R:
        break;
    case VFX_FLASH_B:
        break;
    case VFX_FLASH_Y:
	
        break;
    default:
        break;
    }

    VFX::Start(data);
}

void SettingVFX::StopVFX(int handle)
{
    if (handle >= 0)
    {
        VFX::End(handle);
    }
}

