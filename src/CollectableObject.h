#pragma once
#include "TileObject.h"
#include "JPGImage.h"

class CollectableObject : public TileObject
{
public:
	CollectableObject(PlayState *playState, int startX, int startY);
	~CollectableObject();
	void DoUpdate(int iCurrentTime);
	void Draw();
	/* Removes the object and calls Collect() */
	void CollectObject();
private:
	int state = 0;
	int i = 0;
	/* Override: Actually performs the collection - eg. increment score */
	virtual void Collect();
	/* Override: Load the required image graphics */
	inline virtual std::vector<ImageData*>& GetSprites();
};
