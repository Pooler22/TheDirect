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
	ScreenManager(std::wstring nameIn, float widht, float height)
	{
		screens = std::vector<std::shared_ptr<Screen>>();
		game.reset(new Game(widht, height));
		nameCurrentScreen = nameIn;
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

	void setMapLevel(int x, int y, int* numberTestureVector, int screenWidth, int screenHeight, float scale, ID3D11ShaderResourceView* playerSpriteSheetIn, std::shared_ptr<SpriteFont> spriteFontIn)
	{
		game->setMapLevel(x,y, numberTestureVector, screenWidth, screenHeight, scale, playerSpriteSheetIn, spriteFontIn);
	}

	void Update(float elapsed)
	{
		if (nameCurrentScreen.compare(L"Play") == 0)
			game->Update(elapsed);
		for (auto &screen : screens)
		{
			if (screen->getName().compare(nameCurrentScreen) == 0)
				screen->Update(elapsed);
		}
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		if (nameCurrentScreen.compare(L"Play") == 0)
			game->Draw(batch);
		for (auto &screen : screens)
		{
			if(screen->getName().compare(nameCurrentScreen) == 0)
				screen->Draw(batch);
		}
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

	void setString(std::wstring nameScreen, std::wstring idButton, std::wstring stinrg)
	{
		for (auto &screen : screens)
		{
			if (screen->getName() == nameScreen)
				screen->setString(idButton, stinrg);
		}
	}

	void resize(float scale)
	{
		game->resize(scale);
		for (auto &screen : screens)
		{
			screen->resize(scale);
		}
	}

	bool gameOver()
	{
		return game->gameOver();
	}

	void resetLevel()
	{
		game->resetLevel();
	}

public:
	std::wstring								nameCurrentScreen;
	std::vector<std::shared_ptr<Screen>>		screens;
	std::unique_ptr<Game>						game;
};