#pragma once
#include "Engine/GameObject.h"

enum KEY_IMAGE_TYPE
{
    KEY_IMAGE_NO_KEY = 0, //Œ®‚ğ‚Á‚Ä‚¢‚È‚¢ó‘Ô
    KEY_IMAGE_KEY,      //Œ®‚ğ‚Á‚Ä‚¢‚éó‘Ô
	KEY_IMAGE_MAX,      //Œ®‚Ìó‘Ô
};

/// <summary>
/// Œ®‚ğ“üè‚µ‚½‚©‚í‚©‚éUI
/// </summary>
class KeyImage : public GameObject
{
	int hKeyImage_[KEY_IMAGE_MAX];//Œ®‚Ìæ“¾ó‘Ô‚Ì‰æ‘œ‚ğŠi”[‚·‚é”z—ñ
    float posX, posY, posZ;//‰ŠúˆÊ’u
    bool GetChangeImageFlag;//Œ®‚ğ‚Á‚Ä‚é‚©‚Á‚Ä‚È‚¢‚©”»’è‚·‚éflag
public:
    KeyImage(GameObject* parent);
  
    ~KeyImage();

    void Initialize() override;

    void Update() override;

    void Draw() override;

    void Release() override;
};

