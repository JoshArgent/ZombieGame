#include "HealthBarObject.h"
#include "Psyja2Engine.h"
#include "PlayerObject.h"
#include "PlayState.h"
#include <sstream>

HealthBarObject::HealthBarObject(PlayState* playState) : DisplayableObject(playState->engine), playState(playState)
{
	m_iCurrentScreenX = m_iPreviousScreenX = 0;
	m_iCurrentScreenY = m_iPreviousScreenY = 0;
}

HealthBarObject::~HealthBarObject()
{
}

void HealthBarObject::Draw()
{
	double health = playState->getPlayer()->GetHealth();
	int intHealth = round(health * 100);
	int offset = (BASE_SCREEN_WIDTH / 2) - (BAR_WIDTH / 2);

	// Draw border
	playState->engine->DrawScreenRectangle(offset - 2, 13, offset + BAR_WIDTH + 2, 13 + BAR_HEIGHT + 4, 0x000000);
	// Draw health
	playState->engine->DrawScreenRectangle(offset, 15, offset + BAR_WIDTH, 15 + BAR_HEIGHT, 0x00FF00);
	// Draw negative health
	if (health < 100)
		playState->engine->DrawScreenRectangle(offset + health * BAR_WIDTH, 15, offset + BAR_WIDTH, 15 + BAR_HEIGHT, 0xFF0000);
	// Draw text percentage
	std::ostringstream ss;
	std::string str;
	ss << std::to_string(intHealth) << "%";
	playState->engine->DrawScreenString(offset + (BAR_WIDTH / 2) - 15, 14, ss.str().c_str(), 0x000000, NULL);

	StoreLastScreenPositionForUndraw();
}
