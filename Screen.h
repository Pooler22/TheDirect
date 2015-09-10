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
	Screen(ID3D11ShaderResourceView* playerSpriteSheetIn, std::shared_ptr<DirectX::SpriteFont>  spriteFontIn, std::wstring nameIn, float scaleX, float scaleY)
	{
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		this->name = nameIn;
		this->spriteFont = spriteFontIn;
		this->buttonSpriteSheet = playerSpriteSheetIn;
		this->buttons = std::vector<std::shared_ptr<TextButton>>();
	};

	void addButton(TextButton* button)
	{
		this->buttons.push_back(std::shared_ptr<TextButton>(button));
	}

	void addButton(std::wstring name, std::wstring id, XMFLOAT2 position)
	{
		this->buttons.push_back(std::shared_ptr<TextButton>(new TextButton(buttonSpriteSheet.Get(), spriteFont, name, id, position, scaleX, scaleY)));
	}

	void addMenu(std::wstring* names, std::wstring* ids, XMFLOAT2* positions, int conut)
	{
		for (int i = 0; i < conut; i++)
		{
			this->buttons.push_back(std::shared_ptr<TextButton>(new TextButton(buttonSpriteSheet.Get(), spriteFont, names[i], ids[i], positions[i], scaleX, scaleY)));
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

	std::wstring isClicked(float x, float y)
	{
		for (auto &button : this->buttons)
		{
			if (button->isOver(Windows::Foundation::Rect(x, y, 1, 1)))
				return button->getId();
		}
		return L"false";
	}

	void setName(std::wstring stringIn)
	{
		this->name = stringIn;
	}

	std::wstring getName()
	{
		return this->name;
	}

	void setString(std::wstring idButton, std::wstring stinrg)
	{
		for (auto &button : buttons)
		{
			if (button->getId() == idButton)
				button->setString(stinrg);
		}
	}
	
	void resize(float scaleX, float scaleY)
	{
		this->scaleX = scaleX;
		this->scaleY = scaleY;
		for (auto &button : this->buttons)
		{
			button->resize(scaleX, scaleY);
		}
	}

private:
	float												scaleX;
	float												scaleY;
	std::wstring										name;
	std::shared_ptr<DirectX::SpriteFont> 				spriteFont;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	buttonSpriteSheet;
	std::vector<std::shared_ptr<TextButton>>			buttons;
};