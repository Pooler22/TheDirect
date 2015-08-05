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

	void addScreen(Screen* screen)
	{
		screens.push_back(std::shared_ptr<Screen>(screen));
	}

	void addBrickTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spriteSheet)
	{
		game->addBrickTexture(spriteSheet);
	}

	void addBrickTexture2(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> spriteSheet)
	{
		game->addBrickTexture2(spriteSheet);
	}

	void setMapLevel(XMFLOAT2 size, int* numberTestureVector, std::shared_ptr<std::vector<BRICK_BEHAVIOR>> baehaviorTestureVector, int screenWidth, int screenHeight)
	{
		game->setMapLevel(size, numberTestureVector, baehaviorTestureVector,  screenWidth, screenHeight);
	}

	void setName(std::wstring stringIn)
	{
		nameCurrentScreen = stringIn;
	}

	std::wstring getName()
	{
		return nameCurrentScreen;
	}

	void Update(float elapsed)
	{
		if (nameCurrentScreen.compare(L"Play") == 0)
			game->Update(elapsed);
		for (auto &screen : screens)
		{
			if (screen->getName().compare(nameCurrentScreen) == 0)
			{
				screen->Update(elapsed);
				break;
			}
		}
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		if (nameCurrentScreen.compare(L"Play") == 0)
			game->Draw(batch);
		for (auto &screen : screens)
		{
			if (screen->getName().compare(nameCurrentScreen) == 0)
			{
				screen->Draw(batch);
				break;
			}
		}
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
	void resize(float scale)
	{
		if (nameCurrentScreen.compare(L"Play") == 0)
			game->resize(scale);
		for (auto &screen : screens)
		{
			screen->resize(scale);
		}
	}

public:
	std::wstring								nameCurrentScreen;
	std::unique_ptr<Game>						game;
	std::vector<std::shared_ptr<Screen>>		screens;
};