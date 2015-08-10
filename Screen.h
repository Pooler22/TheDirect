#pragma once

#include <wrl.h>
#include <future>
#include <Content\AnimatedTexture.h>
#include <SpriteBatch.h>
#include <DirectXMath.h>
#include <DirectXTK\Inc\SimpleMath.h>
#include "SpriteFont.h"
#include "Map.h"

class Screen
{
public:

	Screen(ID3D11ShaderResourceView* playerSpriteSheetIn, SpriteFont* spriteFontIn, std::wstring nameIn)
	{
		buttonSpriteSheet = playerSpriteSheetIn;
		spriteFont = spriteFontIn;
		name = nameIn;
		buttons = std::vector<std::shared_ptr<TextButton>>();
	};

	void addButton(TextButton* button)
	{
		buttons.push_back(std::shared_ptr<TextButton>(button));
	}

	void addButton(std::wstring name, std::wstring id, XMFLOAT2 position, float scaleIn)
	{
		buttons.push_back(std::shared_ptr<TextButton>(new TextButton(buttonSpriteSheet, spriteFont, name, id, position, scaleIn)));
	}

	void addMenu(std::wstring* names, std::wstring* ids, XMFLOAT2* position, int size, float scale)
	{
		for (int i = 0; i < size; i++)
		{
			buttons.push_back(std::shared_ptr<TextButton>(new TextButton(buttonSpriteSheet, spriteFont, names[i], ids[i], position[i], scale)));
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

	void resize(float scale)
	{
		for (auto &button : buttons) 
		{
			button->resize(scale);
		}
	}

	void setString(std::wstring idButton, std::wstring stinrg)
	{
		for (auto &button : buttons)
		{
			if (button->getId() == idButton)
				button->setString(stinrg);
		}
	}

private:

	ID3D11ShaderResourceView*					buttonSpriteSheet;
	SpriteFont*									spriteFont;
	std::wstring								name;
	std::vector<std::shared_ptr<TextButton>>	buttons;
};