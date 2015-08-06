#pragma once

#include <wrl.h>
#include <future>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Map.h"
//#include "..\Common\DirectXHelper.h"	// For ThrowIfaFailed and ReadDataAsync

class Screen
{
public:

	Screen(ID3D11ShaderResourceView* playerSpriteSheetIn, SpriteFont* spriteFontIn, std::wstring nameIn)
	{
		playerSpriteSheet = playerSpriteSheetIn;
		spriteFont = spriteFontIn;

		buttons = std::vector<std::shared_ptr<TextButton>>();
		name = nameIn;
	};
	~Screen() {};

	void addElement(TextButton* button)
	{
		buttons.push_back(std::shared_ptr<TextButton>(button));
	}

	void addElement(std::wstring s1, std::wstring s2, XMFLOAT2 position)
	{
		buttons.push_back(std::shared_ptr<TextButton>(new TextButton(playerSpriteSheet, spriteFont, s1, s2, position)));
	}

	void addMenu(std::wstring* s1, std::wstring* s2, XMFLOAT2* position, int size)
	{
		for (int i = 0; i < size; i++)
		{
			buttons.push_back(std::shared_ptr<TextButton>(new TextButton(playerSpriteSheet, spriteFont, s1[i], s2[i], position[i])));
		}
	}

	void Update(float elapsed)
	{
		for (auto &button : buttons)
		{
			button->Update(elapsed);
		}
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		for (auto &button : buttons)
		{
			button->Draw(batch);
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
		for (auto &button : buttons)
		{
			if (button->isOver(x, y))
			{
				return button->getId();
			}
		}
		return L"false";
	}

	void resize(float x, float y)
	{
		for (auto &button : buttons) 
		{
			button->resize(x, y);
		}
	}

private:
	ID3D11ShaderResourceView*					playerSpriteSheet;
	SpriteFont*									spriteFont;

	std::wstring								name;
	std::vector<std::shared_ptr<TextButton>>		buttons;
};