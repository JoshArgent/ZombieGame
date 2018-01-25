#pragma once
#include "CollectableObject.h"
#include "Psyja2Engine.h"
#include "PlayState.h"
#include <sstream>

CollectableObject::CollectableObject(PlayState *playState, int startX, int startY) : TileObject(playState, startX, startY)
{
	m_iDrawWidth = 16;
	m_iDrawHeight = 16;
}

CollectableObject::~CollectableObject()
{
}

void CollectableObject::DoUpdate(int iCurrentTime)
{
	// Animate the collectable object
	i++;
	if (i == 5)
	{
		i = 0;
		state++;
		if (state == GetSprites().size())
			state = 0;
	}
}

void CollectableObject::Draw()
{
	// Do not draw if it should not be visible
	if (!IsVisible())
		return;

	ImageData *sprite = GetSprites()[state];
	sprite->RenderImageWithMask(playState->engine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, sprite->GetWidth(), sprite->GetHeight());

	StoreLastScreenPositionForUndraw();
}

void CollectableObject::CollectObject()
{
	Collect();
	SetVisible(false);
	RedrawBackground();
	playState->engine->RemoveObjectFromArray(this);
}

void CollectableObject::Collect()
{
	// Do nothing
}

inline std::vector<ImageData*>& CollectableObject::GetSprites()
{
	// Do nothing
	return *(new std::vector<ImageData*>);
}
