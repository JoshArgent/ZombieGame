#include "MenuState.h"
#include "Psyja2Engine.h"
#include "JPGImage.h"
#include "PlayState.h"
#include "Button.h"
#include "HelpState.h"
#include "SpriteManager.h"

MenuState::MenuState(Psyja2Engine *engine) : State(engine)
{
}

MenuState::~MenuState()
{
}

void MenuState::SetupBackgroundBuffer()
{
	engine->spriteManager->GetMenu()->RenderImage(engine->GetBackground(), 0, 0, 0, 0, engine->GetScreenWidth(), engine->GetScreenHeight());
}

int MenuState::InitialiseObjects()
{
	engine->DrawableObjectsChanged();
	engine->DestroyOldObjects();
	engine->CreateObjectArray(10);

	playButton = new Button(engine, "Play", 300, 300);
	engine->StoreObjectInArray(playButton);

	helpButton = new Button(engine, "Instructions", 300, 360);
	engine->StoreObjectInArray(helpButton);

	return 1;
}

void MenuState::MouseMoved(int iX, int iY)
{

}

void MenuState::MouseDown(int iButton, int iX, int iY)
{

}

void MenuState::DrawStrings()
{
	// Shadow text:
	Font *font = engine->GetFont("Millimetre-Bold.otf", 50);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 120 + 5, 30 + 5, "Outbreak", 0x000000, font);
	engine->DrawScreenString(engine->GetScreenWidth() / 2 - 120, 30, "Outbreak", 0xFFFFFF, font);
}

void MenuState::GameAction()
{
	// If too early to act then do nothing
	if (!engine->IsTimeToActWithSleep())
		return;

	// Don't act for another 15 ticks
	engine->SetTimeToAct(15);

	// Pause all object movements
	engine->UpdateAllObjects(engine->GetTime());

	// See if the play button has been pressed
	if (playButton->IsPressed())
	{
		// Start playing
		PlayState *nextState = new PlayState(engine, 0, 3, 1, engine->levels[engine->NextLevel()].c_str());
		engine->SwitchState(nextState, false, false);
		return;
	}

	// See if the help button has been pressed
	if (helpButton->IsPressed())
	{
		// Start playing
		HelpState *nextState = new HelpState(engine);
		engine->SwitchState(nextState, false, false);
		return;
	}

}
