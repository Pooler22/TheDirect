#pragma once

#include <wrl.h>
#include <future>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Map.h"
#include "TextButton.h"

class Screen
{
public:

	Screen(ID3D11ShaderResourceView* buttonSpriteSheetIn, SpriteFont* spriteFontIn, std::wstring nameIn)
	{
		buttonSpriteSheet = buttonSpriteSheetIn;
		spriteFont = spriteFontIn;
		buttons = std::vector<std::shared_ptr<TextButton>>();
		name = nameIn;
	};

	void addButton(std::wstring screen, std::wstring id, XMFLOAT2 position)
	{
		buttons.push_back(std::shared_ptr<TextButton>(new TextButton(buttonSpriteSheet, spriteFont, screen, id, position)));
	}

	void addMenu(std::wstring* screen, std::wstring* id, XMFLOAT2* position, int count)
	{
		for (int i = 0; i < count; i++)
		{
			buttons.push_back(std::shared_ptr<TextButton>(new TextButton(buttonSpriteSheet, spriteFont, screen[i], id[i], position[i])));
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

	void resize(float scale)
	{
		for (auto &button : buttons) 
		{
			button->resize(scale);
		}
	}

private:
	ID3D11ShaderResourceView*					buttonSpriteSheet;
	SpriteFont*									spriteFont;
	std::wstring								name;
	std::vector<std::shared_ptr<TextButton>>	buttons;
};