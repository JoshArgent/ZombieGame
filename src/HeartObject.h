#pragma once
#include "CollectableObject.h"

class HeartObject : public CollectableObject
{
public:
	HeartObject(PlayState* playState, int startX, int startY);
	~HeartObject();
private:
	void Collect();
	inline std::vector<ImageData*>& GetSprites();
};

