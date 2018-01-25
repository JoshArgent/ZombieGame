#include "RoundCompleteState.h"
#include "Psyja2Engine.h"
#include "PlayState.h"
#include <sstream>
#include <algorithm>

RoundCompleteState::RoundCompleteState(Psyja2Engine *engine, PlayState *oldState, PlayState *nextPlayState) : State(engine), oldState(oldState), nextPlayState(nextPlayState)
{
	engine->SetupBackgroundBuffer();
	engine->Redraw(true);

	std::ostringstream ss;
	ss << "Round " << std::to_string(oldState->round) << " Complete!";
	msg1 = ss.str();
	ss.str("");
	ss << "Starting Round " << std::to_string(nextPlayState->round) << "...";
	msg2 = ss.str();
}


RoundCompleteState::~RoundCompleteState()
{
}

void RoundCompleteState::SetupBackgroundBuffer()
{
	oldState->SetupBackgroundBuffer();
}

void RoundCompleteState::DrawStrings()
{
	oldState->DrawStrings();
}

void RoundCompleteState::DrawStringsOnTop()
{
	Font *font = engine->GetFont("Millimetre-Bold.otf", 30);
	if (messageState < msg1.length() + 20)
	{
		std::string subStr = msg1.substr(0, std::min(messageState, (int) msg1.length()));
		engine->DrawScreenString(engine->GetScreenWidth() / 2 - 120 + 2, engine->GetScreenHeight() / 2 + 2, subStr.c_str(), 0x000000, font);
		engine->DrawScreenString(engine->GetScreenWidth() / 2 - 120, engine->GetScreenHeight() / 2, subStr.c_str(), 0xFFFFFF, font);
	}
	else
	{
		std::string subStr = msg2.substr(0, std::min((int) (messageState - msg1.length() - 20), (int) msg2.length()));
		engine->DrawScreenString(engine->GetScreenWidth() / 2 - 120 + 2, engine->GetScreenHeight() / 2 + 2, subStr.c_str(), 0x000000, font);
		engine->DrawScreenString(engine->GetScreenWidth() / 2 - 120, engine->GetScreenHeight() / 2, subStr.c_str(), 0xFFFFFF, font);
	}	
}

void RoundCompleteState::GameAction()
{
	// If too early to act then do nothing
	if (!engine->IsTimeToActWithSleep())
		return;

	// Only act after 0.05 seconds
	engine->SetTimeToAct(50);
	engine->Redraw(true);
	messageState++;

	// If message finished been displayed start the next round
	if (messageState > msg1.length() + msg2.length() + 40)
	{
		engine->SetTimeToAct(15);
		delete oldState;
		engine->SwitchState(nextPlayState, false, false);
	}

	// Pause all object movements
	//engine->UpdateAllObjects(engine->GetTime());
}