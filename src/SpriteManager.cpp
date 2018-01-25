#include "SpriteManager.h"
#include <string>
#include <sstream>

SpriteManager::SpriteManager()
{
	// Coins
	coins = new std::vector<ImageData*>;
	for (int i = 1; i < 9; i++)
	{
		std::ostringstream ss;
		ss << "coin/" << std::to_string(i) << ".png";
		ImageData *img = new ImageData();
		ImageData tmp = ImageData();
		tmp.LoadImage(ss.str().c_str());
		img->ShrinkFrom(&tmp, 3);
		coins->push_back(img);
	}

	// Tiles
	tiles = new std::vector<ImageData*>;
	for (int i = 0; i < 13; i++)
	{
		std::ostringstream ss;
		ss << "tiles/" << std::to_string(i) << ".jpg";
		ImageData *img = new ImageData();
		img->LoadImage(ss.str().c_str());
		tiles->push_back(img);
	}

	// Blood
	blood = new ImageData();
	blood->LoadImage("blood.png");

	// Heart - must be a vector for the CollectableObject class to use
	heart = new std::vector<ImageData*>;
	ImageData *heartImg = new ImageData();
	heartImg->LoadImage("heart.png");
	heart->push_back(heartImg);

	// Help
	help = new ImageData();
	help->LoadImage("help.jpg");

	// Menu
	menu = new ImageData();
	menu->LoadImage("menu.jpg");
}

SpriteManager::~SpriteManager()
{
	for (ImageData* i : *coins)
		delete i;
	delete coins;
	for (ImageData* i : *tiles)
		delete i;
	delete tiles;
	delete blood;
	delete heart->at(0);
	delete heart;
	delete help;
	delete menu;
}
