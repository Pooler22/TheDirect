#pragma once

#include <wrl.h>
#include <future>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Screen.h"
#include "Game.h"

class ScreenManager
{

public:
	ScreenManager()
	{
		screens = std::vector<std::shared_ptr<Screen>>();
		game = std::unique_ptr<Game>(new Game());
	}

	~ScreenManager() 
	{
	}

	void addScreen(Screen* screen)
	{
		screens.push_back(std::shared_ptr<Screen>(screen));
	}

	void addBrickTexture(ID3D11ShaderResourceView* spriteSheet)
	{
		game->addBrickTexture(spriteSheet);
	}

	void addBrickTexture2(ID3D11ShaderResourceView* spriteSheet)
	{
		game->addBrickTexture2(spriteSheet);
	}

	void setMapLevel(int x, int y, int* numberTestureVector, std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVector, int screenWidth, int screenHeight)
	{
		game->setMapLevel(x,y, numberTestureVector, baehaviorTestureVector,  screenWidth, screenHeight);
	}

	void Update(float elapsed)
	{
		for (auto &screen : screens)
		{
			if (screen->getName().compare(nameCurrentScreen) == 0)
				screen->Update(elapsed);
		}
		if (nameCurrentScreen.compare(L"Play") == 0)
			game->Update(elapsed);
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		for (auto &screen : screens)
		{
			if(screen->getName().compare(nameCurrentScreen) == 0)
				screen->Draw(batch);
		}
		if (nameCurrentScreen.compare(L"Play") == 0)
			game->Draw(batch);
	}

	void setName(std::wstring stringIn)
	{
		nameCurrentScreen = stringIn;
	}

	std::wstring getName()
	{
		return nameCurrentScreen;
	}

	std::wstring isClicked(float x, float y)
	{
		for (auto &screen : screens)
		{
			if (screen->getName() == nameCurrentScreen)
				return screen->isClicked(x, y);
		}
		return L"false";
	}
	void resize(float x, float y)
	{
		for (auto &screen : screens)
		{
			screen->resize(x, y);
		}
		game->resize(x, y);
	}

public:
	std::wstring								nameCurrentScreen;
	std::vector<std::shared_ptr<Screen>>		screens;
	std::unique_ptr<Game>						game;
};