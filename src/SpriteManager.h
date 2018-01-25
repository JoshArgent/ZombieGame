#pragma once
#include "JPGImage.h"
#include <vector>

class SpriteManager
{
public:
	SpriteManager();
	~SpriteManager();

	inline std::vector<ImageData*>& GetCoins()
	{
		return *coins;
	}

	inline std::vector<ImageData*>& GetTiles()
	{
		return *tiles;
	}

	inline ImageData* GetBlood()
	{
		return blood;
	}

	inline std::vector<ImageData*>& GetHeart()
	{
		return *heart;
	}

	inline ImageData* GetHelp()
	{
		return help;
	}

	inline ImageData* GetMenu()
	{
		return menu;
	}

private:
	std::vector<ImageData*> *coins;
	std::vector<ImageData*> *tiles;
	ImageData *blood;
	std::vector<ImageData*> *heart;
	ImageData *help;
	ImageData *menu;
};

