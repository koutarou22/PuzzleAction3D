#include "KeyImage.h"
#include "Engine/Image.h"
#include "Engine/SceneManager.h"
#include <cassert>

#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "Player.h"

namespace
{
    const float IMAGE_SCALE = 0.6f;
    const float INITIAL_POS_X = 0.4f;
    const float INITIAL_POS_Y = 0.8f;
    const float INITIAL_POS_Z = 0.0f;
}

KeyImage::KeyImage(GameObject* parent)
    : GameObject(parent, "KeyImage"),
    posX(2.0f), posY(INITIAL_POS_Y), posZ(INITIAL_POS_Z)
{
}

KeyImage::~KeyImage()
{
}

void KeyImage::Initialize()
{
    hKeyImage_[0] = Image::Load("Image/Image_Key.png");
    hKeyImage_[1] = Image::Load("Image/Image_GetKey.png");

    assert(hKeyImage_[0] >= 0);

    transform_.scale_ = { IMAGE_SCALE, IMAGE_SCALE, IMAGE_SCALE };
    transform_.position_ = { INITIAL_POS_X, posY, posZ };
}

void KeyImage::Update()
{
}

void KeyImage::Draw()
{

}

void KeyImage::Release()
{
}
