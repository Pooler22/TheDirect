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
	ScreenManager(std::wstring nameIn, float screenWidth, float screenHeight, float scaleX, float scaleY, ID3D11ShaderResourceView* buttonSpriteSheet, std::shared_ptr<SpriteFont> textSprite, ID3D11ShaderResourceView* backgorund)
	{
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->nameCurrentScreen = nameIn;
		this->buttonSpriteSheet = buttonSpriteSheet;
		this->textSprite = textSprite;
		this->screens = std::vector<std::shared_ptr<Screen>>();
		this->game.reset(new Game(screenWidth, screenHeight, scaleX, scaleY));
		this->background.reset(new ScrollingBackground(backgorund, screenWidth, screenHeight));
	}

	void addScreen(Screen* screen)
	{
		this->screens.push_back(std::shared_ptr<Screen>(screen));
	}

	void addScreen(std::wstring nameIn, int size, std::wstring name[], std::wstring id[], XMFLOAT2 position[])
	{
		Screen* screen = new Screen(buttonSpriteSheet, textSprite, nameIn, scaleX, scaleY);
		screen->addMenu(name, id, position, size);
		this->screens.push_back(std::shared_ptr<Screen>(screen));
	}

	void addBrickTexture(ID3D11ShaderResourceView* spriteSheet)
	{
		this->game->addBrickTexture(spriteSheet);
	}

	void addBrickTexture2(ID3D11ShaderResourceView* spriteSheet)
	{
		this->game->addBrickTexture2(spriteSheet);
	}

	void addPlayer(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, ID3D11ShaderResourceView* shotSpriteSheet)
	{
		game->addPlayer(buttonSpriteSheet, positionIn, shotSpriteSheet);
	}

	void addEnemy(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, int i)
	{
		game->addEnemy(buttonSpriteSheet, positionIn, i);
	}

	void addBonus(ID3D11ShaderResourceView* buttonSpriteSheet, DirectX::XMFLOAT2 positionIn, std::shared_ptr<Skill> bonus)
	{
		game->addBonus(buttonSpriteSheet, positionIn, bonus);
	}

	void setMapLevel(int x, int y, int* numberTestureVector,ID3D11ShaderResourceView* playerSpriteSheetIn, std::shared_ptr<SpriteFont> spriteFontIn)
	{
		this->game->setMapLevel(x,y, numberTestureVector, playerSpriteSheetIn, spriteFontIn);
	}

	void Update(float elapsed)
	{
		background->Update(elapsed);
		if (nameCurrentScreen.compare(L"Play") == 0)
			game->Update(elapsed);
		for (auto &screen : this->screens)
		{
			if (screen->getName().compare(nameCurrentScreen) == 0)
				screen->Update(elapsed);
		}
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		background->Draw(batch);
		if (nameCurrentScreen.compare(L"Play") == 0)
			game->Draw(batch);
		for (auto &screen : this->screens)
		{
			if(screen->getName().compare(nameCurrentScreen) == 0)
				screen->Draw(batch);
		}
	}

	void setName(std::wstring string)
	{
		this->nameCurrentScreen = string;
	}

	std::wstring getName()
	{
		return this->nameCurrentScreen;
	}

	std::wstring isClicked(float x, float y)
	{
		for (auto &screen : this->screens)
		{
			if (screen->getName() == nameCurrentScreen)
				return screen->isClicked(x, y);
		}
		return L"false";
	}

	void setString(std::wstring nameScreen, std::wstring idButton, std::wstring stinrg)
	{
		for (auto &screen : this->screens)
		{
			if (screen->getName() == nameScreen)
				screen->setString(idButton, stinrg);
		}
	}

	void resize(float scaleX, float scaleY)
	{
		//TODO background->resize(); 
		game->resize(scaleX, scaleY);
		for (auto &screen : this->screens)
		{
			screen->resize(scaleX, scaleY);
		}
	}

	bool gameOver()
	{
		return this->game->gameOver();
	}

	bool win()
	{
		return this->game->win();
	}

	void resetLevel()
	{
		this->game->resetLevel();
	}

public:
	float										scaleX;
	float										scaleY;
	float										screenWidth;
	float										screenHeight;
	ID3D11ShaderResourceView*					buttonSpriteSheet;
	std::shared_ptr<SpriteFont>					textSprite;

	std::wstring								nameCurrentScreen;
	std::unique_ptr<Game>						game;
	std::vector<std::shared_ptr<Screen>>		screens;
	std::unique_ptr<ScrollingBackground>		background;
};