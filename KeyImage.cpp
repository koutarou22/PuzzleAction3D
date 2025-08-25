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
    float INITIAL_POS_X = 2.0f;
    float INITIAL_POS_Y = 0.8f;
    float INITIAL_POS_Z = 0.0f;

	const float OFFSET_X = 0.2f;
}

KeyImage::KeyImage(GameObject* parent)
    : GameObject(parent, "KeyImage"),
    posX(INITIAL_POS_X), posY(INITIAL_POS_Y), posZ(INITIAL_POS_Z)
{
}

KeyImage::~KeyImage()
{
}

void KeyImage::Initialize()
{
    hKeyImage_[KEY_IMAGE_NO_KEY] = Image::Load("Image/Image_Key.png");
    hKeyImage_[KEY_IMAGE_KEY] = Image::Load("Image/Image_GetKey.png");

	//“o˜^‚³‚ê‚Ä‚¢‚é‚©Šm”F
    assert(hKeyImage_[KEY_IMAGE_NO_KEY] >= 0);
	assert(hKeyImage_[KEY_IMAGE_KEY] >= 0);

    transform_.scale_ = { IMAGE_SCALE, IMAGE_SCALE, IMAGE_SCALE };
    transform_.position_ = { posX, posY, posZ };
}

void KeyImage::Update()
{
}

void KeyImage::Draw()
{
	Player* pPlayer = (Player*)FindObject("Player");

	//Œ®‚ðŽ‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©‚Ìƒtƒ‰ƒO‚ðŽó‚¯Žæ‚é
    bool IsKey = pPlayer->GetClearFlag();

	// Žæ“¾‚µ‚Ä‚¢‚é or Žæ“¾‚µ‚Ä‚¢‚È‚¢‚Å‰æ‘œ‚ðØ‚è‘Ö‚¦‚é
    if (IsKey)
    {
		Image::Draw(hKeyImage_[KEY_IMAGE_KEY]);
        Image::SetTransform(hKeyImage_[KEY_IMAGE_KEY],transform_);
		assert(hKeyImage_[KEY_IMAGE_KEY] >= 0);
    }
    else
    {
		Image::Draw(hKeyImage_[KEY_IMAGE_NO_KEY]);
        Image::SetTransform(hKeyImage_[KEY_IMAGE_NO_KEY], transform_);
		assert(hKeyImage_[KEY_IMAGE_NO_KEY] >= 0);
    }
}

void KeyImage::Release()
{
}
