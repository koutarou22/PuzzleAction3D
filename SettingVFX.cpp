#include "SettingVFX.h"
#include "Engine/VFX.h"


const std::string SettingVFX::VFXName_[VFX_MAX] = {
    "PaticleAssets//bubble.png",
    "PaticleAssets//cloudD.png",
    "PaticleAssets//cloudB.png",
    "PaticleAssets//flashB_B.png"
};


int SettingVFX::SetVFX(int vfxnum, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 size, DirectX::XMFLOAT3 dir)
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

    switch (vfxnum)
    {
    case VFX_BUBBLE:
        data.speed = 0.01f;
        break;
    case VFX_CLOUD_B:
        data.speed = 0.005f;
        break;
    case VFX_FLASH_B:
        data.speed = 0.02f;
        break;
    default:
        break;
    }

    VFX::Start(data);

    return VFX::Start(data);
}

void SettingVFX::StopVFX(int handle)
{
    if (handle >= 0) {
        VFX::End(handle);
    }
}

