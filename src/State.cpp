#include "State.h"
#include "Psyja2Engine.h"

State::State(Psyja2Engine *engine) : engine(engine)
{
}

State::~State()
{
}

void State::SetupBackgroundBuffer()
{

}

int State::InitialiseObjects()
{
	engine->DrawableObjectsChanged();
	engine->DestroyOldObjects();
	engine->CreateObjectArray(1);
	return 0;
}

void State::MouseMoved(int iX, int iY)
{

}

void State::MouseDown(int iButton, int iX, int iY)
{

}

void State::DrawStrings()
{

}

void State::DrawStringsOnTop()
{

}

void State::GameAction()
{

}

void State::KeyDown(int iKeyCode)
{

}
