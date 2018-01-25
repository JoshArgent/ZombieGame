#pragma once
#include "CollectableObject.h"
class CoinObject : public CollectableObject
{
public:
	CoinObject(PlayState *playState, int startX, int startY);
	~CoinObject();
private:
	void Collect();
	inline std::vector<ImageData*>& GetSprites();
};

