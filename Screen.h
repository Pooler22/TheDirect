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

	Screen(ID3D11ShaderResourceView* playerSpriteSheetIn, std::shared_ptr<DirectX::SpriteFont>  spriteFontIn, std::wstring nameIn)
	{
		this->buttonSpriteSheet = playerSpriteSheetIn;
		this->spriteFont = spriteFontIn;
		this->name = nameIn;
		this->buttons = std::vector<std::shared_ptr<TextButton>>();
	};

	void addButton(TextButton* button)
	{
		this->buttons.push_back(std::shared_ptr<TextButton>(button));
	}

	void addButton(std::wstring name, std::wstring id, XMFLOAT2 position, float scaleX, float scaleY)
	{
		this->buttons.push_back(std::shared_ptr<TextButton>(new TextButton(buttonSpriteSheet, spriteFont, name, id, position, scaleX, scaleY)));
	}

	void addMenu(std::wstring* names, std::wstring* ids, XMFLOAT2* position, int size, float scaleX, float scaleY)
	{
		for (int i = 0; i < size; i++)
		{
			this->buttons.push_back(std::shared_ptr<TextButton>(new TextButton(buttonSpriteSheet, spriteFont, names[i], ids[i], position[i], scaleX, scaleY)));
		}
	}

	void Update(float elapsed)
	{
		for (auto &button : this->buttons)
		{
			button->Update(elapsed);
		}
	}

	void Draw(DirectX::SpriteBatch* batch)
	{
		for (auto &button : this->buttons)
		{
			button->Draw(batch);
		}
	}

	void setName(std::wstring stringIn)
	{
		this->name = stringIn;
	}

	std::wstring getName()
	{
		return this->name;
	}

	std::wstring isClicked(float x, float y)
	{
		for (auto &button : this->buttons)
		{
			if (button->getBoundingRectangle().IntersectsWith(Windows::Foundation::Rect(x, y,1,1)))
			{
				return button->getId();
			}
		}
		return L"false";
	}

	void resize(float scaleX, float scaleY)
	{
		for (auto &button : this->buttons)
		{
			button->resize(scaleX, scaleY);
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
	std::shared_ptr<DirectX::SpriteFont> 		spriteFont;
	std::wstring								name;
	std::vector<std::shared_ptr<TextButton>>	buttons;
};