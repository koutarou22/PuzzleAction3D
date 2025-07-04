#include "UI.h"
#include "Engine/Image.h"
#include "KeyImage.h"
#include "TextImage.h"
#include "Residue.h"

UI::UI(GameObject* parent) :GameObject(parent, "UI")
{
}

UI::~UI()
{
}

void UI::Initialize()
{
	//Instantiate<KeyImage>(this);
	Instantiate<TextImage>(this);
	Instantiate<Residue>(this);
}

void UI::Update()
{

}

void UI::Draw()
{
	
}

void UI::Release()
{
}

