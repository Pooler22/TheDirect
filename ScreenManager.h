#pragma once

#include <wrl.h>
#include <future>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Screen.h"
#include "Game.h"
//#include "..\Common\DirectXHelper.h"	// For ThrowIfaFailed and ReadDataAsync

class ScreenManager
{
public:
	ScreenManager()
	{
		screens = std::vector<std::shared_ptr<Screen>>();
		game = std::unique_ptr<Game>(new Game());
	};
	~ScreenManager() {};
	void addScreen(Screen* screen)
	{
		screens.push_back(std::shared_ptr<Screen>(screen));
	}

	void addBrickTexture(ID3D11ShaderResourceView* playerSpriteSheet)
	{
		game->addBrickTexture(playerSpriteSheet);
	}

	void setMapLevel(XMFLOAT2 size, std::shared_ptr<std::vector<int>> numberTestureVector, std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVector)
	{
		game->setMapLevel(size, numberTestureVector, baehaviorTestureVector);
	}

	void Update(float elapsed)
	{
		for (auto &screen : screens)
		{
			if (screen->getName().compare(name) == 0)
				screen->Update(elapsed);
		}
		if (name.compare(L"Play") == 0)
		{
			game->Update(elapsed);
		}
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		for (auto &screen : screens)
		{
			if(screen->getName().compare(name) == 0)
				screen->Draw(batch);
		}
		if (name.compare(L"Play") == 0)
		{
			game ->Draw(batch);
		}
	}

	void setName(std::wstring stringIn)
	{
		name = stringIn;
	}

	std::wstring getName()
	{
		return name;
	}

	std::wstring isClicked(float x, float y)
	{
		for (auto &screen : screens)
		{
			if (screen->getName() == name)
			{
				return screen->isClicked(x, y);
			}
		}
		return L"false";
	}
	void updatePosition(float x, float y)
	{
		for (auto &screen : screens)
		{
			screen->updatePosition(x, y);
		}
	}

private:
	std::wstring								name;
	std::vector<std::shared_ptr<Screen>>		screens;
	std::unique_ptr<Game>						game;
};